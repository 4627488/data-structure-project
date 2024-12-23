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