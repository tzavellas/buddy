#include <stdlib.h>
#include "buddy_allocator.h"

size_t log2(size_t x)
{
    size_t ret = 0;
    while (x > 1)
    {
        x >>= 1;
        ret++;
    }
    return ret;
}

void push(node_ptr* head, void* data)
{
    node_ptr nd = (node_ptr)malloc(sizeof(node_t));

    nd->data = data;
    nd->next = *head;
    *head = nd;
}

void* pop(node_ptr* head)
{
    void* ret = NULL;
    if (head)
    {
        node_ptr next = (*head)->next;
        ret = (*head)->data;
        free(*head);
        *head = next;
    }
    return ret;
}

void split(void* data, size_t size, void** first, void** second)
{
    if (data)
    {
        *first = data;
        *second = (void*)((char*)data + size / 2);
    }
}



buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size)
{
    buddy_allocator_t* ret = (buddy_allocator_t*)malloc(sizeof(buddy_allocator_t));
    if (ret)
    {
        ret->ptr = raw_memory;
        ret->memory_size = raw_memory_size;
        size_t heads_size= log2(raw_memory_size);
        ret->heads = (node_ptr*)malloc((heads_size + 1)* sizeof(node_ptr*));
        for (int i = 0; i <= heads_size; i++)
        {
            ret->heads[i] = NULL;
        }
        push(&ret->heads[heads_size], raw_memory);
    }
    return ret;
}

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator)
{
    if (buddy_allocator)
    {
        /*if ((split == buddy_allocator->state) || (full == buddy_allocator->state))
        {
            if (buddy_allocator->left)
                buddy_allocator_free(buddy_allocator->left, buddy_allocator->left->ptr);
            if (buddy_allocator->right)
                buddy_allocator_free(buddy_allocator->right, buddy_allocator->right->ptr);
        }
        else if (available == buddy_allocator->state)
        {
            free(buddy_allocator->ptr);
        }
        free(buddy_allocator);*/
    }
}

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size)
{
    void* ret = NULL;
    if (buddy_allocator)
    {
        size_t memory_size = buddy_allocator->memory_size;
        while (size < memory_size/2)
        {
            size_t level = log2(memory_size);
            if (!buddy_allocator->heads[level-1])
            {
                void* first = NULL;
                void* second = NULL;
                void* data = pop(&buddy_allocator->heads[level]);
                
                split(data, memory_size, &first, &second);
                push(&buddy_allocator->heads[level - 1], second);
                push(&buddy_allocator->heads[level - 1], first);
            }

        }
    }
    return ret;
}

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr)
{
}