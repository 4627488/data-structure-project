# 5、Huffman编码与解码 (必做)（Huffman编码、二叉树）

## 项目简介

本项目实现了Huffman编码和解码算法。Huffman编码是一种无损数据压缩算法，通过使用变长编码来减少数据的存储空间。该项目包括以下功能：

- 计算字符频率
- 构建Huffman树
- 生成Huffman编码
- 编码文件
- 解码文件

## 文件结构

- `huffman.h`：包含Huffman编码和解码的函数声明和数据结构定义。
- `huffman.c`：包含Huffman编码和解码的函数实现。
- `main.c`：包含主函数，用于调用编码和解码函数。
- `Makefile`：用于构建和测试项目的Makefile。
- `source.txt`：示例输入文件。
- `code.dat`：编码后的二进制文件。
- `Huffman.txt`：保存Huffman编码的文件。
- `recode.txt`：解码后的输出文件。

## 使用方法

### 构建&编译项目

构建项目，并编码`source.txt`文件：

```sh
make && ./huffman
```

### 测试项目

在项目根目录下运行以下命令以测试项目，测试将编码和解码`source.txt`文件：

```sh
make test
```

该命令将运行编码和解码过程，并使用`diff`命令检查`recode.txt`和`source.txt`是否相同。如果相同，将输出"Test passed!"，否则输出"Test failed!"。

### 清理项目

在项目根目录下运行以下命令以清理项目：

```sh
make clean
```

该命令将删除生成的目标文件和中间文件。

## 代码示例

### 编码文件

```c
const char* inputFile = "source.txt";
const char* encodedFile = "code.dat";
const char* decodedFile = "recode.txt";
const char* codeFile = "Huffman.txt";

// 编码文件
encodeFile(inputFile, encodedFile, codeFile);

// 解码文件
decodeFile(encodedFile, decodedFile, codeFile);
```

## 数据结构

**哈夫曼树节点 (HuffmanNode)**

```c
typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;
```

`data`：存储字符或标志符，如内部节点用`$`表示。  
`freq`：该字符或子树的频率。  
`left` 和 `right`：指向子节点的指针。

**小根堆 (MinHeap)**

```c
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode **array;
} MinHeap;
```

`array`：一个存储哈夫曼节点指针的数组，用于维护小根堆。  
`size`：当前堆中的元素数量。  
`capacity`：堆的最大容量。

## 算法设计思想

1. 统计文本文件中每个字符的出现次数，结果存入`freq`数组。
2. 将字符和频率转化为哈夫曼节点，并按频率最小构建小根堆。特别地，插入一个EOF节点，频率为1，用于标记文件结束。
3. 反复从小根堆中取出两个频率最低的节点，合并为新节点，插回堆中。最终堆中只剩一个节点，即哈夫曼树的根节点。
4. 使用深度优先搜索(DFS)遍历哈夫曼树，左分支记作`0`，右分支记作`1`，递归生成每个叶子节点的编码。
5. 使用生成的Huffman编码对文件进行编码和解码。

**文件编码**：遍历文件，每次将字符替换为对应的哈夫曼编码，并逐位写入文件。如果不满8位，则缓存，待补齐8位后写入。

**文件解码**：逐位读取压缩文件，沿着哈夫曼树进行位移，直到找到叶子节点，即解码出字符。重复此过程直到读取到特殊的EOF标记。

## 算法复杂度

**小根堆维护 (minHeapify)**

```c
void minHeapify(MinHeap *minHeap, int idx)
```

维护小根堆的性质，使父节点的频率始终小于或等于子节点的频率。  
时间复杂度：单次调整的复杂度为 $O(log n)$。
空间复杂度：$O(n)$，用于存储频率和节点。

**堆构建 (buildMinHeap)**

```c
void buildMinHeap(MinHeap *minHeap)
```

自底向上调整堆结构，使其成为小根堆。  
时间复杂度：整体复杂度为$O(n)$。

**构建哈夫曼树**

```c
HuffmanNode *buildHuffmanTree()
```

通过多次提取最小节点并合并，最终形成哈夫曼树。  
时间复杂度：整体复杂度为 $O(n log n)$，其中 $n$ 为字符种类数量。

## 测试结果

编码 `source.txt` 文件，生成 `code.dat` 和 `Huffman.txt` 文件，解码后生成 `recode.txt` 文件。

```sh
make test
```

测试结果显示编码和解码过程正确，`recode.txt` 和 `source.txt` 文件内容一致。

一开始编码文件大小为 6,281 字节，编码后文件大小为 3,497 字节，压缩率约为 44.3%。