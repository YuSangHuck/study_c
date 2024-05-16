#include "block_io.h"
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

static void read_field_to_block(int fd, char* blk, int* offset, void* field, size_t size) {
    if (*offset + size > BLOCK_SIZE) {
        int pre_blk = BLOCK_SIZE - *offset;
        int post_blk = size - pre_blk;

        // c의 일부를 읽어옵니다.
        memcpy(field, &blk[*offset], pre_blk);
        *offset = 0;

        // 다음 블록을 읽어옵니다.
        read(fd, blk, BLOCK_SIZE);

        // c의 나머지를 읽어옵니다.
        memcpy(((char*)field) + pre_blk, &blk[*offset], post_blk);
        *offset += post_blk;
    } else {
        // 블록을 넘어가지 않는 경우 필드를 그대로 읽어옵니다.
        memcpy(field, &blk[*offset], size);
        *offset += size;
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

    for (int i = 0; i < cnt; ++i) {
        // 필드를 읽을 때마다 블록을 확인하고 필드가 블록을 넘어가는 경우 처리합니다.
        if (offset == 0) {
            read(fd, blk, BLOCK_SIZE);
        }

        read_field_to_block(fd, blk, &offset, &samples[i].c, sizeof(samples[i].c));
        read_field_to_block(fd, blk, &offset, &samples[i].i, sizeof(samples[i].i));
        read_field_to_block(fd, blk, &offset, &samples[i].l, sizeof(samples[i].l));
        read_field_to_block(fd, blk, &offset, &samples[i].d, sizeof(samples[i].d));

        // 다음 필드를 읽을 때 블록을 넘어갈 경우 offset을 초기화합니다.
        if (offset >= BLOCK_SIZE) {
            offset = 0;
        }
    }

    close(fd);
}
