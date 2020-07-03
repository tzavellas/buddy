#pragma once
#include "Helper.h"

typedef struct buddy_allocator
{
    treenode_ptr root;
}buddy_allocator_t;

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size);

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator);

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size);

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr);