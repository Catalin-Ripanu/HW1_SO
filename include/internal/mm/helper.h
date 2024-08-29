#ifndef MALLOC
#define MALLOC

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#define MIN_SIZE (2 * sizeof(size_t))
#define PAGE_SIZE 4096
#define ZONE_NUM 3
#define IS_INUSE 0x1
#define IS_FIRST 0x2
#define IS_LAST 0x4

typedef struct mem_list
{
    struct mem_list *prev;
    size_t flags;
    size_t size;
    struct mem_list *next;
} mem_list;

struct zone
{
    mem_list *blocks;
    size_t no_head;
};

extern struct zone list_blocks;

void *alloc_mem(size_t size);
void *alloc_block(size_t size);
void free_block(void *ptr);
void *move_block(mem_list *block, size_t size);
mem_list *search_zone(struct zone *zone, void *ptr);

#endif