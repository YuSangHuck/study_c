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

// char c; int i; long l; double d;
#define SIZEOF_SAMPLE (sizeof(char) + sizeof(int) + sizeof(long) + sizeof(double))

#endif /* STRUCT_DEF_H */
