#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>

void readPrimes(int *primes, int size);
void writeResults(const char *filename, int *results, int size, int start);

int main() {
    BTree *tree = createBTree();
    int primes[10000];
    readPrimes(primes, 10000);

    // Build B-tree with primes
    for (int i = 0; i < 10000; i++) {
        insert(tree, primes[i]);
    }

    // Query 200-300
    int results[101];
    for (int i = 200; i <= 300; i++) {
        results[i - 200] = search(tree, i);
    }
    writeResults("b-tree1.txt", results, 101, 200);

    // Delete primes 500-2000
    for (int i = 500; i <= 2000; i++) {
        if (search(tree, i)) {
            delete (tree, i);
        }
    }

    // Query 600-700
    for (int i = 600; i <= 700; i++) {
        results[i - 600] = search(tree, i);
    }
    writeResults("b-tree2.txt", results, 101, 600);

    // Insert even numbers 1-1000
    for (int i = 2; i <= 1000; i += 2) {
        insert(tree, i);
    }

    // Query 100-200 even numbers
    for (int i = 100; i <= 200; i += 2) {
        results[(i - 100) / 2] = search(tree, i);
    }
    writeResults("b-tree3.txt", results, 51, 100);

    freeBTree(tree);
    return 0;
}

void readPrimes(int *primes, int size) {
    FILE *file = fopen("primes.txt", "r");
    if (file == NULL) {
        perror("Failed to open primes.txt");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        if (fscanf(file, "%d", &primes[i]) != 1) {
            perror("Failed to read prime number");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

void writeResults(const char *filename, int *results, int size, int start) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %s\n", start + i, results[i] ? "yes" : "no");
    }
    fclose(file);
}
