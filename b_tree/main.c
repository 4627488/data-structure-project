#include "b_tree.h"
#include <stdio.h>
#include <stdlib.h>

void readPrimes(int *primes, int size);
void writeResults(BTree *tree, const char *filename, int *queries, int size);

int isPrime(int n)
{
    if (n < 2)
    {
        return 0;
    }
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    BTree *tree = createBTree();
    int primes[10000];
    readPrimes(primes, 10000);

    // 给B树插入10000个素数
    for (int i = 0; i < 10000; i++)
    {
        insert(tree, primes[i]);
    }

    // 查询200-300的数是否在B树中
    int queries[10001];
    for (int i = 200; i <= 300; i++)
    {
        queries[i - 200] = i;
    }
    writeResults(tree, "b-tree1.txt", queries, 101);

    // 删除500-2000的数
    for (int i = 500; i <= 2000; i++)
    {
        if (search(tree, i))
        {
            removeKey(tree, i);
        }
    }

    // 查询 600-700 的质数数是否在B树中
    int size = 0;
    for (int i = 600; i <= 700; i++)
    {
        if (isPrime(i))
        {
            queries[size++] = i;
        }
    }
    writeResults(tree, "b-tree2.txt", queries, size);

    // 插入1-1000的所有偶数
    for (int i = 2; i <= 1000; i += 2)
    {
        insert(tree, i);
    }

    // 查询100-200的所有偶数是否在B树中
    for (int i = 100; i <= 200; i += 2)
    {
        queries[i / 2 - 50] = i;
    }
    writeResults(tree, "b-tree3.txt", queries, 51);

    freeBTree(tree);
    return 0;
}

void readPrimes(int *primes, int size)
{
    FILE *file = fopen("primes.txt", "r");
    if (file == NULL)
    {
        perror("Failed to open primes.txt");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
    {
        if (fscanf(file, "%d", &primes[i]) != 1)
        {
            perror("Failed to read prime number");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

void writeResults(BTree *tree, const char *filename, int *queries, int size)
{
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d %s\n", queries[i],
                search(tree, queries[i]) ? "yes" : "no");
    }
    fclose(file);
}
