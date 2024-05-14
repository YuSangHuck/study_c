#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct_def.h"
#include "io.h"
#include "block_io.h"
#include "fio.h"

void measure_time(void (*function)(const char*, sample*, int), const char* filename, sample* samples, int cnt, const char* label) {
    clock_t start, end;
    double elapsed_time;

    start = clock();
    function(filename, samples, cnt);
    end = clock();

    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time for %s: %f seconds\n", label, elapsed_time);
}

int main() {
    const char* filename_1kb_io = "1kb_io.dat";
    const char* filename_1mb_io = "1mb_io.dat";
    const char* filename_1gb_io = "1gb_io.dat";
    const char* filename_1kb_block_io = "1kb_block_io.dat";
    const char* filename_1mb_block_io = "1mb_block_io.dat";
    const char* filename_1gb_block_io = "1gb_block_io.dat";
    const char* filename_1kb_fio = "1kb_fio.dat";
    const char* filename_1mb_fio = "1mb_fio.dat";
    const char* filename_1gb_fio = "1gb_fio.dat";

    // Dynamically allocate sample arrays
    int cnt_1kb = 1024 / SIZEOF_SAMPLE_WRITE;
    int cnt_1mb = 1024 * 1024 / SIZEOF_SAMPLE_WRITE;
    int cnt_1gb = 1024 * 1024 * 1024 / SIZEOF_SAMPLE_WRITE;

    sample* sample_1kb = malloc(cnt_1kb * sizeof(sample));
    sample* sample_1mb = malloc(cnt_1mb * sizeof(sample));
    sample* sample_1gb = malloc(cnt_1gb * sizeof(sample));

    // Measure time for io
    measure_time(io_write, filename_1kb_io, sample_1kb, cnt_1kb, "1kb io");
    measure_time(io_write, filename_1mb_io, sample_1mb, cnt_1mb, "1mb io");
    measure_time(io_write, filename_1gb_io, sample_1gb, cnt_1gb, "1gb io");

    measure_time(block_io_write, filename_1kb_block_io, sample_1kb, cnt_1kb, "1kb block_io");
    measure_time(block_io_write, filename_1mb_block_io, sample_1mb, cnt_1mb, "1mb block_io");
    measure_time(block_io_write, filename_1gb_block_io, sample_1gb, cnt_1gb, "1gb block_io");

    measure_time(fio_write, filename_1kb_fio, sample_1kb, cnt_1kb, "1kb fio");
    measure_time(fio_write, filename_1mb_fio, sample_1mb, cnt_1mb, "1mb fio");
    measure_time(fio_write, filename_1gb_fio, sample_1gb, cnt_1gb, "1gb fio");

    // Free allocated memory
    free(sample_1kb);
    free(sample_1mb);
    free(sample_1gb);

    return 0;
}
