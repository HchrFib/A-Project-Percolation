#include "models.h"

struct Node {
    double x, y;
};

// create square grid
Graph square_grid(int n) {
    int size = n * n;
    Graph G(size);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int u = i * n + j;
            if (j < n - 1) G.add_edge(u, u + 1);
            if (i < n - 1) G.add_edge(u, u + n);
        }
    }
    return G;
}
//Generate random graph Erdos-Renyi
Graph erdos_renyi(int n, double p) {
    Graph G(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((double) rand() / RAND_MAX < p) {
                G.add_edge(i, j);
            }
        }
    }
    return G;
}
//Generate random graph Geometric
Graph random_geometric_graph(int n, double radio) {
    Graph G(n);
    vector<Node> nodes(n);
    for (int i = 0; i < n; i++) {
        nodes[i] = { (double) rand() / RAND_MAX, (double) rand() / RAND_MAX };
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double distance = sqrt(pow(nodes[i].x - nodes[j].x, 2) + pow(nodes[i].y - nodes[j].y, 2));
            if (distance < radio) {
                G.add_edge(i, j);
            }
        }
    }
    return G;
}
