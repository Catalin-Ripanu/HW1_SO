#include <internal/mm/helper.h>

int func_align(size_t x, int y)
{
    return (x + y - 1) & ~(y - 1);
}

int func_align_pag(size_t x)
{
    if (x)
        return func_align(x, PAGE_SIZE);
    else
        return PAGE_SIZE;
}

void *func_head(struct zone *zone, int idx)
{
    if (zone->blocks)
        return ((void *)((char *)(zone)->blocks) + (0 * idx));
    else
        return NULL;
}

void *func_mem(mem_list *block)
{
    if (block)
        return ((void *)((char *)(block) + sizeof(mem_list)));
    else
        return NULL;
}

size_t func_data(mem_list *block)
{
    if (block)
        return ((block)->size - sizeof(mem_list));
    else
        return 0;
}

int decision(mem_list *block, int num)
{
    if (block)
        return block->flags & num;
    else
        return 0;
}

static mem_list *merge_block(mem_list *block)
{
    mem_list *p = block->prev;
    mem_list *n = block->next;

    if (!decision(n, IS_INUSE) && !(n->flags & IS_FIRST))
    {
        block->next = n->next;
        block->next->prev = block;
        block->flags |= n->flags;
        block->size = block->size + n->size;
    }

    if (!(p->flags & IS_LAST) && p != n && !decision(p, IS_INUSE))
    {
        p->next = block->next;
        block->next->prev = p;
        p->size = block->size + p->size;
        p->flags |= block->flags;
        block = p;
    }
    return block;
}

static void add_block(mem_list **head, mem_list *block)
{
    mem_list *tmp_head = *head;
    if (!tmp_head)
    {
        if (!block->next)
            block->next = block;
        if (!block->prev)
            block->prev = block;

        *head = block;
        return;
    }

    mem_list *btail = block->prev;
    mem_list *htail = tmp_head->prev;

    block->prev = htail;
    htail->next = block;

    btail->next = tmp_head;
    tmp_head->prev = btail;
}

static void split_block(mem_list *block, size_t size)
{
    mem_list *new_block = (void *)((char *)block + size);
    new_block->size = block->size - size;
    new_block->prev = block;
    new_block->next = block->next;
    new_block->flags |= block->flags;
    new_block->flags &= ~IS_FIRST;

    block->next->prev = new_block;
    block->next = new_block;
    block->size = size;
    block->flags &= ~IS_LAST;
}

void *move_block(mem_list *block, size_t size)
{
    if (!block || !block->flags)
        return NULL;

    void *src_ptr = func_mem(block);
    void *dst_ptr = src_ptr;
    if (func_data(block) < size)
    {
        dst_ptr = malloc(size);
        if (!dst_ptr)
            return NULL;
        size_t src_size = func_data(block);
        memcpy(dst_ptr, src_ptr, src_size);
        free(src_ptr);
    }
    return dst_ptr;
}

mem_list *find_block(struct zone *zone, size_t size)
{
    mem_list *head = zone->blocks;
    if (!head)
        return NULL;
    mem_list *block = head;
    do
    {
        if (!decision(block, IS_INUSE) && block->size >= size)
        {
            if (func_data(block) > size)
                split_block(block, size);
            block->flags |= IS_INUSE;
            return block;
        }
        block = block->next;
    } while (head != block);
    return NULL;
}

mem_list *new_block(struct zone *zone, size_t size)
{
    size_t pagesz = func_align_pag(size);
    mem_list *block = alloc_mem(pagesz);
    if (!block)
        return NULL;
    block->size = pagesz;
    block->prev = block;
    block->next = block;
    block->flags = IS_FIRST | IS_LAST;
    if (func_data(block) > size)
        split_block(block, size);
    block->flags |= IS_INUSE;
    add_block(&zone->blocks, block);
    return block;
}

void *alloc_block(size_t size)
{

    mem_list *block = find_block(&list_blocks, size);
    if (block)
        return func_mem(block);

    block = new_block(&list_blocks, size);
    if (block)
        return func_mem(block);
    return NULL;
}

static mem_list *search_using_ptr(mem_list *head, void *ptr)
{

    if (!head)
        return NULL;

    mem_list *block = head;
    do
    {

        if (func_mem(block) == ptr)
            return block;

        block = block->next;
    } while (block != head);
    return NULL;
}
static int remove_block(mem_list *block)
{
    if (!(decision(block, IS_LAST) && decision(block, IS_FIRST)))
        return 0;

    if (!block)
        return 0;

    block->prev->next = block->next;
    block->next->prev = block->prev;

    if (list_blocks.blocks == block)
    {
        if (block->next != block)
            list_blocks.blocks = block->next;
        else
            list_blocks.blocks = NULL;
    }
    return 1;
}

void free_block(void *ptr)
{
    mem_list *block = search_using_ptr(list_blocks.blocks, ptr);
    if (!block)
        return;
    if (block && !decision(block, IS_INUSE))
        return;

    block->flags &= ~IS_INUSE;
    block = merge_block(block);
    void *addr = block;
    size_t size = block->size;
    if (remove_block(block))
    {
        munmap(addr, size);
    }
}

mem_list *search_zone(struct zone *zone, void *ptr)
{
    if (!zone)
        return NULL;

    mem_list *head_mem = NULL;
    mem_list *block = NULL;
    for (size_t head = 0; head < zone->no_head; head++)
    {
        head_mem = (mem_list *)func_head(zone, head);
        block = search_using_ptr(head_mem, ptr);
        if (block)
            return block;
    }
    return NULL;
}