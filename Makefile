CC=gcc
CFLAGS=­DLINUX ­ansi ­Wall ­D_GNU_SOURCE g
CPPFLAGS=
all:	shell
shell:	shell.o
	$(CC) -o shell shell.o
shell.o:	shell.c
	$(CC) -std=c99 -c shell.c
clean:	
	rm -f shell shell.o
