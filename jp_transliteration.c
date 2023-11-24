#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define STRLEN 1000

const char Gojyuon[17][5][4] =
{
    {
        "あ","い","う","え","お"
    },
    {
        "か","き","く","け","こ"
    },
    {
        "さ","し","す","せ","そ"
    },
    {
        "た","ち","つ","て","と"
    },
    {
        "な","に","ぬ","ね","の"
    },
    {
        "は","ひ","ふ","へ","ほ"
    },
    {
        "ま","み","む","め","も"
    },
    {
        "や","い","ゆ","え","よ"
    },
    {
        "ら","り","る","れ","ろ"
    },
    {
        "わ","ゐ","う","ゑ","を"
    },
    {
        "が","ぎ","ぐ","げ","ご"
    },
    {
        "ざ","じ","ず","ぜ","ぞ"
    },
    {
        "だ","ぢ","づ","で","ど"
    },
    {
        "ぱ","ぴ","ぷ","ぺ","ぽ"
    },
    {
        "ば","び","ぶ","べ","ぼ"
    },
    {
        "ゃ","ぃ","ゅ","ぇ","ょ"
    },
    {
        "","","っ","",""
    }
};

const char consonants[15] = "kstnhmyrwgzdpb";
const char vowels[6] = "aiueo";
const char longvowels[11] = "āīūēō";
const char punctuation[6] = "-,.?!";
const char punctuation_jp[16] = "ー、。？！";

int whichLongVowel(char c1, char c2){
    for(int i=0;i<strlen(vowels);i++){
        if(c1 == longvowels[i*2] && c2 == longvowels[i*2+1]){
            return i;
        }
    }
    return -1;
}

int whichPunctuation(char c){
    for(int i=0;i<strlen(punctuation);i++){
        if(c == punctuation[i]){
            return i;
        }
    }
    return -1;
}

int nandan(char c){
    for(int i=0;i<5;i++){
        if(c == vowels[i])
            return i;
    }
    return -1;
}

int nangyou(char str[STRLEN], int i, int j){
    if(j == 0)
        return 0;
    if(j == 1){
        if(str[i] == 'f' && str[i+1] == 'u')
            return 5;
        if(str[i] == 'j'){
            if(str[i+1] == 'i')
                return 11;
            else
                return 111;
        }
        int k;
        for(k = 0; k < 14 && str[i] != consonants[k]; k++);
        if(k < 14)
            return k + 1;
    }
    if(j == 2){
        if(str[i] == 's' && str[i+1] == 'h'){
            if(str[i+2] == 'i')
                return 2;
            else
                return 102;
        }
        if(str[i] == 'c' && str[i+1] == 'h'){
            if(str[i+2] == 'i')
                return 3;
            else
                return 103;
        }
        if(str[i] == 't' && str[i+1] == 's' && str[i+2] == 'u')
            return 3;
        if(str[i] == 'd' && str[i+1] == 'z' && str[i+2] == 'u')
                return 12;
        int k;
        for(k = 0; k < 14 && str[i] != consonants[k]; k++);
        if(k < 14 && str[i+1] == 'y')
            return 100 + k + 1;
    }
    return -1;
}

char* gojyuon(char str[STRLEN], int i, int j){
    char *kana = calloc(10, sizeof(char));
    if(j > 0 && str[i] == str[i+1]){
        strcat(kana, Gojyuon[16][2]);
        i++;
        j--;
    }
    int dan = nandan(str[i+j]);
    int gyou = nangyou(str, i, j);
    if(dan < 0 || gyou < 0){
        free(kana);
        return NULL;
    }
    if(gyou < 15){
        strcat(kana, Gojyuon[gyou][dan]);
    }
    else{
        strcat(kana,Gojyuon[gyou-100][1]);
        strcat(kana,Gojyuon[15][dan]);
    }
    return kana;
}

char* preprocess(char raw[STRLEN]){
    char* str = calloc(STRLEN, sizeof(char));
    int i=0,j=0;
    while(i<strlen(raw)){
        if(isalpha(raw[i]) || (i+1 < strlen(raw) && whichLongVowel(raw[i],raw[i+1]) >= 0) || whichPunctuation(raw[i]) >= 0){
            int k = -1;
            if(i + 1 < strlen(raw)){
                k = whichLongVowel(raw[i],raw[i+1]);
            }
            if(k >= 0){
                str[j++] = vowels[k];
                str[j++] = vowels[2];
                i += 2;
            }
            else{
                bool is_n = false;
                raw[i] = tolower(raw[i]);
                if(raw[i] == 'n'){
                    if(i+1 == strlen(raw))
                        is_n = true;
                    if(nandan(raw[i+1]) < 0 && raw[i+1] != 'y' && raw[i+1] != 'n' && raw[i-1] != 'n')
                        if(i+2 == strlen(raw) || whichLongVowel(raw[i+1],raw[i+2]) < 0)
                            is_n = true;
                }
                if(is_n){
                    str[j++] = 'n';
                    str[j++] = 'n';
                }
                else{
                    if(raw[i] == 'e' && raw[i-1] == ' ' && (i+2 == strlen(raw) || !isalpha(raw[i+1])))
                        str[j++] = 'h';
                    if(raw[i] == 'w' && raw[i-1] == ' ' && i+2 < strlen(raw) && raw[i+1] == 'a' && (i+3 == strlen(raw) || !isalpha(raw[i+2])))
                        raw[i] = 'h';
                    str[j++] = raw[i];
                }
                i++;
            }
                
        }
        else
            i++;
    }
    return str;
}

char* transliterate(char raw[STRLEN]){
    char* str = preprocess(raw);
    // printf("%s\n",str);
    char* str_kana = calloc(3 * STRLEN, sizeof(char));
    int i=0;
    while(i < strlen(str)){
        int k = whichPunctuation(str[i]);
        if(k >= 0){
            strncat(str_kana,&punctuation_jp[k*3],3);
            i += 1;
        }
        else{
            if(i + 1 < strlen(str) && str[i] == 'n' && str[i+1] == 'n'){
                strcat(str_kana,"ん");
                i += 2;
            }
            else{
                int j = 0;
                while(i + j < strlen(str) && nandan(str[i+j]) < 0){
                    j++;
                }
                char* kana = gojyuon(str,i,j);
                if(kana == NULL){
                    printf("%s\n",str_kana);
                    free(str);
                    free(str_kana);
                    return NULL;
                }
                strcat(str_kana,kana);
                free(kana);
                i += j + 1;
            }
        }    
    }
    free(str);
    return str_kana;
}

int main(){
    char str[STRLEN];
    fgets(str,STRLEN,stdin);
    char* str_kana = transliterate(str);
    if(str_kana == NULL)
        printf("syntax error\n");
    else{
        printf("%s\n",str_kana);
        free(str_kana);
    }
}