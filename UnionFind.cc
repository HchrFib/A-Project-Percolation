#include "UnionFind.h"

// Constructor that initializes the parent and rank vectors
UnionFind::UnionFind(int N) {
    parent.resize(N);
    rank.resize(N, 0);
    for (int i = 0; i < N; ++i) {
        parent[i] = i;
    }
}

// Method to find the representative of the set containing x
int UnionFind::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Path compression
    }
    return parent[x];
}

// Method to unite the sets that contain x and y
void UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
        // Union by rank
        if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

// Method to check if x and y are in the same set
bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}
