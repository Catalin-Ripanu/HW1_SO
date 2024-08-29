// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <errno.h>
#include <time.h>

unsigned int sleep(unsigned int seconds)
{
    struct timespec aux;
    aux.tv_sec = seconds;
    aux.tv_nsec = 0;
    if (nanosleep(&aux, &aux))
        return aux.tv_sec;
    return 0;
}