#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int N);
    int find(int x);
    void unite(int x, int y);
    bool connected(int x, int y);
};

#endif
