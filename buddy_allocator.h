#pragma once

enum state_t
{
    Free = 0,
    Taken,
    Full
};

typedef struct node
{
    void* data;
    struct node* next;
    struct node* buddy;
    struct node* parent;
    state_t state;
}node_t;

typedef node_t* node_ptr;

typedef struct buddy_allocator
{
    size_t memory_size;
    void* ptr;
    node_ptr* heads;
    size_t heads_size;
}buddy_allocator_t;

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size);

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator);

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size);

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr);