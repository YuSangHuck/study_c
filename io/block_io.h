#ifndef BLOCK_IO_H
#define BLOCK_IO_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "struct_def.h"

#define BLOCK_SIZE 65536 // 64KB
// #define BLOCK_SIZE 1 // 64KB

void block_io_write(const char* filename, sample* samples, int cnt);
void block_io_read(const char* filename, sample* samples, int cnt);

#endif /* BLOCK_IO_H */
