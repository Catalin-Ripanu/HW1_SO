// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "unistd.h"

#define AT_FDCWD -100
#define AT_EMPTY_PATH 0x1000

int stat(const char *restrict path, struct stat *restrict buf)
{
	return fstatat(AT_FDCWD, path, buf, AT_EMPTY_PATH);
}
