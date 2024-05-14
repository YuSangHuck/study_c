#include "fio.h"

void fio_write(const char* filename, sample* samples, int cnt) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < cnt; ++i) {
        fwrite(&samples[i].c, sizeof(samples[i].c), 1, file);
        fwrite(&samples[i].i, sizeof(samples[i].i), 1, file);
        fwrite(&samples[i].l, sizeof(samples[i].l), 1, file);
        fwrite(&samples[i].d, sizeof(samples[i].d), 1, file);
    }

    fclose(file);
}

void fio_read(const char* filename, sample* samples, int cnt) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < cnt; ++i) {
        fread(&samples[i].c, sizeof(samples[i].c), 1, file);
        fread(&samples[i].i, sizeof(samples[i].i), 1, file);
        fread(&samples[i].l, sizeof(samples[i].l), 1, file);
        fread(&samples[i].d, sizeof(samples[i].d), 1, file);
    }

    fclose(file);
}
