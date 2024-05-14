CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: main

main: main.o block_io.o
	$(CC) $(CFLAGS) -o main main.o block_io.o

main.o: main.c block_io.h
	$(CC) $(CFLAGS) -c main.c

block_io.o: block_io.c block_io.h
	$(CC) $(CFLAGS) -c block_io.c

clean:
	rm -f main main.o block_io.o
