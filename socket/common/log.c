#include "log.h"

static void
getlocaltime(UINT *year, UINT *month, UINT *day, UINT *hour, UINT *min,
                UINT *sec, UINT *usec) {
    static time_t base_next_hour_sec = 0;
    static time_t base_tv_sec = 0;
    static struct tm base_tm;
    struct timeval tv;
    int calc_min;
    int calc_sec;
    time_t diff_sec;
    time_t cur_sec;

    gettimeofday(&tv, NULL);
    cur_sec = tv.tv_sec;
    *usec = tv.tv_usec;

    /* The first call to this function, or at most once in each hour:
     * use (slower) localtime_r to initialize base_date & base_tv_sec.
     */
    if (base_next_hour_sec == 0 || base_next_hour_sec <= cur_sec) {

        /* Must call localtime_r for checking DST! */
        localtime_r(&tv.tv_sec, &base_tm);

        base_tv_sec = cur_sec;

        calc_min = base_tm.tm_min;
        calc_sec = base_tm.tm_sec;

        base_next_hour_sec = cur_sec + ((60 - (base_tm.tm_min + 1)) * 60 +
                                        (60 - base_tm.tm_sec));
    }
    else {
        /* Now we are in the same hour as in base_date:
         * simply calculate the minute and second. */
        diff_sec = cur_sec - base_tv_sec;

        calc_min = diff_sec / 60;
        calc_sec = diff_sec % 60;

        if (calc_sec + base_tm.tm_sec >= 60) {
            calc_sec = (calc_sec + base_tm.tm_sec) - 60;
            calc_min = (calc_min + base_tm.tm_min) + 1;
        }
        else {
            calc_sec = (calc_sec + base_tm.tm_sec);
            calc_min = (calc_min + base_tm.tm_min);
        }
    }

    *year = base_tm.tm_year + 1900;
    *month = base_tm.tm_mon + 1;
    *day = base_tm.tm_mday;
    *hour = base_tm.tm_hour;
    *min = calc_min;
    *sec = calc_sec;
}

static void
log_format(char *buf, const char *fmt, va_list ap) {
    char *ptr = buf;
    UINT year, month, day, hour, min, sec, usec;

    /* 1. date */
    *(ptr++) = '[';
    getlocaltime(&year, &month, &day, &hour, &min, &sec, &usec);
    sprintf(ptr, "%02d-%02dT%02d:%02d:%02d.%06d", month, day, hour, min, sec,
            usec);
    ptr += LOG_TIMESTAMP_LEN;
    *(ptr++) = ']';
    *(ptr++) = ' ';

    /* 2. message */
    *ptr = '\0';

    if (fmt != NULL)
        vsnprintf(ptr, LOG_BUF_SIZE - strlen(buf) - 2, fmt, ap);

    ptr += strlen(ptr);

    *(ptr++) = '\n';
    *(ptr++) = '\0';
}

void
log_internal(const char *fmt, ...) {
    char log_buf[LOG_BUF_SIZE];
    va_list ap;

    va_start(ap, fmt);

    log_format(log_buf, fmt, ap);
    printf("%s", log_buf);

    va_end(ap);
}
