CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = my_program

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o
