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

### 构建项目

在项目根目录下运行以下命令以构建项目：

```sh
make
```

### 编码文件

在项目根目录下运行以下命令以编码`source.txt`文件：

```sh
./huffman
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

## 数据结构

### HuffmanNode

```c
typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;
```

### MinHeap

```c
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode **array;
} MinHeap;
```

## 算法设计思想

1. 计算每个字符的频率。
2. 使用这些频率构建一个最小堆。（插入一个EOF节点，频率为1）
3. 从最小堆中提取两个最小频率的节点，创建一个新节点，其频率为这两个节点频率之和，并将这两个节点作为新节点的子节点。
4. 将新节点插入最小堆。
5. 重复步骤3和4，直到堆中只剩下一个节点，这个节点就是Huffman树的根节点。
6. 通过遍历Huffman树生成每个字符的Huffman编码。
7. 使用生成的Huffman编码对文件进行编码和解码。

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

## 算法时间复杂度

Huffman编码算法的时间复杂度主要由以下几个部分组成：

1. 计算字符频率：O(n)，其中n是输入字符的数量。
2. 构建最小堆：O(d log d)，其中d是不同字符的数量。
3. 构建Huffman树：O(d log d)。
4. 生成Huffman编码：O(d)。
5. 编码文件：O(n)。
6. 解码文件：O(n)。

因此，Huffman编码算法的总体时间复杂度为O(n + d log d)。

## 改进方法

1. 使用更高效的数据结构，如斐波那契堆，可以进一步优化最小堆的构建和操作。
2. 在实际应用中，可以结合其他压缩算法（如LZW）以提高压缩效率。
3. 对于特定类型的数据，可以根据其特点设计专门的编码方案，以获得更好的压缩效果。