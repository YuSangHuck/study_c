#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define LOG_TIMESTAMP_LEN 21
#define LOG_BUF_SIZE      8192
#define UINT              unsigned int
#define NULL              ((void *)0)

#define LOG(...)          log_internal(__VA_ARGS__)

void log_internal(const char *fmt, ...);
