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
    const char* filename = "data.bin";

    // 1KB를 block_write
    measure_time(block_write_kb, filename, 1, "1KB");

    // 1MB를 block_write
    measure_time(block_write_mb, filename, 1, "1MB");

    // 1GB를 block_write
    measure_time(block_write_gb, filename, 1, "1GB");

    return 0;
}
