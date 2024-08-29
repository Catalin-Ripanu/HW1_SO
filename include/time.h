#ifndef _TIME_H
#define _TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
#define NULL 0L
#else
#endif
    typedef long time_t;
    struct timespec
    {
        time_t tv_sec;
        long tv_nsec;
    };
    struct tm
    {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
        long __tm_gmtoff;
        const char *__tm_zone;
    };

#define CLOCKS_PER_SEC 1000000L

#define TIME_UTC 1
#define __NEED_struct_timespec
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3
#define CLOCK_MONOTONIC_RAW 4
#define CLOCK_REALTIME_COARSE 5
#define CLOCK_MONOTONIC_COARSE 6
#define CLOCK_BOOTTIME 7
#define CLOCK_REALTIME_ALARM 8
#define CLOCK_BOOTTIME_ALARM 9
#define CLOCK_SGI_CYCLE 10
#define CLOCK_TAI 11

#define TIMER_ABSTIME 1

    int nanosleep(const struct timespec *t1, struct timespec *t2);

#ifdef __cplusplus
}
#endif

#endif