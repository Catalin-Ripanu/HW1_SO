// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define AT_EMPTY_PATH 0x1000

int fstat(int fd, struct stat *st)
{
	if (fd < 0)
	{
		errno = EBADF;
		return -1;
	}
	return fstatat(fd, "", st, AT_EMPTY_PATH);
}
