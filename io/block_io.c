#include "block_io.h"

void block_write(int fd, void* data, size_t size) {
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0) {
        size_t bytes_to_write = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        ssize_t bytes_written = write(fd, (char*)data + offset, bytes_to_write);
        if (bytes_written == -1) {
            perror("Failed to write to file");
            return;
        }
        remaining -= bytes_written;
        offset += bytes_written;
    }
}

void block_read(int fd, void* data, size_t size) {
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0) {
        size_t bytes_to_read = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        ssize_t bytes_read = read(fd, (char*)data + offset, bytes_to_read);
        if (bytes_read == -1) {
            perror("Failed to read from file");
            return;
        }
        remaining -= bytes_read;
        offset += bytes_read;
    }
}

void block_write_kb(const char* filename, size_t size_kb) {
    struct sample samples[size_kb * 1024 / sizeof(struct sample)];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    block_write(fd, samples, sizeof(samples));
    close(fd);
}

void block_write_mb(const char* filename, size_t size_mb) {
    struct sample samples[size_mb * 1024 * 1024 / sizeof(struct sample)];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    block_write(fd, samples, sizeof(samples));
    close(fd);
}

void block_write_gb(const char* filename, size_t size_gb) {
    struct sample samples[size_gb * 1024 * 1024 * 1024 / sizeof(struct sample)];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    block_write(fd, samples, sizeof(samples));
    close(fd);
}
