#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "struct_def.h"

void io_write(const char* filename, sample* samples, int cnt);
void io_read(const char* filename, sample* samples, int cnt);

#endif /* IO_H */
