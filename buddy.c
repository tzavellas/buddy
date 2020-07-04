#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "buddy_allocator.h"

int main()
{
	buddy_allocator_t* buddy = NULL;

	{
		size_t raw_memory_size = 16384;
		void* raw_memory = malloc(raw_memory_size);
		buddy = buddy_allocator_create(raw_memory, raw_memory_size);
	}
	{
		srand((unsigned int)time(NULL));
		int count = rand() % 20 + 1;
		void** p = (void**)malloc(sizeof(void*)*20);
		if (p)
		{
			size_t total = 0;
			for (int i = 0; i < 20; i++)
			{
				size_t s = (size_t)(rand() % 512) + 32;
				p[i] = buddy_allocator_alloc(buddy, s);
				if (p[i])
					total += s;
				printf("%d Requested %zd - allocated %d\n", i, s, (p[i] != NULL));
			}
			printf("Sum request allocated: %zd\n", total);
			printf("Sum actually allocated: %zd\n", count_allocated(buddy->root));
		}
		
		buddy_allocator_destroy(buddy);
		free(p);
	}
	return 0;
}