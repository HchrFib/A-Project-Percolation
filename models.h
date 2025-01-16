#ifndef MODELS_H
#define MODELS_H

#include "graph.h"
#include <cmath>
#include <cstdlib>

// Models
Graph square_grid(int n);
//Generate random graph Erdos-Renyi
Graph erdos_renyi(int n, double q);
//Generate random graph Geometric
Graph random_geometric_graph(int n, double radio);

#endif
