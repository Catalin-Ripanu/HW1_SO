// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <stdarg.h>
#include <errno.h>

int close(int fd)
{
	int var;
	var = (int)syscall(__NR_close, fd);
	if (var < 0)
	{
		errno = -var;
		return -1;
	}
	return var;
}
