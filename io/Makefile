CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: main

main: main.o io.o block_io.o fio.o
	$(CC) $(CFLAGS) -o main main.o io.o block_io.o fio.o

main.o: main.c io.h block_io.h fio.h
	$(CC) $(CFLAGS) -c main.c

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c

block_io.o: block_io.c block_io.h
	$(CC) $(CFLAGS) -c block_io.c

fio.o: fio.c fio.h
	$(CC) $(CFLAGS) -c fio.c

clean:
	rm -f main main.o io.o block_io.o fio.o
