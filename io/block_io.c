#include "block_io.h"

void block_io_write(const char* filename, sample* samples, int cnt) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    char blk[BLOCK_SIZE];
    int offset = 0;

    for (int i = 0; i < cnt; ++i) {
        // Ensure offset is within bounds before memcpy
        if (offset + sizeof(samples[i].c) <= BLOCK_SIZE) {
            memcpy(&blk[offset], &samples[i].c, sizeof(samples[i].c));
            offset += sizeof(samples[i].c);
        } else {
            int pre_blk = BLOCK_SIZE - offset;
            int post_blk = sizeof(samples[i].c) - pre_blk;
            // c의 일부
            memcpy(&blk[offset], (const char*)&samples[i].c, pre_blk);
            write(fd, blk, BLOCK_SIZE);
            offset = 0;

            // c의 나머지
            memcpy(&blk[offset], (const char*)&samples[i].c + pre_blk, post_blk);
            offset += post_blk;
        }

        if (offset + sizeof(samples[i].i) <= BLOCK_SIZE) {
            memcpy(&blk[offset], &samples[i].i, sizeof(samples[i].i));
            offset += sizeof(samples[i].i);
        } else {
            int pre_blk = BLOCK_SIZE - offset;
            int post_blk = sizeof(samples[i].i) - pre_blk;
            // i의 일부
            memcpy(&blk[offset], (const char*)&samples[i].i, pre_blk);
            write(fd, blk, BLOCK_SIZE);
            offset = 0;

            // i의 나머지
            memcpy(&blk[offset], (const char*)&samples[i].i + pre_blk, post_blk);
            offset += post_blk;
        }

        if (offset + sizeof(samples[i].l) <= BLOCK_SIZE) {
            memcpy(&blk[offset], &samples[i].l, sizeof(samples[i].l));
            offset += sizeof(samples[i].l);
        } else {
            int pre_blk = BLOCK_SIZE - offset;
            int post_blk = sizeof(samples[i].l) - pre_blk;
            // l의 일부
            memcpy(&blk[offset], (const char*)&samples[i].l, pre_blk);
            write(fd, blk, BLOCK_SIZE);
            offset = 0;

            // l의 나머지
            memcpy(&blk[offset], (const char*)&samples[i].l + pre_blk, post_blk);
            offset += post_blk;

        }

        if (offset + sizeof(samples[i].d) <= BLOCK_SIZE) {
            memcpy(&blk[offset], &samples[i].d, sizeof(samples[i].d));
            offset += sizeof(samples[i].d);
        } else {
            int pre_blk = BLOCK_SIZE - offset;
            int post_blk = sizeof(samples[i].d) - pre_blk;
            // d의 일부
            memcpy(&blk[offset], (const char*)&samples[i].d, pre_blk);
            write(fd, blk, BLOCK_SIZE);
            offset = 0;

            // d의 나머지
            memcpy(&blk[offset], (const char*)&samples[i].d + pre_blk, post_blk);
            offset += post_blk;

        }

        if (offset >= BLOCK_SIZE) {
            write(fd, blk, BLOCK_SIZE);
            offset = 0;
        }
    }

    if (offset > 0) {
        write(fd, blk, offset);
    }

    close(fd);
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
        if (offset == 0) {
            read(fd, blk, BLOCK_SIZE);
        }

        memcpy(&samples[i].c, &blk[offset], sizeof(samples[i].c));
        offset += sizeof(samples[i].c);

        memcpy(&samples[i].i, &blk[offset], sizeof(samples[i].i));
        offset += sizeof(samples[i].i);

        memcpy(&samples[i].l, &blk[offset], sizeof(samples[i].l));
        offset += sizeof(samples[i].l);

        memcpy(&samples[i].d, &blk[offset], sizeof(samples[i].d));
        offset += sizeof(samples[i].d);

        if (offset >= BLOCK_SIZE) {
            offset = 0;
        }
    }

    close(fd);
}
