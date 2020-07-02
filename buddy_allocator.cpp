#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buddy_allocator.h"

size_t log2(size_t x)
{
	return (size_t)ceil(log(x) / log(2));
}

void associate_buddies(node_ptr first, node_ptr second)
{
	first->buddy = second;
	second->buddy = first;
}

node_ptr get_nonfull_node(node_ptr head)
{
	node_ptr ret = NULL;
	while (head)
	{
		if (Full != head->state)
		{
			ret = head;
			break;
		}
		head = head->next;
	}
	return ret;
}

node_ptr create_node(node_ptr parent, void* data)
{
	node_ptr ret = (node_ptr)malloc(sizeof(node_t));
	if (ret)
	{
		ret->buddy = NULL;
		ret->data = data;
		ret->parent = parent;
		ret->state = Free;
	}
	return ret;
}

void push(node_ptr* head, node_ptr n)
{
	n->next = *head;
	*head = n;
}

void pop(node_ptr* head)
{
	if (head)
	{
		node_ptr next = (*head)->next;
		free(*head);
		*head = next;
	}
}


//node_ptr find_free(node_ptr head)
//{
//	node_ptr ret = head;
//
//	while (ret)
//	{
//		if (ret->state == Free)
//		{
//			break;
//		}
//		ret = ret->next;
//	}
//	return ret;
//}

node_ptr find_data(node_ptr head, void* data)
{
	node_ptr ret = NULL;
	if (data)
	{
		node_ptr p = head;
		while (p)
		{
			if (p->data == data)
			{
				ret = p;
				break;
			}
			p = p->next;
		}
	}
	return ret;
}

void remove_node(node_ptr* head, node_ptr n)
{
	if (n)
	{
		if (*head == n)
		{
			pop(head);
		}
		else
		{
			node_ptr p = *head;
			while (p)
			{
				node_ptr temp = p->next;
				if (temp == n)
				{
					p->next = temp->next;
					free(temp);
					break;
				}
				p = temp;
			}
		}
	}
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
		ret->heads_size = log2(raw_memory_size);	// init linked list with pointers to memory blocks
		ret->heads = (node_ptr*)malloc((ret->heads_size + 1) * sizeof(node_ptr*));
		for (size_t i = 0; i <= ret->heads_size; i++)
		{
			ret->heads[i] = NULL;
		}
		node_ptr n = create_node(NULL, raw_memory);
		push(&ret->heads[ret->heads_size], n);	// add raw memory block to last position
	}
	return ret;
}

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator)
{
	if (buddy_allocator)
	{
		for (size_t i = 0; i <= buddy_allocator->heads_size; i++)
		{
			while (buddy_allocator->heads[i])	// Clear i-th linked list 
			{
				pop(&buddy_allocator->heads[i]);
			}
		}
		free(buddy_allocator->heads);	// release linked list
		free(buddy_allocator->ptr);		// release raw memory block
		free(buddy_allocator);			// release buddy allocator
	}
}

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size)
{
	void* ret = NULL;
	if (buddy_allocator)
	{
		size_t memory_size = buddy_allocator->memory_size;
		if (size > memory_size)
		{
			// not enought memory, nothing to do
		}
		else if (size > memory_size / 2)
		{
			size_t top_level = log2(size);
			node_ptr p = get_nonfull_node(buddy_allocator->heads[top_level]);
			if (p)
			{
				p->state = Taken;	// allocate
				ret = p->data;
			}
			else
			{
				// not enough memory, nothing to do
			}
		}
		else
		{
			size_t level = 0;
			node_ptr p_level = NULL;
			while (size <= memory_size / 2)
			{
				memory_size >>= 1;
				level = log2(memory_size);
				p_level = get_nonfull_node(buddy_allocator->heads[level]);
				if (!p_level)	// if level has no available blocks
				{
					node_ptr parent = get_nonfull_node(buddy_allocator->heads[level + 1]); // get a block from the previous level
					if (!parent)	// if there are no available blocks
					{
						// not enough memory, nothing to do
						break;
					}
					void* first_block = NULL;
					void* second_block = NULL;
					parent->state = Taken;		// mark block as Taken

					split(parent->data, memory_size, &first_block, &second_block);	// split the block

					node_ptr first = create_node(parent, first_block);		// create list nodes with common parent
					node_ptr second = create_node(parent, second_block);
					associate_buddies(first, second);						// associate them as buddies

					push(&buddy_allocator->heads[level], second);	// add the two new blocks in the current level
					push(&buddy_allocator->heads[level], first);

					p_level = buddy_allocator->heads[level];
				}
			}
			if (p_level)
			{
				p_level->state = Full;
				ret = p_level->data;
				if (Full == p_level->buddy->state)
				{
					p_level->parent->state = Full;
				}
			}
		}
	}
	return ret;
}

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr)
{
	if (buddy_allocator && ptr)
	{
		node_ptr found = NULL;
		size_t level = 0;
		while (level < buddy_allocator->heads_size)	// search all levels for memory block ptr
		{
			found = find_data(buddy_allocator->heads[level], ptr);	// if block exists in level
			if (found)
			{
				if (Free == found->buddy->state)	// Check if his buddy is free
				{
					found->parent->state = Free;
					ptr = found->parent->data;
					remove_node(&buddy_allocator->heads[level], found->buddy);
					remove_node(&buddy_allocator->heads[level], found);
				}
				else	// if buddy is not free, set this as free and break
				{
					found->state = Free;
					break;
				}
			}
			level++;
		}
	}
}