// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <sys/mman.h>

struct mem_list_m mem_list_head;
struct mem_list_m *mem_head = NULL;

void mem_list_init(void)
{
	mem_list_head.start = NULL;
	mem_list_head.len = 0;
	mem_list_head.prev = &mem_list_head;
	mem_list_head.next = &mem_list_head;
}

static struct mem_list_m *mem_list_alloc(void)
{
	return mmap(NULL, sizeof(struct mem_list_m), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static int mem_list_free(struct mem_list_m *item)
{
	return munmap(item, sizeof(struct mem_list_m));
}

int mem_list_add(void *start, size_t len)
{
	struct mem_list_m *item;

	item = mem_list_alloc();
	if (item == NULL)
		return -1;

	item->start = start;
	item->len = len;

	/* Add item in list. */
	item->next = &mem_list_head;
	item->prev = mem_list_head.prev;
	mem_list_head.prev->next = item;
	mem_list_head.prev = item;

	return 0;
}

struct mem_list_m *mem_list_find(void *start)
{
	struct mem_list_m *iter;

	for (iter = mem_list_head.next; iter != &mem_list_head; iter = iter->next)
		if (iter->start == start)
			return iter;

	return NULL;
}

static struct mem_list_m *mem_list_extract(void *start)
{
	struct mem_list_m *item;

	item = mem_list_find(start);
	if (item == NULL)
		return NULL;

	/* Extract item from doubly-linked list. */
	item->next->prev = item->prev;
	item->prev->next = item->next;
	item->next = item;
	item->prev = item;

	return item;
}

int mem_list_del(void *start)
{
	struct mem_list_m *item;

	item = mem_list_extract(start);
	if (item == NULL)
		return -1;

	mem_list_free(item);

	return 0;
}

void mem_list_cleanup(void)
{
	struct mem_list_m *iter, *tmp;

	for (iter = mem_list_head.next, tmp = iter->next;
		 iter != &mem_list_head;
		 iter = tmp, tmp = iter->next)
	{
		/* Extract item from doubly-linked list. */
		iter->next->prev = iter->prev;
		iter->prev->next = iter->next;
		iter->next = iter;
		iter->prev = iter;

		/* Free item. */
		mem_list_free(iter);
	}
}

size_t mem_list_num_items(void)
{
	struct mem_list_m *iter;
	size_t count = 0;

	for (iter = mem_list_head.next; iter != &mem_list_head; iter = iter->next)
		count++;

	return count;
}

void mem_add(struct mem_list_m *block)
{
	block->prev = NULL;
	block->next = NULL;
	if (!mem_head || (unsigned long)mem_head > (unsigned long)block)
	{
		if (mem_head)
		{
			mem_head = block;
		}
		block->next = mem_head;
		mem_head = block;
	}
	else
	{
		struct mem_list_m *ptr = mem_head;
		while (ptr->next && (unsigned long)ptr->next < (unsigned long)block)
		{
			ptr = ptr->next;
		}
		block->next = ptr->next;
		ptr->next = block;
	}
}

void mem_remove(struct mem_list_m *block)
{
	if (!block->prev)
	{
		if (block->next)
		{
			mem_head = block->next;
			munmap(block, sizeof(struct mem_list_m));
		}
		else
		{
			mem_head = NULL;
		}
	}
	else
	{
		block->prev->next = block->next;
		if (block->next)
		{
			block->next->prev = block->prev;
			munmap(block, sizeof(struct mem_list_m));
		}
		else
		{
			block->prev->next = NULL;
			munmap(block, sizeof(struct mem_list_m));
		}
	}
}

struct mem_list_m *mem_split(struct mem_list_m *block, size_t size)
{
	void *mem_block = BLOCK_MEM(block);
	struct mem_list_m *newptr = (struct mem_list_m *)((unsigned long)mem_block + size);
	newptr->len = block->len - (size + sizeof(struct mem_list_m));
	block->len = size;
	return newptr;
}