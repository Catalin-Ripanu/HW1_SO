// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <internal/syscall.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <internal/mm/helper.h>
#include <errno.h>

struct zone list_blocks = {NULL, 1};

void *alloc_mem(size_t size)
{
	void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	return mem == MAP_FAILED ? NULL : mem;
}

static void *alloc_mem_block(size_t size)
{
	return alloc_block(size + sizeof(mem_list));
}

void *malloc(size_t size)
{
	if (!size)
		return NULL;

	void *mem = NULL;
	mem = alloc_mem_block(size);
	return mem;
}

void *calloc(size_t nmemb, size_t size)
{
	void *var = malloc(nmemb * size);
	if (!var)
		return NULL;
	memset(var, 0, nmemb * size);
	return var;
}

static void free_mem_block(void *ptr)
{
	free_block(ptr);
}

void free(void *ptr)
{
	if (!ptr)
		return;
	free_mem_block(ptr);
}

void *realloc(void *ptr, size_t size)
{
	if (!ptr || !size)
		return NULL;

	mem_list *block = NULL;
	block = (mem_list *)search_zone(&list_blocks, ptr);
	return move_block(block, size);
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	if (nmemb > 0 && __SIZE_MAX__ / nmemb < size)
		return NULL;
	return realloc(ptr, nmemb * size);
}
