#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>

// 获取节点的高度
int height(AVLNode *node)
{
    return node ? node->height : 0;
}

// 创建新节点
AVLNode *create_node(int key)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1; // 新节点初始化为叶子节点
    return node;
}

// 右旋转
AVLNode *right_rotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    // 更新高度
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// 左旋转
AVLNode *left_rotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// 计算节点的平衡因子
int get_balance(AVLNode *node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

// 插入新节点
AVLNode *insert_node(AVLNode *node, int key)
{
    if (!node) return create_node(key);

    if (key < node->key)
        node->left = insert_node(node->left, key);
    else if (key > node->key)
        node->right = insert_node(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // 左左情况
    if (balance > 1 && key < node->left->key) return right_rotate(node);

    // 右右情况
    if (balance < -1 && key > node->right->key) return left_rotate(node);

    // 左右情况
    if (balance > 1 && key > node->left->key)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // 右左情况
    if (balance < -1 && key < node->right->key)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// 查找最小值节点
AVLNode *min_value_node(AVLNode *node)
{
    AVLNode *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// 删除节点
AVLNode *delete_node(AVLNode *root, int key)
{
    if (!root) return root;

    if (key < root->key)
        root->left = delete_node(root->left, key);
    else if (key > root->key)
        root->right = delete_node(root->right, key);
    else
    {
        // 节点只有一个子节点或没有子节点
        if ((root->left == NULL) || (root->right == NULL))
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (!temp)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            // 有两个子节点的情况
            AVLNode *temp = min_value_node(root->right);
            root->key = temp->key;
            root->right = delete_node(root->right, temp->key);
        }
    }

    if (!root) return root;

    // 更新高度
    root->height = 1 + max(height(root->left), height(root->right));

    int balance = get_balance(root);

    // 重新平衡树
    if (balance > 1 && get_balance(root->left) >= 0) return right_rotate(root);

    if (balance > 1 && get_balance(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) return left_rotate(root);

    if (balance < -1 && get_balance(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

// 查找节点
int search_node(AVLNode *root, int key)
{
    if (!root) return 0;
    if (key == root->key) return 1;
    if (key < root->key) return search_node(root->left, key);
    return search_node(root->right, key);
}

// 创建 AVL 树
AVLTree *create_avl_tree()
{
    AVLTree *tree = (AVLTree *)malloc(sizeof(AVLTree));
    tree->root = NULL;
    return tree;
}

// 插入 AVL 树节点
void insert_avl_tree(AVLTree *tree, int key)
{
    tree->root = insert_node(tree->root, key);
}

// 删除 AVL 树节点
void delete_avl_tree(AVLTree *tree, int key)
{
    tree->root = delete_node(tree->root, key);
}

// 搜索 AVL 树节点
int search_avl_tree(AVLTree *tree, int key)
{
    return search_node(tree->root, key);
}

// 递归释放节点
void free_node(AVLNode *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

// 释放 AVL 树
void free_avl_tree(AVLTree *tree)
{
    if (tree)
    {
        free_node(tree->root);
        free(tree);
    }
}