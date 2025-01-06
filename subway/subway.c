#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int u, v, w; // 边的两个顶点和权重
} Edge;

// 交换两条边
void swap(Edge *a, Edge *b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;
}

// 合并
int _sort(Edge arr[], int low, int high) {
    int pivot = arr[high].w;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].w < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// 归并排序
void sort(Edge arr[], int low, int high) {
    if (low < high) {
        int pi = _sort(arr, low, high);
        sort(arr, low, pi - 1);
        sort(arr, pi + 1, high);
    }
}

// 并查集查找
int find(int fa[], int u) {
    if (fa[u] != u) {
        fa[u] = find(fa, fa[u]); // 路径压缩
    }
    return fa[u];
}

// 并查集合并
void unite(int fa[], int rank[], int u, int v) {
    int rootU = find(fa, u);
    int rootV = find(fa, v);
    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            fa[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            fa[rootU] = rootV;
        } else {
            fa[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

int main() {
    int n, m; // 顶点数和边数
    scanf("%d %d", &n, &m);
    Edge *edges = (Edge *)malloc(m * sizeof(Edge));
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);
    }

    // 边按权重排序
    sort(edges, 0, m - 1);

    int *fa = (int *)malloc((n + 1) * sizeof(int));
    int *rank = (int *)calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; ++i) {
        fa[i] = i;
    }

    int maxw = 0; // 记录生成树的最大权重
    for (int i = 0; i < m; ++i) {
        if (find(fa, edges[i].u) != find(fa, edges[i].v)) {
            unite(fa, rank, edges[i].u, edges[i].v);
            if (edges[i].w > maxw) {
                maxw = edges[i].w;
            }
            if (find(fa, 1) == find(fa, n)) {
                break;
            }
        }
    }

    printf("%d\n", maxw);

    free(edges);
    free(fa);
    free(rank);

    return 0;
}
