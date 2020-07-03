#include <math.h>
#include <stdlib.h>
#include "Helper.h"

treenode_ptr create_leaf(void* data, size_t block_size)
{
	treenode_ptr ret = (treenode_ptr)malloc(sizeof(treenode_t));
	if (ret)
	{
		ret->data = data;
		ret->block_size = block_size;
		ret->state = Available;
		ret->left = NULL;
		ret->right = NULL;
	}
	return ret;
}

void destroy_tree(treenode_ptr leaf)
{
	if (leaf)
	{
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		free(leaf);
	}
}

size_t log2(size_t x)
{
	return (size_t)ceil(log(x) / log(2));
}

treenode_ptr search(treenode_ptr leaf, void* data)
{
	treenode_ptr ret = NULL;
	if (leaf)
	{
		if ((Full == leaf->state) && (data == leaf->data))
		{
			ret = leaf;
		}
		else
		{
			ret = search(leaf->left, data);
			if (!ret)
				ret = search(leaf->right, data);
		}
	}
	return ret;
}

int split(treenode_ptr* leaf, void* data, size_t block_size)
{
	int ret = 0;
	if (*leaf)
	{
		treenode_ptr left, right;

		left = create_leaf(data, block_size);
		right = create_leaf((void*)((char*)data + block_size), block_size);

		if (left && right)
		{
			(*leaf)->left = left;
			(*leaf)->right = right;
			ret = 1;
		}
	}
	return ret;
}

state_t update_leaf(treenode_ptr* leaf)
{
	state_t ret = Available;
	if (*leaf)
	{
		if (!(*leaf)->left && !(*leaf)->right)
		{
			ret = (*leaf)->state;
		}
		else if ((*leaf)->left && (*leaf)->right)
		{
			state_t left = update_leaf(&(*leaf)->left);
			state_t right = update_leaf(&(*leaf)->right);
			if (Full == left && Full == right)
			{
				(*leaf)->state = Full;
				ret = Full;
			}
			else if (Available == left && Available == right)
			{
				free((*leaf)->left);
				(*leaf)->left = NULL;
				free((*leaf)->right);
				(*leaf)->right = NULL;
				(*leaf)->state = Available;
				ret = Available;
			}
			else
			{
				(*leaf)->state = Allocated;
				ret = Allocated;
			}
		}
		else
		{
			// not valid case, leaves come in pairs
		}
	}
	return ret;
}

size_t count_allocated(treenode_ptr leaf)
{
	size_t ret = 0;
	if (leaf)
	{
		if (!leaf->left && !leaf->right)
		{
			ret = (Full == leaf->state) ? leaf->block_size : 0;
		}
		else
		{
			ret = count_allocated(leaf->left);
			ret += count_allocated(leaf->right);
		}
	}
	return ret;
}