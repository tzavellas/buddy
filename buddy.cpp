#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "buddy_allocator.h"
#include <time.h>

int main()
{
	buddy_allocator_t* buddy = NULL;

	{
		size_t raw_memory_size = 2048;
		void* raw_memory = malloc(raw_memory_size);
		buddy = buddy_allocator_create(raw_memory, raw_memory_size);
	}
	{
		//srand(time(NULL));
		/*int count = rand() % 20 + 1;*/
		size_t s[] = { 544, 135,430,915,648,430,604,861,745,395,775,730,311,144,870,949,514,97,156,83 };
		void** p = (void**)malloc(20);
		if (p)
		{
			int j = 0;
			for (int i = 0; i < 20; i++)
			{
				/*int s = rand() % 1024 + 32;*/
				p[j] = buddy_allocator_alloc(buddy, s[i]);
				printf("%d Requested %d - allocated %d\n", i, s[i], (p[j] != NULL));
				if (p[j])
				{
					j++;
				}
			}

			for (int i = j - 1; i > -1; i--)
			{
				buddy_allocator_free(buddy, p[i]);
				p[i] = NULL;
			}
		}
		free(p);

		/*void* p1 = buddy_allocator_alloc(buddy, 128);
		assert(p1);
		void* p2 = buddy_allocator_alloc(buddy, 30);
		assert(p2);
		void* p3 = buddy_allocator_alloc(buddy, 25);
		assert(p3);
		void* p4 = buddy_allocator_alloc(buddy, 70);
		assert(p4);
		buddy_allocator_free(buddy, p1);
		buddy_allocator_free(buddy, p2);
		buddy_allocator_free(buddy, p3);
		buddy_allocator_free(buddy, p4);*/
	}
	buddy_allocator_destroy(buddy);
	return 0;
}