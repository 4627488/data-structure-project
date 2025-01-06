#ifndef B_TREE_H
#define B_TREE_H

#define M 4 // B-tree order

typedef struct BTreeNode {
    int keys[M - 1];
    struct BTreeNode *children[M];
    int numKeys;
    int isLeaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode *root;
} BTree;

BTree *createBTree();
void insert(BTree *tree, int key);
void removeKey(BTree *tree, int key);
int search(BTree *tree, int key);
void freeBTree(BTree *tree);

#endif // B_TREE_H
