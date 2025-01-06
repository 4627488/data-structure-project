#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>

// 声明函数，实现在下面
void deleteNode(BTreeNode *node, int key);

// 创建一个新节点，isLeaf表示节点是否是叶子节点
BTreeNode *createNode(int isLeaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    for (int i = 0; i < M; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 创建一棵B树，初始化根节点为NULL
BTree *createBTree() {
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

// 分裂子节点，将子节点分裂成两个，并更新父节点
void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
    BTreeNode *newChild = createNode(child->isLeaf);
    newChild->numKeys = M / 2 - 1;

    // 复制子节点后半部分的keys到新节点
    for (int j = 0; j < M / 2 - 1; j++) {
        newChild->keys[j] = child->keys[j + M / 2];
    }

    // 如果不是叶子节点，复制子节点的children
    if (!child->isLeaf) {
        for (int j = 0; j < M / 2; j++) {
            newChild->children[j] = child->children[j + M / 2];
        }
    }

    child->numKeys = M / 2 - 1;

    // 将父节点的children移动一位以给新子节点腾出空间
    for (int j = parent->numKeys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = newChild;

    // 将父节点的keys向后移动，插入分裂后的关键字
    for (int j = parent->numKeys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[M / 2 - 1];
    parent->numKeys++;
}

// 插入一个新关键字到非满节点
void insertNonFull(BTreeNode *node, int key) {
    int i = node->numKeys - 1;

    if (node->isLeaf) {
        // 在叶子节点插入key
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        // 在非叶子节点插入key
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == M - 1) {
            // 如果子节点满了，先分裂子节点
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// 插入一个新关键字到B树
void insert(BTree *tree, int key) {
    if (tree->root == NULL) {
        // 如果树为空，创建一个新的根节点
        tree->root = createNode(1);
        tree->root->keys[0] = key;
        tree->root->numKeys = 1;
    } else {
        // 如果根节点已满，分裂根节点
        if (tree->root->numKeys == M - 1) {
            BTreeNode *newRoot = createNode(0);
            newRoot->children[0] = tree->root;
            splitChild(newRoot, 0, tree->root);
            int i = 0;
            if (newRoot->keys[0] < key) {
                i++;
            }
            insertNonFull(newRoot->children[i], key);
            tree->root = newRoot;
        } else {
            insertNonFull(tree->root, key);
        }
    }
}

// 合并相邻的兄弟节点
void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    // 将父节点的key移到合并后的子节点中
    child->keys[M / 2 - 1] = node->keys[idx];

    // 将兄弟节点的key移到子节点中
    for (int i = 0; i < sibling->numKeys; ++i) {
        child->keys[i + M / 2] = sibling->keys[i];
    }

    // 如果不是叶子节点，合并兄弟节点的children
    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->numKeys; ++i) {
            child->children[i + M / 2] = sibling->children[i];
        }
    }
    for (int i = idx + 1; i < node->numKeys; ++i) {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = idx + 2; i <= node->numKeys; ++i) {
        node->children[i - 1] = node->children[i];
    }

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    // 删除兄弟节点
    free(sibling);
}

// 在父节点中填充缺少的子节点
void fill(BTreeNode *node, int idx) {
    // 向左兄弟借节点
    if (idx != 0 && node->children[idx - 1]->numKeys >= M / 2) {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx - 1];

        for (int i = child->numKeys - 1; i >= 0; --i) {
            child->keys[i + 1] = child->keys[i];
        }

        if (!child->isLeaf) {
            for (int i = child->numKeys; i >= 0; --i) {
                child->children[i + 1] = child->children[i];
            }
        }

        child->keys[0] = node->keys[idx - 1];

        if (!child->isLeaf) {
            child->children[0] = sibling->children[sibling->numKeys];
        }

        node->keys[idx - 1] = sibling->keys[sibling->numKeys - 1];

        child->numKeys += 1;
        sibling->numKeys -= 1;
    }
    // 向右兄弟借节点
    else if (idx != node->numKeys &&
             node->children[idx + 1]->numKeys >= M / 2) {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx + 1];

        child->keys[child->numKeys] = node->keys[idx];

        if (!child->isLeaf) {
            child->children[child->numKeys + 1] = sibling->children[0];
        }

        node->keys[idx] = sibling->keys[0];

        for (int i = 1; i < sibling->numKeys; ++i) {
            sibling->keys[i - 1] = sibling->keys[i];
        }

        if (!sibling->isLeaf) {
            for (int i = 1; i <= sibling->numKeys; ++i) {
                sibling->children[i - 1] = sibling->children[i];
            }
        }

        child->numKeys += 1;
        sibling->numKeys -= 1;
    }
    // 如果兄弟节点不能借，则合并
    else {
        if (idx != node->numKeys) {
            merge(node, idx);
        } else {
            merge(node, idx - 1);
        }
    }
}

// 从叶子节点删除一个关键字
void removeFromLeaf(BTreeNode *node, int idx) {
    for (int i = idx + 1; i < node->numKeys; ++i) {
        node->keys[i - 1] = node->keys[i];
    }
    node->numKeys--;
}

// 从非叶子节点删除一个关键字
void removeFromNonLeaf(BTreeNode *node, int idx) {
    int key = node->keys[idx];

    if (node->children[idx]->numKeys >= M / 2) {
        // 使用前驱节点替代当前节点
        BTreeNode *cur = node->children[idx];
        while (!cur->isLeaf) {
            cur = cur->children[cur->numKeys];
        }
        int pred = cur->keys[cur->numKeys - 1];
        node->keys[idx] = pred;
        deleteNode(node->children[idx], pred);
    }
    // 使用后继节点替代当前节点
    else if (node->children[idx + 1]->numKeys >= M / 2) {
        BTreeNode *cur = node->children[idx + 1];
        while (!cur->isLeaf) {
            cur = cur->children[0];
        }
        int succ = cur->keys[0];
        node->keys[idx] = succ;
        deleteNode(node->children[idx + 1], succ);
    }
    // 合并当前节点和子节点
    else {
        merge(node, idx);
        deleteNode(node->children[idx], key);
    }
}

// 删除一个节点
void deleteNode(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->numKeys && node->keys[idx] < key) {
        ++idx;
    }

    if (idx < node->numKeys && node->keys[idx] == key) {
        if (node->isLeaf) {
            removeFromLeaf(node, idx);
        } else {
            removeFromNonLeaf(node, idx);
        }
    } else {
        if (node->isLeaf) {
            return;
        }

        int flag = (idx == node->numKeys);

        if (node->children[idx]->numKeys < M / 2) {
            fill(node, idx);
        }

        if (flag && idx > node->numKeys) {
            deleteNode(node->children[idx - 1], key);
        } else {
            deleteNode(node->children[idx], key);
        }
    }
}

// 删除B树中的一个关键字
void removeKey(BTree *tree, int key) {
    if (!tree->root) {
        return;
    }

    deleteNode(tree->root, key);

    if (tree->root->numKeys == 0) {
        BTreeNode *tmp = tree->root;
        if (tree->root->isLeaf) {
            tree->root = NULL;
        } else {
            tree->root = tree->root->children[0];
        }
        free(tmp);
    }
}

// 在节点中查找一个关键字
int searchNode(BTreeNode *node, int key) {
    int i = 0;
    while (i < node->numKeys && key > node->keys[i]) {
        i++;
    }
    if (i < node->numKeys && key == node->keys[i]) {
        return 1;
    }
    if (node->isLeaf) {
        return 0;
    }
    return searchNode(node->children[i], key);
}

// 在B树中查找一个关键字
int search(BTree *tree, int key) {
    if (tree->root == NULL) {
        return 0;
    }
    return searchNode(tree->root, key);
}

// 释放节点内存
void freeNode(BTreeNode *node) {
    if (!node->isLeaf) {
        for (int i = 0; i <= node->numKeys; i++) {
            freeNode(node->children[i]);
        }
    }
    free(node);
}

// 释放B树内存
void freeBTree(BTree *tree) {
    if (tree->root != NULL) {
        freeNode(tree->root);
    }
    free(tree);
}
