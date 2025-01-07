#include "avl_tree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

void generate_primes(int *primes, int max) {
    int count = 0;
    for (int i = 2; i <= max; i++) {
        if (is_prime(i)) {
            primes[count++] = i;
        }
    }
}

void write_results(const char *filename, int *nums, int size, AVLTree *tree) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d %s\n", nums[i],
                search_avl_tree(tree, nums[i]) ? "yes" : "no");
    }
    fclose(file);
}

int main() {
    // 测试
    AVLTree *tree2 = create_avl_tree();
    insert_avl_tree(tree2, 50);
    insert_avl_tree(tree2, 30);
    insert_avl_tree(tree2, 70);
    insert_avl_tree(tree2, 20);
    insert_avl_tree(tree2, 40);
    insert_avl_tree(tree2, 60);
    insert_avl_tree(tree2, 80);
    insert_avl_tree(tree2, 10);
    insert_avl_tree(tree2, 25);
    insert_avl_tree(tree2, 35);
    insert_avl_tree(tree2, 45);
    insert_avl_tree(tree2, 55);
    insert_avl_tree(tree2, 65);
    insert_avl_tree(tree2, 75);
    insert_avl_tree(tree2, 85);

    print_avl_tree(tree2);
    free_avl_tree(tree2);

    AVLTree *tree = create_avl_tree();
    int primes[10000];
    generate_primes(primes, 10000);

    for (int i = 0; i < 10000 && primes[i] != 0; i++) {
        insert_avl_tree(tree, primes[i]);
    }
    // print_avl_tree(tree);

    int query1[101];
    for (int i = 200; i <= 300; i++) {
        query1[i - 200] = i;
    }
    write_results("tree1.txt", query1, 101, tree);

    for (int i = 500; i <= 2000; i++) {
        if (is_prime(i)) {
            delete_avl_tree(tree, i);
        }
    }
    // print_avl_tree(tree);
    int size = 0;
    int query2[101];
    for (int i = 600; i <= 700; i++) {
        if (is_prime(i)) {
            query2[size++] = i;
        }
    }
    write_results("tree2.txt", query2, size, tree);

    for (int i = 2; i <= 1000; i += 2) {
        insert_avl_tree(tree, i);
    }
    // print_avl_tree(tree);

    int query3[51];
    for (int i = 100; i <= 200; i += 2) {
        query3[(i - 100) / 2] = i;
    }
    write_results("tree3.txt", query3, 51, tree);

    free_avl_tree(tree);
    return 0;
}
