#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum state_t
{
    available = 0,
    split = 1,
    full = 2,
};

struct buddy_allocator
{
    state_t state;
    size_t memory_size;
    void* ptr;
    struct buddy_allocator* left;
    struct buddy_allocator* right;
};

typedef struct buddy_allocator buddy_allocator_t;

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size);

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator);

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size);

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr);

int main()
{
    buddy_allocator_t* buddy = NULL;

    {
        size_t raw_memory_size = 1024;
        void* raw_memory = malloc(raw_memory_size);

        buddy = buddy_allocator_create(raw_memory, raw_memory_size);
    }

    {
        void* p = buddy_allocator_alloc(buddy, 1024);
        assert(p);
        buddy_allocator_free(buddy, p);
    }
    

    buddy_allocator_destroy(buddy);

}

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size)
{
    buddy_allocator_t* ret = (buddy_allocator_t*)malloc(sizeof(buddy_allocator_t));
    if (ret)
    {
        ret->ptr = raw_memory;
        ret->memory_size = raw_memory_size;
        ret->state = available;
        ret->left = NULL;
        ret->right = NULL;
    }
    return ret;
}

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator)
{
    if (buddy_allocator)
    {
        if ((split == buddy_allocator->state) || (full == buddy_allocator->state))
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
        free(buddy_allocator);
    }
}

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size)
{
    void* ret = NULL;
    if (buddy_allocator)
    {
        if (size == buddy_allocator->memory_size)
        {
            //buddy_allocator->allocated = 1;
            ret = buddy_allocator->ptr;
        }
        else if (size< buddy_allocator->memory_size)
        {

        }
        else
        {
            fprintf(stderr, "Not enough memory (requested %d, available %d)\n", size, buddy_allocator->memory_size);
        }
    }
    return ret;
}

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr)
{
}

