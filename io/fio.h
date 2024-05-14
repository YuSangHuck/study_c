#ifndef FIO_H
#define FIO_H
#include <stdio.h>
#include "struct_def.h"

void fio_write(const char* filename, sample* samples, int cnt);
void fio_read(const char* filename, sample* samples, int cnt);

#endif /* FIO_H */
