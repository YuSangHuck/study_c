#include <stdio.h>
#include <time.h>
#include "block_io.h"

void measure_time(void (*function)(const char*, size_t), const char* filename, size_t size, const char* label) {
    clock_t start, end;
    double elapsed_time;

    start = clock();
    function(filename, size);
    end = clock();

    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time for %s: %f seconds\n", label, elapsed_time);
}

int main() {
    const char* filename_kb = "data_kb.bin";
    const char* filename_mb = "data_mb.bin";
    const char* filename_gb = "data_gb.bin";

    // 1KB를 block_write
    measure_time(block_write_kb, filename_kb, 1, "1KB");

    // 1MB를 block_write
    measure_time(block_write_mb, filename_mb, 1, "1MB");

    // 1GB를 block_write
    measure_time(block_write_gb, filename_gb, 1, "1GB");

    return 0;
}
