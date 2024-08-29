/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef __MEM_LIST_H__
#define __MEM_LIST_H__ 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <internal/types.h>

	struct mem_list_m
	{
		void *start;
		size_t len;
		struct mem_list_m *prev;
		struct mem_list_m *next;
	};

#define ALLOC_UNIT 12288
#define MIN_DEALLOC 4096
#define BLOCK_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(struct mem_list_m)))
#define BLOCK_HEADER(ptr) ((void *)((unsigned long)ptr - sizeof(struct mem_lis_mt)))

	extern struct mem_list_m mem_list_head;
	extern struct mem_list_m *mem_head;

	void mem_list_init(void);
	int mem_list_add(void *start, size_t len);
	struct mem_list_m *mem_list_find(void *start);
	int mem_list_del(void *start);
	void mem_list_cleanup(void);
	size_t mem_list_num_items(void);
	void mem_add(struct mem_list_m *);
	void mem_remove(struct mem_list_m *);
	struct mem_list_m *mem_split(struct mem_list_m *, size_t);

#ifdef __cplusplus
}
#endif

#endif
