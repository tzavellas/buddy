#pragma once
enum state_t
{
    Available = 0,
    Allocated,
    Full
};

typedef struct treenode
{
    void* data;

    size_t block_size;
    state_t state;

    struct treenode* left;
    struct treenode* right;
}treenode_t;

typedef treenode_t* treenode_ptr;

treenode_ptr create_leaf(void* data, size_t block_size);

void destroy_tree(treenode_ptr leaf);

size_t log2(size_t x);

treenode_ptr search(treenode_ptr leaf, void* data);

int split(treenode_ptr* leaf, void* data, size_t block_size);

state_t update_leaf(treenode_ptr* leaf);

size_t count_allocated(treenode_ptr leaf);