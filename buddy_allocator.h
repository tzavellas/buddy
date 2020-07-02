#pragma once

typedef struct node
{
    void* data;
    struct node* next;
}node_t;

typedef node_t* node_ptr;

void push(node_ptr* head, void* data);

void* pop(node_ptr* head);

void split(void* data, size_t size, void** first, void** second);

typedef struct buddy_allocator
{
    size_t memory_size;
    void* ptr;
    node_ptr* heads;
}buddy_allocator_t;

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size);

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator);

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size);

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr);