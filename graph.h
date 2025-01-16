#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Graph {
public:
    int num_nodes;
    int width; 
    vector<vector<int>> adjacency; 
    Graph();
    Graph(int n);
    void add_edge(int u, int v);
    void show_graph();
    void generate_dot(const string& filename, int num_components, int max_component_size);
};

#endif

