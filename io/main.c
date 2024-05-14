#include <stdio.h>
#include "block_io.h"

int main() {
    struct sample samples[123 * 1024 / sizeof(struct sample)];
    FILE* file = fopen("data.bin", "wb");

    // 데이터를 파일에 쓰기
    block_write(file, samples, sizeof(samples));

    fclose(file);
    return 0;
}
