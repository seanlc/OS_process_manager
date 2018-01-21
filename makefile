CC= gcc
CFLAGS= -Wall -std=c99 -g

main: tests.c internals.c
	$(CC) $(CFLAGS) tests.c -o main

clean:
	rm -f main
