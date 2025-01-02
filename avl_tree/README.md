# 4、 平衡二叉树编程

## 项目简介

该项目实现了一个平衡二叉树（AVL树），并使用该树完成以下任务：

1. 对于1-10000的质数序列<2，3，5，7，…，9973>，建立平衡二叉排序树。
2. 依次查询200-300的每个数是否在树中，将结果写入`tree1.txt`。
3. 依次删除500-2000中的每个质数，再查询600-700之间的每个质数是否在树中，将结果写入`tree2.txt`。
4. 在任务（2）的基础上，依次插入1-1000的所有偶数，依次查询100-200的每个偶数是否在树中，将结果写入`tree3.txt`。

## 文件结构

- `main.c`：主程序，包含质数生成、插入、删除和查询操作。
- `avl_tree.h`：AVL树的头文件，定义了树的结构和函数。
- `avl_tree.c`：AVL树的实现文件，包含树的插入、删除、查询等操作。

## 编译和运行

```bash
make
./main
```

## 数据结构

### 平衡二叉树节点结构 (AVLNode)

```c
typedef struct AVLNode {
    int key;                      // 节点存储的键值
    struct AVLNode *left;         // 指向左子树的指针
    struct AVLNode *right;        // 指向右子树的指针
    int height;                   // 当前节点的高度
} AVLNode;
```

每个节点存储一个键值 `key`，以及指向左、右子节点的指针。相较于普通的孩子表示法，额外维护节点的`height`，用于计算平衡因子。

### 平衡二叉树结构 (AVLTree)

```c
typedef struct AVLTree {
    AVLNode *root;                // 平衡二叉树的根节点
} AVLTree;
```

AVL树结构体只需要一个指向根节点的指针，即可表示整个树的状态。

## 主要算法

### 1. 插入节点 (Insert)

递归地插入节点，沿路径回溯时更新节点高度，并计算平衡因子。

根据平衡因子进行旋转操作，包括**单右旋**、**单左旋**、**左右旋**和**右左旋**，确保树保持平衡。

新节点插入时，节点高度增加最多为1，因此调整操作的复杂度较低。

```c
void insert_avl_tree(AVLTree *tree, int key) {
    tree->root = insert_node(tree->root, key);
}
```

### 2. 删除节点 (Delete)

- 递归删除节点，分三种情况处理：
  1. 叶子节点直接删除。
  2. 只有一个子节点的节点直接提升子节点。
  3. 有两个子节点时，找到右子树中最小的节点替代当前节点。
- 删除后同样通过旋转恢复平衡。

```c
void delete_avl_tree(AVLTree *tree, int key) {
    tree->root = delete_node(tree->root, key);
}
```

### 3. 查找节点 (Search)

- 根据二叉搜索树的性质递归查找节点，时间复杂度取决于树的高度。

```c
int search_avl_tree(AVLTree *tree, int key) {
    return search_node(tree->root, key);
}
```

### 4. 平衡调整 (旋转操作)

- **右旋** (Right Rotate)：左子树过高时进行右旋，降低左子树高度。
- **左旋** (Left Rotate)：右子树过高时进行左旋，降低右子树高度。
- **左右旋** (Left-Right Rotate)：先对左子节点左旋，再右旋父节点。
- **右左旋** (Right-Left Rotate)：先对右子节点右旋，再左旋父节点。

```c
AVLNode *right_rotate(AVLNode *y);
AVLNode *left_rotate(AVLNode *x);
```

### 算法的时间复杂度

- **查找操作**：$O(\log n)$
- **插入操作**：$O(\log n)$
- **删除操作**：$O(\log n)$
- **旋转操作**：$O(1)$

AVL树的高度保持在 $O(\log n)$ 范围内，因此查找、插入和删除操作的时间复杂度均为 $O(\log n)$。每次插入或删除最多需要进行一次或两次旋转，每次旋转的复杂度为 $O(1)$，因此插入的复杂度记最高次数项为 $O(\log n)$。