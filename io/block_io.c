#include "block_io.h"

/**
 * @brief Write the specified field to the block buffer.
 * If writing the field would exceed the block size, the block buffer is written to the file,
 * and the offset is reset to 0.
 *
 * @param fd  File descriptor of the file to write to.
 * @param blk  Block buffer where the field will be written.
 * @param offset  Pointer to the current offset within the block buffer.
 * @param field  Pointer to the field data to be written.
 * @param size  Size of the field to be written.
 */
static void write_field_to_block(int fd, char* blk, int* offset, void* field, size_t size) {
    int remain = size;
    while (remain > 0) {
        int space_left_in_block = BLOCK_SIZE - *offset;
        int bytes_to_copy = remain < space_left_in_block ? remain : space_left_in_block;
        memcpy(&blk[*offset], field + (size - remain), bytes_to_copy);
        remain -= bytes_to_copy;
        *offset += bytes_to_copy;
        if (*offset >= BLOCK_SIZE) {
            write(fd, blk, BLOCK_SIZE);
            *offset = 0;
        }
    }
}

void block_io_write(const char* filename, sample* samples, int cnt) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    char blk[BLOCK_SIZE];
    int offset = 0;

    for (int i = 0; i < cnt; ++i) {
        // print_sample(&samples[i]);
        write_field_to_block(fd, blk, &offset, &samples[i].c, sizeof(samples[i].c));
        write_field_to_block(fd, blk, &offset, &samples[i].i, sizeof(samples[i].i));
        write_field_to_block(fd, blk, &offset, &samples[i].l, sizeof(samples[i].l));
        write_field_to_block(fd, blk, &offset, &samples[i].d, sizeof(samples[i].d));
    }

    if (offset > 0) {
        write(fd, blk, offset);
    }

    close(fd);
}

/**
 * @brief Read the specified field from the block buffer.
 * If the field spans multiple blocks, it reads from multiple blocks.
 *
 * @param fd  File descriptor of the file to read from.
 * @param blk  Block buffer where the field will be read from.
 * @param offset  Pointer to the current offset within the block buffer.
 * @param field  Pointer to the location where the field data will be stored.
 * @param size  Size of the field to be read.
 */
static void read_field_from_block(int fd, char* blk, int* offset, void* field, size_t size) {
    // printf("offset: %d, size: %d\n", *offset, size);

    int remain = size;
    while (remain > 0) {
        int space_left_in_block = BLOCK_SIZE - *offset;
        // printf(" space_left_in_block: %d\n", space_left_in_block);
        int bytes_to_copy = remain < space_left_in_block ? remain : space_left_in_block;
        // printf(" bytes_to_copy: %d\n", bytes_to_copy);
        memcpy(field + (size - remain), &blk[*offset], bytes_to_copy);
        remain -= bytes_to_copy;
        *offset += bytes_to_copy;
        if (*offset >= BLOCK_SIZE) {
            // printf(" read\n");
            read(fd, blk, BLOCK_SIZE);
            *offset = 0;
        }
    }
}
void block_io_read(const char* filename, sample* samples, int cnt) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    char blk[BLOCK_SIZE];
    int offset = 0;

    read(fd, blk, BLOCK_SIZE);

    for (int i = 0; i < cnt; ++i) {
        read_field_from_block(fd, blk, &offset, &samples[i].c, sizeof(samples[i].c));
        read_field_from_block(fd, blk, &offset, &samples[i].i, sizeof(samples[i].i));
        read_field_from_block(fd, blk, &offset, &samples[i].l, sizeof(samples[i].l));
        read_field_from_block(fd, blk, &offset, &samples[i].d, sizeof(samples[i].d));
        // print_sample(&samples[i]);
    }

    close(fd);
}
