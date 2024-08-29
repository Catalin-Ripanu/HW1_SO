// SPDX-License-Identifier: BSD-3-Clause

#include <time.h>
#include <errno.h>
#include <internal/syscall.h>
#define LIMIT 1000000000

int nanosleep(const struct timespec *req, struct timespec *rem)
{
    if (req->tv_nsec < 0 || req->tv_nsec >= LIMIT)
    {
        errno = EINVAL;
        return -1;
    }
    int var = (int)syscall(__NR_nanosleep, req, rem);
    if (var == -1)
    {
        errno = EINTR;
        return -1;
    }
    return 0;
}