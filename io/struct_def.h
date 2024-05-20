#ifndef STRUCT_DEF_H
#define STRUCT_DEF_H

typedef struct sample {
    char c;
    short s;
    int i;
    unsigned int ui;
    long l;
    float f;
    double d;
} sample;

// char c; int i; long l;
#define SIZEOF_SAMPLE (sizeof(char) + sizeof(int) + sizeof(long))

#endif /* STRUCT_DEF_H */
