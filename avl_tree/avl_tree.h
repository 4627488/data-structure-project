#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
} AVLTree;

AVLTree *create_avl_tree();
void insert_avl_tree(AVLTree *tree, int key);
void delete_avl_tree(AVLTree *tree, int key);
int search_avl_tree(AVLTree *tree, int key);
void free_avl_tree(AVLTree *tree);

#endif // AVL_TREE_H
