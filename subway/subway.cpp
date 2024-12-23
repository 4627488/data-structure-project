#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

int find(vector<int>& parent, int u) {
    if (parent[u] != u) {
        parent[u] = find(parent, parent[u]);
    }
    return parent[u];
}

void unite(vector<int>& parent, vector<int>& rank, int u, int v) {
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
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    sort(edges.begin(), edges.end());

    vector<int> parent(n + 1), rank(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }

    int maxWeight = 0;
    for (const auto& edge : edges) {
        if (find(parent, edge.u) != find(parent, edge.v)) {
            unite(parent, rank, edge.u, edge.v);
            maxWeight = max(maxWeight, edge.weight);
            if (find(parent, 1) == find(parent, n)) {
                break;
            }
        }
    }

    cout << maxWeight << endl;
    return 0;
}
