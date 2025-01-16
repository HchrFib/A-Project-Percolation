#ifndef UTILS_H
#define UTILS_H

#include "UnionFind.h"
#include "graph.h"
#include "directoryManager.h"
#include "models.h"
#include <vector>
#include <algorithm>
#include <fstream> 
#include <iomanip> 
#include <cmath>    
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>

struct Metrics;
struct VMetrics;

// progress bar
void show_progress_bar(int total, int current);
// DFS function to traverse a connected compnent
void dfs(int node, const vector<vector<int>>& adjList, vector<bool>& visited);

// Functon to count the number of connected components
int countConnectedComponents(const Graph& G, int& max_component_size,unordered_map<long, long>& component_count);

// percolation probability
Metrics percolation_probability(const Graph& G, int num_simulations, double p, string type_percolation, string model_graph);
void simulate_percolation_transition(int num_simulations, string type_percolation, int num_nodes,string model_graph, double p) ;

//Percolation property 
bool percolates(int max_component_size, int G_num_nodes); 

// Bond percolation function
Graph percolate_by_nodes(const Graph& G, double q);
// Side percolation function
Graph percolate_by_edges(const Graph& G, double q);

// percolation using Union-Find on the square grid
bool percolates_with_union_find(const Graph& G);

#endif


