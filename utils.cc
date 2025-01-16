#include "utils.h"
#include <unordered_map>
#include "excel_image_generator.h"
#include <ctime> // Incluir para usar la fecha y hora

using namespace std;

struct Metrics {
    int sum_NCC = 0;                        // Summation of the number of connected components
    int sum_MSCC = 0;                       // Summation maximum size connected component
    double percolation_rate = 0.0;          // Variable to store percolation rate
    double sum_relative_size_CCG = 0.0;     // Summation of the relative size of the giant connected component
    double sum_susceptibility = 0.0;        // Summation to store the susceptibility
    unordered_map<long, long> component_count; // Count of components by size
};

struct VMetrics {
    vector<double> probabilities;           // Store the p values
    vector<double> percolation_rates;       // Store the percolation rates
    vector<double> average_NCC;             // Store average number of connected components          
    vector<double> average_MSCC;            // Store average connected component max size
    vector<double> average_relative_size;    // Store average relative sizes of the giant connected component
    vector<double> average_susceptibility;  // Store average susceptibilities
};

string get_current_time_as_string() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", ltm);
    return string(timestamp);
}

void show_progress_bar(int total, int current) {
    const int bar_width = 50;
    std::cout << "[";
    int pos = bar_width * current / total;
    for (int i = 0; i < bar_width; ++i) {
        std::cout << (i < pos ? "=" : " ");
    }
    std::cout << "] " << current * 100 / total << "%\r" << std::flush;
}


// DFS to count size of connected component
int dfs_and_count_size(int node, const vector<vector<int>>& adjList, vector<bool>& visited) {
    visited[node] = true;
    int size = 1;
    for (int neighbor : adjList[node]) {
        if (!visited[neighbor]) size += dfs_and_count_size(neighbor, adjList, visited);
    }

    return size;
}

// Count number of connected components and find the largest component size
int countConnectedComponents(const Graph& G, int& max_component_size, unordered_map<long, long>& component_count) {
    vector<bool> visited(G.num_nodes, false);
    int components = 0;
    max_component_size = 0;

    for (int i = 0; i < G.num_nodes; i++) {
        if (!visited[i]) {
            components++;
            int component_size = dfs_and_count_size(i, G.adjacency, visited);
            component_count[component_size]++; // Count the number of components of this size
            max_component_size = max(max_component_size, component_size);
        }
    }
    return components;  // Return total number of components
}

// Node percolation
Graph percolate_by_nodes(const Graph& G, double q) {
    vector<int> surviving_nodes;
    vector<int> new_index(G.num_nodes, -1);  // Map nodes to their new index

    for (int u = 0; u < G.num_nodes; u++) {
        double probability = (double) rand() / RAND_MAX;
        if (probability <= q) {
            new_index[u] = surviving_nodes.size();
            surviving_nodes.push_back(u);
        }
    }

    Graph Gq(surviving_nodes.size());
    for (int u : surviving_nodes) {
        for (int v : G.adjacency[u]) {
            if (new_index[v] != -1 && new_index[u] < new_index[v]) Gq.add_edge(new_index[u], new_index[v]);
        }
    }
    return Gq;
}

// Edge percolation
Graph percolate_by_edges(const Graph& G, double q) {
    Graph Gq(G.num_nodes);
    for (int u = 0; u < G.num_nodes; u++) {
        for (int v : G.adjacency[u]) {
            if (u < v) {
                double probability = (double) rand() / RAND_MAX;
                if (probability <= q) Gq.add_edge(u, v);
            }
        }
    }
    return Gq;
}

// Check if percolates using union-find
bool percolates_with_union_find(const Graph& G) {
    UnionFind uf(G.num_nodes);
    // Connect nodes in the graph
    for (int i = 0; i < G.num_nodes; i++) {
        for (int neighbor : G.adjacency[i]) uf.unite(i, neighbor);
    }

    // Check if any node from the top row is connected to any node in the bottom row
    for (int top = 0; top < G.width; top++) {
        int topRoot = uf.find(top);  // Root of the current top node
        for (int bottom = G.num_nodes - G.width; bottom < G.num_nodes; bottom++) {
            if (uf.find(bottom) == topRoot) return true;  // Percolation path found
        }
    }
    return false;
}

// Check if it percolates based on the size of the largest component
bool percolates(int max_component_size, int G_num_nodes) {
    return max_component_size > G_num_nodes * 0.50;
}

// Calculate the percolation probability
Metrics percolation_probability(const Graph& G, int num_simulations, double p, string type_percolation, string model_graph) {
   
    int percolated_count = 0;
    Metrics metrics;
    unordered_map<long, long> total_component_count; // Total components of each size
    unordered_map<long, long> simulation_count;      // Number of simulations for each size of component

    for (int i = 0; i < num_simulations; i++) {
        Graph G_percolated;
        int max_component_size = 0;
        int num_components = 0;  
        
        if (type_percolation == "S" || type_percolation == "s") G_percolated = percolate_by_nodes(G, p);
        else if (type_percolation == "B" || type_percolation == "b") G_percolated = percolate_by_edges(G, p);
        
        unordered_map<long, long> component_count;
        num_components = countConnectedComponents(G_percolated, max_component_size, component_count);
        
        // Check if it percolates according to the model
        if (model_graph == "square_grid") {
            if (percolates_with_union_find(G_percolated)) percolated_count++;
        } else {
            if (percolates(max_component_size, G.num_nodes)) percolated_count++;
        }

        // Accumulate component sizes
        for (const auto& [size, count] : component_count) {
            total_component_count[size] += count;       // Total number of components of this size
            simulation_count[size]++;                   // Count the number of simulations for this size
        }

        metrics.sum_NCC  += num_components;
        metrics.sum_MSCC += max_component_size; 
        metrics.sum_relative_size_CCG += (double)max_component_size / G.num_nodes;
        
        // Save .dot file
        string f = model_graph + "_dot_file/Graph_percolate_by_nodes_prob_" + to_string(p) + "_simulation_" + to_string(i) + "_percolate_" + ((percolates_with_union_find(G_percolated)) ? "YES" : "NO") + "_.dot";
        G_percolated.generate_dot(f, num_components, max_component_size);
    }

    // Calculate average susceptibility
    double susceptibility = 0.0;
    double total_component_size = 0.0;

    for (const auto& [size, total_count] : total_component_count) {
        int num_simulations_for_size = simulation_count[size]; // Number of simulations for this size
        if (num_simulations_for_size > 0) {
            double average_count = static_cast<double>(total_count) / num_simulations_for_size; // Average appearances
            susceptibility += (double)(size*size*average_count); // Numerator: s² * n̄_s
            total_component_size += double(size*average_count);  // Denominator: s * n̄_s
        }
    }
    if (total_component_size > 0) susceptibility /= total_component_size; // Normalize susceptibility

    metrics.percolation_rate = static_cast<double>(percolated_count) / num_simulations;
    metrics.sum_susceptibility += susceptibility; // Save the susceptibility for this p value

    return metrics;
}

double find_percolation_threshold(vector<double>& Xs, vector<double>& Ys) {
 
    vector<double> slopes;

    // Calcula derivate
    for (uint i = 1; i < Xs.size(); ++i) {
        double slope = (Ys[i] - Ys[i - 1]) / 
                       (Xs[i] - Xs[i - 1]);
        slopes.push_back(slope);
    }

    // find slope
    auto max_slope_it = max_element(slopes.begin(), slopes.end());
    unsigned max_slope_index = distance(slopes.begin(), max_slope_it);

    // chekc valid index
    if (max_slope_index + 1 >= Xs.size()) {
        return -1.0;  
    }

    double threshold_probability = Xs[max_slope_index + 1]; // +1 to adjust the offset of the slope arrangement
    cout << "Estimated Threshold: " << threshold_probability << endl;

    return threshold_probability;
}

// Phase transition simulation
void simulate_percolation_transition(int num_simulations, string type_percolation, int num_nodes, string model_graph, double i) {
    
    srand(static_cast<unsigned int>(time(0)));
    // Create all directories
    DirectoryManager dirManager1(model_graph + "_xlsx");
    DirectoryManager dirManager2(model_graph + "_dot_file");
    
    // Try to create the directories
    dirManager1.createDirectory();
    dirManager2.createDirectory();
    
    vector<double> global_probabilities;
    vector<double> global_percolation_rates;
    vector<double> global_average_NCC;
    vector<double> global_average_MSCC;
    vector<double> global_average_relative_size;
    vector<double> global_average_susceptibility;

    vector<Graph> VG;
    Graph G;
    if(model_graph == "erdos_renyi") {
        for(int k = 0; k < 50; k++)  {
            G = erdos_renyi(num_nodes, i);
            VG.push_back(G);
        }

    } else if(model_graph == "square_grid")  {
            G = square_grid(num_nodes);
            VG.push_back(G);
    } else if(model_graph == "geometric" ) {
            G = random_geometric_graph(num_nodes,i );
            VG.push_back(G);
    }
    int num_graphs = VG.size();
    for(int i = 0; i < num_graphs; i++) {   
        Metrics metrics;   
        VMetrics vm;
        cout << "Processing graph : " << i + 1 << " " << model_graph << endl; 
        for(double p = 0.1;  p <= 1.0; p +=0.01) {
            show_progress_bar(100, 100*p+1);
            this_thread::sleep_for(std::chrono::milliseconds(100));
    
            metrics = percolation_probability(VG[i], num_simulations, p, type_percolation, model_graph);
            vm.probabilities.push_back(p);
            vm.percolation_rates.push_back(metrics.percolation_rate);
            vm.average_NCC.push_back(static_cast<double>(metrics.sum_NCC) / num_simulations);
            vm.average_MSCC.push_back(static_cast<double>(metrics.sum_MSCC) / num_simulations);
            vm.average_relative_size.push_back(metrics.sum_relative_size_CCG / num_simulations);
            vm.average_susceptibility.push_back(metrics.sum_susceptibility / num_simulations);
        }
        cout << endl;

        // Accumulate the results of this graph into the global vectors
        if (i == 0) {
            global_probabilities = vm.probabilities;
            global_percolation_rates = vm.percolation_rates;
            global_average_NCC = vm.average_NCC;
            global_average_MSCC = vm.average_MSCC;
            global_average_relative_size = vm.average_relative_size;
            global_average_susceptibility = vm.average_susceptibility;
        } else {
            // Add the values ​​of this graph to the global vectors
            for (size_t j = 0; j < global_probabilities.size(); j++) {
                global_percolation_rates[j] += vm.percolation_rates[j];
                global_average_NCC[j] += vm.average_NCC[j];
                global_average_MSCC[j] += vm.average_MSCC[j];
                global_average_relative_size[j] += vm.average_relative_size[j];
                global_average_susceptibility[j] += vm.average_susceptibility[j];
            }
        }

        // Print results to console
        cout << "Data saved in percolation_data_num_nodes_" << num_nodes << ".xlsx\n";
        cout << fixed << setprecision(4);  // Show 4 decimal places
        for (size_t i = 0; i < vm.probabilities.size(); i++) {
            cout << "p = "                           << vm.probabilities[i]          << " Percolation Rate = "       << vm.percolation_rates[i] 
                 << " Average num CC = "             << vm.average_NCC[i]            << " Average max size CC = "    << vm.average_MSCC[i]      
                 << " Average relative size CCG = "  << vm.average_relative_size[i]  << " Susceptibility = "         << vm.average_susceptibility[i] <<  endl;
        }
    }

    // Divide the cumulative results by the number of graphs to obtain the overall averages
    for (size_t j = 0; j < global_probabilities.size(); j++) {
        global_percolation_rates[j] /= num_graphs;
        global_average_NCC[j] /= num_graphs;
        global_average_MSCC[j] /= num_graphs;
        global_average_relative_size[j] /= num_graphs;
        global_average_susceptibility[j] /= num_graphs;
    }

    string timestamp = get_current_time_as_string();
  
    // Initialize the Excel file
    string excel_filename = model_graph + "_xlsx/percolation_" + type_percolation + "_" + to_string(num_nodes) + "_" + timestamp + ".xlsx";
    initialize_excel(excel_filename);

    double threshold_probability = 0.0;
    threshold_probability = find_percolation_threshold(global_probabilities, global_percolation_rates);
    
    // Worksheet name <= limit of 31 characters.
    // Create a spreadsheet for probabilities vs percolation rate
    create_excel(global_probabilities, global_percolation_rates, "Probability(p)", "Percolation rate", "Probability vs Percolation rate", threshold_probability);

    threshold_probability = 0.0;
    threshold_probability = find_percolation_threshold(global_probabilities, global_average_susceptibility);
    // Create another spreadsheet for probabilities vs average susceptibility
    create_excel(global_probabilities, global_average_susceptibility, "Probability(p)", "Average Susceptibility", "Probability vs Susceptibility", threshold_probability);

    threshold_probability = 0.0;
    threshold_probability = find_percolation_threshold(global_probabilities, global_average_relative_size);
    // Create another spreadsheet for probabilities vs average Relative size of the giant connected component
    create_excel(global_probabilities, global_average_relative_size, "Probability(p)", "Average relative size ", "Average Relative size giant CC", threshold_probability);


    threshold_probability = 0.0;
    threshold_probability = find_percolation_threshold(global_probabilities, global_average_NCC);
    // Create another spreadsheet for probabilities vs Average number Component Connected
    create_excel(global_probabilities, global_average_NCC, "Probability(p)", "Average num CC", "Prob vs Average num CC", threshold_probability);

    threshold_probability = 0.0;
    threshold_probability = find_percolation_threshold(global_probabilities, global_average_MSCC);
    // Create another spreadsheet for probabilities vs Average max size Component Connected
    create_excel(global_probabilities, global_average_MSCC, "Probability(p)", "Average max size CC", "Prob vs Average max size CC",threshold_probability);

    // Finalize the Excel file
    finalize_excel();
       
}
