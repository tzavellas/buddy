#pragma once
enum state_t
{
    Available = 0,
    Allocated,
    Full
};

/// <summary>
/// Binary search tree leaf struct
/// </summary>
typedef struct leaf
{
    void* data;

    size_t block_size;
    state_t state;

    struct leaf* left;
    struct leaf* right;
}leaf_t;

typedef leaf_t* leaf_ptr;

/// <summary>
/// Creates a new leaf for a binary tree. Leaf is not attached to any tree, rather a pointer is returned
/// </summary>
/// <param name="data"> Raw memory block start address </param>
/// <param name="block_size"> Raw memory block size </param>
/// <returns> pointer to the new leaf </returns>
leaf_ptr create_leaf(void* data, size_t block_size);

/// <summary>
/// Destroys a binary sub-tree, postorder traversal
/// </summary>
/// <param name="leaf"> The sub-tree root </param>
void destroy_tree(leaf_ptr leaf);

/// <summary>
/// Calculates the log base 2 of an integer
/// </summary>
/// <param name="x"> The integer </param>
/// <returns> the rounded-up log base 2 of the input </returns>
size_t log2(size_t x);

/// <summary>
/// Searches the binary sub-tree for a leaf that contains the data and its state is Full
/// </summary>
/// <param name="leaf"> The sub-tree root </param>
/// <param name="data"> The pointer to data </param>
/// <returns> a pointer to the leaf that contains the data or NULL if not found </returns>
leaf_ptr search(leaf_ptr leaf, void* data);

/// <summary>
/// Given a leaf with data, it splits the given data into 2 blocks of block_size and assigns them to two new child leaves
/// </summary>
/// <param name="leaf"> The leaf </param>
/// <param name="data"> The data to be split </param>
/// <param name="block_size"> The block_size of each new child leaf. Typically, it should be half the data block size </param>
/// <returns> 1 if successful, 0 othewise </returns>
int split(leaf_ptr* leaf, void* data, size_t block_size);

/// <summary>
/// Updates a sub-tree state, postorder traversal
/// </summary>
/// <param name="leaf"> The sub-tree root </param>
/// <returns> the new state of the sub-tree root </returns>
state_t update_leaf(leaf_ptr* leaf);

/// <summary>
/// Counts the accumulated block size of a sub-tree. Used for measuring internal fragmentation
/// </summary>
/// <param name="leaf"> The sub-tree root </param>
/// <returns> Number of bytes used </returns>
size_t count_allocated(leaf_ptr leaf);