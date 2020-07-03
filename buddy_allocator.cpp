#include <stdlib.h>
#include "buddy_allocator.h"

static void* request_block(treenode_ptr leaf, size_t size, size_t memory_size)
{
	void* ret = NULL;
	
	size_t levelsize = log2(size);
	size_t level = log2(memory_size);
	while (levelsize < level)
	{
		memory_size >>= 1;
		level = log2(memory_size);

		if (Available == leaf->state)
		{
			if (split(&leaf, leaf->data, memory_size))
			{
				leaf->state = Allocated;
				leaf = leaf->left;
			}
			else
			{
				// error in splitting -- CHECK
				break;
			}
		}
		else if (Allocated == leaf->state)
		{
			state_t left_state = leaf->left->state;
			if (Full == left_state)
			{
				leaf = leaf->right;
			}
			else if ((Allocated == left_state) && (level == levelsize))
			{
				leaf = leaf->right;
			}
			else
			{
				leaf = leaf->left;
			}
		}
		else
		{
			// not enough memory, nothing to do
		}

	}
	if (Available == leaf->state)
	{
		ret = leaf->data;
		leaf->state = Full;		
	}
	else
	{
		// not enough memory, nothing to do
	}
	return ret;
}

buddy_allocator_t* buddy_allocator_create(void* raw_memory, size_t raw_memory_size)
{
	buddy_allocator_t* ret = (buddy_allocator_t*)malloc(sizeof(buddy_allocator_t));
	if (ret)
	{
		ret->root = NULL;
		ret->root = create_leaf(raw_memory, raw_memory_size);
	}
	return ret;
}

void buddy_allocator_destroy(buddy_allocator_t* buddy_allocator)
{
	if (buddy_allocator)
	{
		destroy_tree(buddy_allocator->root);
		free(buddy_allocator);			// release buddy allocator
	}
}

void* buddy_allocator_alloc(buddy_allocator_t* buddy_allocator, size_t size)
{
	void* ret = NULL;
	if (buddy_allocator)
	{
		treenode_ptr root = buddy_allocator->root;
		size_t memory_size = root->block_size;
		if (Full != root->state)
		{
			if (size > memory_size)
			{
				// not enough memory, nothing to do
			}
			else if (size > memory_size / 2)
			{
				if (Available == root->state)
				{
					ret = root->data;
					root->state = Full;
				}
				else
				{
					// not enough memory, nothing to do
				}
			}
			else
			{
				ret = request_block(root, size, memory_size);
				update_leaf(&root);
			}
		}
	}
	return ret;
}

void buddy_allocator_free(buddy_allocator_t* buddy_allocator, void* ptr)
{
	if (buddy_allocator && ptr)
	{
		treenode_ptr root = buddy_allocator->root;
		treenode_ptr found = search(root, ptr);
		if (found)
		{
			found->state = Available;
			update_leaf(&root);
		}
	}
}