#ifndef BLOCK_IO_H
#define BLOCK_IO_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/stat.h>

#define BLOCK_SIZE 65536 // 64KB

struct sample {
    int* arr_int;
    char* arr_char;
};

void block_write(int fd, void* data, size_t size);
void block_read(int fd, void* data, size_t size);

void block_write_kb(const char* filename, size_t size_kb);
void block_write_mb(const char* filename, size_t size_mb);
void block_write_gb(const char* filename, size_t size_gb);

#endif /* BLOCK_IO_H */
