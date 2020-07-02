#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "buddy_allocator.h"

int main()
{
    buddy_allocator_t* buddy = NULL;

    {
        size_t raw_memory_size = 256;
        void* raw_memory = malloc(raw_memory_size);
        buddy = buddy_allocator_create(raw_memory, raw_memory_size);
    }
    {
        void* p2 = buddy_allocator_alloc(buddy, 30);
        assert(p2);
        void* p1 = buddy_allocator_alloc(buddy, 128);
        assert(p1);
        void* p3 = buddy_allocator_alloc(buddy, 128);
        assert(!p3);
        //buddy_allocator_free(buddy, p1);
    }
    buddy_allocator_destroy(buddy);
    return 0;
}