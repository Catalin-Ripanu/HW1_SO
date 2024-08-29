// SPDX-License-Identifier: BSD-3-Clause

#include <sys/stat.h>
#include <internal/types.h>
#include <unistd.h>
#include <internal/syscall.h>
#include <fcntl.h>
#include <errno.h>
#define STATX_ALL 0x7ff
#define makedev(x, y) (           \
	(((x)&0xfffff000ULL) << 32) | \
	(((x)&0x00000fffULL) << 8) |  \
	(((y)&0xffffff00ULL) << 12) | \
	(((y)&0x000000ffULL)))

/* Structure describing file characteristics as defined in linux/stat.h */
struct statx
{
	uint32_t stx_mask;
	uint32_t stx_blksize;
	uint64_t stx_attributes;
	uint32_t stx_nlink;
	uint32_t stx_uid;
	uint32_t stx_gid;
	uint16_t stx_mode;
	uint16_t pad1;
	uint64_t stx_ino;
	uint64_t stx_size;
	uint64_t stx_blocks;
	uint64_t stx_attributes_mask;
	struct
	{
		int64_t tv_sec;
		uint32_t tv_nsec;
		int32_t pad;
	} stx_atime, stx_btime, stx_ctime, stx_mtime;
	uint32_t stx_rdev_major;
	uint32_t stx_rdev_minor;
	uint32_t stx_dev_major;
	uint32_t stx_dev_minor;
	uint64_t spare[14];
};

int fstatat_statx(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	struct statx aux;
	int var = (int)syscall(__NR_statx, fd, path, flag, STATX_ALL, &aux);
	if (var < 0)
	{
		errno = -var;
		return -1;
	}

	st->st_dev = makedev(aux.stx_dev_major, aux.stx_dev_minor);
	st->st_rdev = makedev(aux.stx_rdev_major, aux.stx_rdev_minor);
	st->st_ino = aux.stx_ino;
	st->st_size = aux.stx_size;
	st->st_blksize = aux.stx_blksize;
	st->st_blocks = aux.stx_blocks;
	st->st_atime = aux.stx_atime.tv_sec;
	st->st_uid = aux.stx_uid;
	st->st_gid = aux.stx_gid;
	st->st_mtime = aux.stx_mtime.tv_sec;
	st->st_atime_nsec = aux.stx_atime.tv_nsec;
	st->st_mode = aux.stx_mode;
	st->st_nlink = aux.stx_nlink;
	st->st_mtime_nsec = aux.stx_mtime.tv_nsec;
	st->st_ctime = aux.stx_ctime.tv_sec;
	st->st_ctime_nsec = aux.stx_ctime.tv_nsec;

	return var;
}

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	int var1 = (int)syscall(__NR_newfstatat, fd, path, st, flag);
	if (var1 < 0)
	{
		errno = -var1;
		return -1;
	}
	return 0;
}
