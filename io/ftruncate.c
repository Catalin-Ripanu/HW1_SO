// SPDX-License-Identifier: BSD-3-Clause

#include <unistd.h>
#include <internal/syscall.h>
#include <errno.h>

int ftruncate(int fd, off_t length)
{
	int var = (int)syscall(__NR_ftruncate, fd, length);
	if (var < 0)
	{
		errno = -var;
		return -1;
	}
	return var;
}
