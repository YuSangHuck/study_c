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
// static void read_field_from_block(int fd, char* blk, int* offset, void* field, size_t size);

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
static void write_value_to_block(int fd, char* blk, int* offset, void* value, size_t size) {
    write_field_to_block(fd, blk, offset, &value, size);
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
        print_sample(&samples[i]);
        // write_field_to_block(fd, blk, &offset, &samples[i].c, sizeof(samples[i].c));
        // write_field_to_block(fd, blk, &offset, &samples[i].i, sizeof(samples[i].i));
        // write_field_to_block(fd, blk, &offset, &samples[i].l, sizeof(samples[i].l));
        write_value_to_block(fd, blk, &offset, (void *)samples[i].c, sizeof(samples[i].c));
        write_value_to_block(fd, blk, &offset, (void *)samples[i].i, sizeof(samples[i].i));
        write_value_to_block(fd, blk, &offset, (void *)samples[i].l, sizeof(samples[i].l));
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
        print_sample(&samples[i]);
    }

    close(fd);
}

#define BLOCK_SIZE 76  // 19 * 4 바이트

int test_xxd_char() {
    unsigned char blk[BLOCK_SIZE] = {
        0x00,0x00,0x00,0x00,  0x01,0x00,0x00,0x00,  0x1a,0x00,0x00,0x00,  0x34,0x00,0x4a,0x00,
        0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
        0x26,0xd7,0x57,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,  0x01,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x01,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,  0x01,0x00,0x00,0x00,  0x01,0x00,0x00,0x00
    };

    int fd = open("output.dat.char", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("파일 열기 실패");
        return 1;
    }

    ssize_t written = write(fd, blk, sizeof(blk));
    if (written == -1) {
        perror("파일 쓰기 실패");
        close(fd);
        return 1;
    } else if (written != sizeof(blk)) {
        fprintf(stderr, "쓰기 크기 불일치: %zd 바이트 기록됨, %zu 바이트 기대됨\n", written, sizeof(blk));
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int test_xxd_int() {
    unsigned int blk[BLOCK_SIZE/4] = {
        0x00000000, 0x00000001, 0x0000001a, 0x004a0034,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x0057d726, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000001, 0x00000000, 0x00000001,
        0x00000000, 0x00000001, 0x00000001
    };

    for (size_t i = 0; i < BLOCK_SIZE/4; i++)
    {
        unsigned char *blk_char = (unsigned char *)(blk+i);
        // printf("0x");
        for (size_t j = 0; j < 4; j++)
        {
            printf("%02x", *(blk_char + j));
            if ((j+1)%2 == 0)
                printf(" ");
        }
        if ((i+1)%4 == 0)
            printf("\n");
    }
    printf("\n");


    int fd = open("output.dat.int", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("파일 열기 실패");
        return 1;
    }

    ssize_t written = write(fd, blk, sizeof(blk));
    if (written == -1) {
        perror("파일 쓰기 실패");
        close(fd);
        return 1;
    } else if (written != sizeof(blk)) {
        fprintf(stderr, "쓰기 크기 불일치: %zd 바이트 기록됨, %zu 바이트 기대됨\n", written, sizeof(blk));
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}