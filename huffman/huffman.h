#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>

typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffmanNode **array;
} MinHeap;

HuffmanNode *createNode(char data, unsigned freq);
MinHeap *createMinHeap(unsigned capacity);
void insertMinHeap(MinHeap *minHeap, HuffmanNode *node);
HuffmanNode *extractMin(MinHeap *minHeap);
void printCodes(HuffmanNode *root, int arr[], int top, FILE *output);
void HuffmanCodes(char data[], int freq[], int size, FILE *output);
void encodeFile(const char *inputFile, const char *outputFile,
                const char *codeFile);
void decodeFile(const char *inputFile, const char *outputFile,
                const char *codeFile);

#endif // HUFFMAN_H
