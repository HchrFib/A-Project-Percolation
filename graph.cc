#include "graph.h"

Graph::Graph() : num_nodes(0), width(0), adjacency(0) {}

Graph::Graph(int n) : num_nodes(n), adjacency(n) {
    
    width = static_cast<int>(sqrt(n)); // used only for square_grid
}

void Graph::add_edge(int u, int v) {
    adjacency[u].push_back(v);
    adjacency[v].push_back(u);
}

void Graph::show_graph() {
    for (int i = 0; i < num_nodes; i++) {
        cout << i << ": ";
        for (int v : adjacency[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

void Graph::generate_dot(const string& filename, int num_components, int max_component_size) {
    ofstream dot_file(filename);
    if (!dot_file) {
        cerr << "Error creating file " << filename << endl;
        return;
    }

    dot_file << "graph G {" << endl;
    dot_file << "    label=\"number of connected components: " << num_components 
             << "\\nMaximum connected component size: " << max_component_size 
             << "\\nFilename: " << filename << "\";" << endl;

    for (int i = 0; i < num_nodes; i++) {
        dot_file << "    " << i << ";" << endl;

        for (int v : adjacency[i]) {
            if (i < v) {
                dot_file << "    " << i << " -- " << v << ";" << endl;
            }
        }
    }

    dot_file << "}" << endl;
    dot_file.close();
}
