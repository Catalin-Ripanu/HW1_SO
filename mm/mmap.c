// SPDX-License-Identifier: BSD-3-Clause

#include <sys/mman.h>
#include <errno.h>
#include <sys/stat.h>
#include <internal/syscall.h>

#define OFF_MASK 0xFFFFFFFFFFFFEFFF

void helper_function(volatile void *addr)
{
	syscall(__NR_futex, (long)addr, 9, 0, NULL, NULL, 0);
}

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	long val;
	struct stat *aux = NULL;

	if ((fstat(fd, aux) < 0) && ((flags & MAP_ANONYMOUS) == 0))
	{
		return MAP_FAILED;
	}

	if (fd < 0 && flags == MAP_ANONYMOUS)
	{
		errno = EINVAL;
		return MAP_FAILED;
	}

	if (length >= __LONG_MAX__)
	{
		errno = ENOMEM;
		return MAP_FAILED;
	}

	if (offset & OFF_MASK)
	{
		errno = EINVAL;
		return MAP_FAILED;
	}

	val = syscall(__NR_mmap, addr, length, prot, flags, fd, offset);

	return (void *)val;
}

void *mremap(void *old_address, size_t old_size, size_t new_size, int flags)
{
	if (old_size >= __LONG_MAX__)
	{
		errno = ENOMEM;
		return MAP_FAILED;
	}
	return (void *)syscall(__NR_mremap, old_address, old_size, new_size, flags);
}

int munmap(void *addr, size_t length)
{
	int val = (int)syscall(__NR_munmap, addr, length);
	if (val < 0)
	{
		errno = -val;
		return -1;
	}
	return 0;
}
