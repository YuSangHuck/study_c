#ifndef BLOCK_IO_H
#define BLOCK_IO_H

#include <stdio.h>
// #include <stdlib.h>

#define BLOCK_SIZE 65536 // 64KB

struct sample {
    int* arr_int;
    char* arr_char;
};

// block_write 함수 선언
void block_write(FILE* file, void* data, size_t size);

// block_read 함수 선언
void block_read(FILE* file, void* data, size_t size);

#endif /* BLOCK_IO_H */
