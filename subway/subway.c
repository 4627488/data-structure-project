#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int u, v, weight;
} Edge;

void swap(Edge *a, Edge *b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Edge arr[], int low, int high) {
    int pivot = arr[high].weight;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].weight < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(Edge arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int find(int parent[], int u) {
    if (parent[u] != u) {
        parent[u] = find(parent, parent[u]);
    }
    return parent[u];
}

void unite(int parent[], int rank[], int u, int v) {
    int rootU = find(parent, u);
    int rootV = find(parent, v);
    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    Edge *edges = (Edge *)malloc(m * sizeof(Edge));
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].weight);
    }

    quickSort(edges, 0, m - 1);

    int *parent = (int *)malloc((n + 1) * sizeof(int));
    int *rank = (int *)calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }

    int maxWeight = 0;
    for (int i = 0; i < m; ++i) {
        if (find(parent, edges[i].u) != find(parent, edges[i].v)) {
            unite(parent, rank, edges[i].u, edges[i].v);
            if (edges[i].weight > maxWeight) {
                maxWeight = edges[i].weight;
            }
            if (find(parent, 1) == find(parent, n)) {
                break;
            }
        }
    }

    printf("%d\n", maxWeight);

    free(edges);
    free(parent);
    free(rank);

    return 0;
}