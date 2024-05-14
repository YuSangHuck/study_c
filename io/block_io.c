#include "block_io.h"

void block_write(FILE* file, void* data, size_t size) {
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0) {
        size_t bytes_to_write = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite((char*)data + offset, 1, bytes_to_write, file);
        remaining -= bytes_to_write;
        offset += bytes_to_write;
    }
}

void block_read(FILE* file, void* data, size_t size) {
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0) {
        size_t bytes_to_read = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fread((char*)data + offset, 1, bytes_to_read, file);
        remaining -= bytes_to_read;
        offset += bytes_to_read;
    }
}
