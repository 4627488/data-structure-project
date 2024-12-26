#include "huffman.h"
#include <assert.h>
#include <string.h>

#define EOF_MARKER 0x00

HuffmanNode *createNode(char data, unsigned freq) {
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

void swapHuffmanNode(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) // 需要旋转
    {
        swapHuffmanNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

MinHeap *createMinHeap(unsigned capacity) {
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode **)malloc(minHeap->capacity * sizeof(HuffmanNode *));
    return minHeap;
}

void buildMinHeap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}

HuffmanNode *extractMin(MinHeap *minHeap) {
    HuffmanNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap *minHeap, HuffmanNode *node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

MinHeap *createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap *minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

HuffmanNode *buildHuffmanTree(char data[], int freq[], int size) {
    HuffmanNode *left, *right, *top;
    MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void calculateFrequencies(const char *inputFile, int freq[]) {
    FILE *file = fopen(inputFile, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        freq[(unsigned char)ch]++;
    }

    freq[EOF_MARKER] = 1; // Add frequency for EOF marker

    fclose(file);
}

void generateCodes(HuffmanNode *root, int arr[], int top, char codes[][256]) {
    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1, codes);
    }

    if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1, codes);
    }

    if (!root->left && !root->right) {
        char code[256];
        for (int i = 0; i < top; ++i) {
            code[i] = arr[i] + '0';
        }
        code[top] = '\0';
        strcpy(codes[(unsigned char)root->data], code);
    }
}

void saveCodesToFile(char data[], char codes[][256], int size, const char *codeFile) {
    FILE *file = fopen(codeFile, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %s\n", (unsigned char)data[i], codes[(unsigned char)data[i]]);
    }

    fprintf(file, "%d %s\n", EOF_MARKER, codes[EOF_MARKER]); // Save EOF marker

    fclose(file);
}

static int bitBuffer = 0; // 公共的writeBit 缓冲区，当剩余不足8位时，先不写入文件
static int bitCount = 0;

void writeBit(FILE *file, int bit) {
    bitBuffer = (bitBuffer << 1) | bit;
    bitCount++;

    if (bitCount == 8) {
        fputc(bitBuffer, file);
        bitBuffer = 0;
        bitCount = 0;
    }
}

void flushBits(FILE *file) {
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        fputc(bitBuffer, file);
        bitBuffer = 0;
        bitCount = 0;
    }
}

void encodeFile(const char *inputFile, const char *outputFile, const char *codeFile) {
    int freq[257] = {0}; // 多一个位置用于存储EOF标记
    calculateFrequencies(inputFile, freq);

    char data[257];
    int size = 0;
    for (int i = 0; i < 257; i++) {
        if (freq[i] > 0) {
            data[size++] = (char)i;
        }
    }

    HuffmanNode *root = buildHuffmanTree(data, freq, size);

    char codes[257][256] = {{0}}; // 编码表
    int arr[256], top = 0;
    generateCodes(root, arr, top, codes);

    saveCodesToFile(data, codes, size, codeFile);

    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "wb");
    if (!input || !output) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(input)) != EOF) {
        char *code = codes[(unsigned char)ch];
        for (int i = 0; code[i] != '\0'; i++) {
            writeBit(output, code[i] - '0');
        }
    }

    // 写入EOF标记
    char *code = codes[EOF_MARKER];
    for (int i = 0; code[i] != '\0'; i++) {
        writeBit(output, code[i] - '0');
    }

    flushBits(output);

    fclose(input);
    fclose(output);
}

HuffmanNode *buildHuffmanTreeFromCodes(char codes[257][256], char data[], int size) {
    HuffmanNode *root = createNode('$', 0);
    for (int i = 0; i < size; i++) {
        HuffmanNode *current = root;
        char *code = codes[(unsigned char)data[i]];
        for (int j = 0; code[j] != '\0'; j++) {
            if (code[j] == '0') {
                if (!current->left) {
                    current->left = createNode('$', 0);
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = createNode('$', 0);
                }
                current = current->right;
            }
        }
        current->data = data[i];
    }
    return root;
}

void decodeFile(const char *inputFile, const char *outputFile, const char *codeFile) {
    FILE *codeFilePtr = fopen(codeFile, "r");
    if (!codeFilePtr) {
        perror("Failed to open code file");
        exit(EXIT_FAILURE);
    }

    char codes[257][256] = {{0}};
    char data[257];
    int size = 0;
    int ascii;
    char code[256];

    char line[256];
    while (fgets(line, sizeof(line), codeFilePtr)) {
        if (sscanf(line, "%d %s", &ascii, code) == 2) {
            char ch = (char)ascii;
            strcpy(codes[(unsigned char)ch], code);
            data[size++] = ch;
        }
    }
    fclose(codeFilePtr);

    FILE *input = fopen(inputFile, "rb");
    FILE *output = fopen(outputFile, "w");
    if (!input || !output) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    HuffmanNode *root = buildHuffmanTreeFromCodes(codes, data, size);
    HuffmanNode *current = root;
    int bit;
    while ((bit = fgetc(input)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            int currentBit = (bit >> i) & 1;
            if (currentBit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            assert(current != NULL); // 保证编码表正确
            if (!current->left && !current->right) {
                if ((unsigned char)current->data == EOF_MARKER) // 到达EOF标记
                {
                    fclose(input);
                    fclose(output);
                    return;
                }
                fputc(current->data, output);
                current = root;
            }
        }
    }

    fclose(input);
    fclose(output);
}
