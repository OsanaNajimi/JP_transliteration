CC = gcc
CFLAGS=-Wall -g -fsanitize=address
exe = jp_transliteration
all: $(exe)
%: %.c
	${CC} ${CFLAGS} $< -o $@ 
clean:
	rm  ${exe}
