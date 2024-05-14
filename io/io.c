#include "io.h"

void io_write(const char* filename, sample* samples, int cnt) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < cnt; ++i) {
        write(fd, &samples[i].c, sizeof(samples[i].c));
        write(fd, &samples[i].i, sizeof(samples[i].i));
        write(fd, &samples[i].l, sizeof(samples[i].l));
        write(fd, &samples[i].d, sizeof(samples[i].d));
    }

    close(fd);
}

void io_read(const char* filename, sample* samples, int cnt) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < cnt; ++i) {
        read(fd, &samples[i].c, sizeof(samples[i].c));
        read(fd, &samples[i].i, sizeof(samples[i].i));
        read(fd, &samples[i].l, sizeof(samples[i].l));
        read(fd, &samples[i].d, sizeof(samples[i].d));
    }

    close(fd);
}
