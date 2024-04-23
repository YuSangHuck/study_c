#define _GNU_SOURCE
#include <stdio.h>
#include <execinfo.h>

void print_trace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Call stack:\n");
    for (i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }

    free(strings);
}

#define PRINT_WITH_FILE_LINE(fmt, ...) \
    printf("%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define printf(fmt, ...) PRINT_WITH_FILE_LINE(fmt, ##__VA_ARGS__)

void some_function() {
    printf("here!\n");
    print_trace();
}

int main() {
    some_function();
    return 0;
}