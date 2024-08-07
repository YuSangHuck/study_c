#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct_def.h"
#include "io.h"
#include "block_io.h"
#include "fio.h"

void setup_sample(sample* samples, int cnt) {
    for (size_t i = 0; i < cnt; i++) {
        samples[i].c = cnt - i;
        samples[i].i = cnt - i;
        samples[i].l = cnt - i;
        samples[i].d = cnt - i;
    }
}
void print_sample(sample* sample) {
    printf("c: %d, i: %d, l: %ld, d: %f\n", sample->c, sample->i, sample->l, sample->d);
}

void measure_time(void (*function)(const char*, sample*, int), const char* filename, sample* samples, int cnt, const char* label) {
    clock_t start, end;
    double elapsed_time;

    start = clock();
    function(filename, samples, cnt);
    end = clock();

    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time for %20s: %3f seconds\n", label, elapsed_time);
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

    printf("SIZEOF_SAMPLE: %ld\n", SIZEOF_SAMPLE);

    // Dynamically allocate sample arrays
    size_t cnt_1kb = 1024 / SIZEOF_SAMPLE;
    size_t cnt_1mb = 1024 * cnt_1kb;
    size_t cnt_1gb = 1024 * cnt_1mb;

    sample* sample_1kb = malloc(cnt_1kb * sizeof(sample));
    sample* sample_1mb = malloc(cnt_1mb * sizeof(sample));
    sample* sample_1gb = malloc(cnt_1gb * sizeof(sample));

    // setup
    setup_sample(sample_1kb, cnt_1kb);
    setup_sample(sample_1mb, cnt_1mb);
    setup_sample(sample_1gb, cnt_1gb);

    // Measure time for io write
    measure_time(io_write, filename_1kb_io, sample_1kb, cnt_1kb, "io_write;  1kb");
    // measure_time(io_write, filename_1mb_io, sample_1mb, cnt_1mb, "io_write;  1mb");
    // measure_time(io_write, filename_1gb_io, sample_1gb, cnt_1gb, "io_write;  1gb");

    measure_time(block_io_write, filename_1kb_block_io, sample_1kb, cnt_1kb, "block_io_write;  1kb");
    // measure_time(block_io_write, filename_1mb_block_io, sample_1mb, cnt_1mb, "block_io_write;  1mb");
    // measure_time(block_io_write, filename_1gb_block_io, sample_1gb, cnt_1gb, "block_io_write;  1gb");

    measure_time(fio_write, filename_1kb_fio, sample_1kb, cnt_1kb, "fio_write;  1kb");
    // measure_time(fio_write, filename_1mb_fio, sample_1mb, cnt_1mb, "fio_write;  1mb");
    // measure_time(fio_write, filename_1gb_fio, sample_1gb, cnt_1gb, "fio_write;  1gb");

    // Measure time for io read
    measure_time(io_read, filename_1kb_io, sample_1kb, cnt_1kb, "io_read;  1kb");
    // measure_time(io_read, filename_1mb_io, sample_1mb, cnt_1mb, "io_read;  1mb");
    // measure_time(io_read, filename_1gb_io, sample_1gb, cnt_1gb, "io_read;  1gb");

    measure_time(block_io_read, filename_1kb_block_io, sample_1kb, cnt_1kb, "block_io_read;  1kb");
    // measure_time(block_io_read, filename_1mb_block_io, sample_1mb, cnt_1mb, "block_io_read;  1mb");
    // measure_time(block_io_read, filename_1gb_block_io, sample_1gb, cnt_1gb, "block_io_read;  1gb");

    measure_time(fio_read, filename_1kb_fio, sample_1kb, cnt_1kb, "fio_read;  1kb");
    // measure_time(fio_read, filename_1mb_fio, sample_1mb, cnt_1mb, "fio_read;  1mb");
    // measure_time(fio_read, filename_1gb_fio, sample_1gb, cnt_1gb, "fio_read;  1gb");

    // Free allocated memory
    free(sample_1kb);
    free(sample_1mb);
    free(sample_1gb);

    test_xxd_char();
    test_xxd_int();

    return 0;
}
