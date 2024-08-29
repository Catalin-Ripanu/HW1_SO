/* SPDX-License-Identifier: BSD-3-Clause */

#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include <internal/syscall.h>
#include <internal/io.h>
#include "errno.h"

int puts(const char *str)
{
    int err = write(1, str, strlen(str));
    if (err == -1)
    {
        errno = EIO;
        return EOF;
    }
    err = write(1, "\n", 1);
    if (err == -1)
    {
        errno = EIO;
        return EOF;
    }
    return 0;
}