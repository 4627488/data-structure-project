#include "huffman.h"

int main() {
    const char *inputFile = "source.txt";
    const char *encodedFile = "code.dat";
    const char *decodedFile = "recode.txt"; // 为什么不是decode呢?
    const char *codeFile = "Huffman.txt";

    // 编码文件
    encodeFile(inputFile, encodedFile, codeFile);

    // 解码文件
    decodeFile(encodedFile, decodedFile, codeFile);

    return 0;
}
