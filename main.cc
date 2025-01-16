#include "graph.h"
#include "utils.h"
#include "models.h"
#include <iostream>
#include <string>
#include <cstdlib>  // Para rand() y srand()
#include <ctime>    // Para time()

int main(int argc, char *argv[]) {

    srand(static_cast<unsigned int>(time(0)));

    // Llegir paràmetres de la línia de comandes: model del graf, mida del graf, tipus de percolació, probabilitat de generació pels models geomètric i d'Erdos-Renyi
    if ((argc != 4) && (argc != 5)) {
        cout << "Usage: " << argv[0] << " <model_graph> <graph_size> <type_percolation> [generation_probability]" << endl;
        return 1;
    }
    string model_graph = argv[1];
    int graph_size = atoi(argv[2]);
    string type_percolation = argv[3];
    double generation_probability = 0.0;
    if (argc == 5) {
        generation_probability = atof(argv[4]);
        if (generation_probability > 1) cout << "generation_probability no pot ser superior a 1" << endl <<
            "utilitzant valor per defecte" << endl;
        else if (generation_probability < 0) cout << "generation_probability no pot ser inferior a 0" << endl <<
            "utilitzant valor per defecte" << endl;
    }

    // Paràmetres fixes dels experiments
    int num_simulations = 50;
    // int step_size de les simulacions també es podria configurar aquí
    
    if (model_graph == "s" || model_graph == "S") { // Square grid
      
       
            simulate_percolation_transition(num_simulations, type_percolation, graph_size, "square_grid",0);
    
    } else if (model_graph == "g" || model_graph == "G") { // Geometric

        vector<double> Prob = {0.3, 0.40} ;
        for(double i = 0; i < Prob.size(); i++) {
            simulate_percolation_transition(num_simulations, type_percolation, graph_size, "geometric" , Prob[i]);    
        } 
        
    } else if (model_graph == "b" || model_graph == "B") { // Binomial Erdos-Renyi
        Graph G(graph_size);
        vector<double> Prob;
        if(graph_size == 50)  Prob = {0.15,0.20,0.25};
        else if(graph_size == 25) Prob = {0.10, 0.15,0.20};
        else if(graph_size == 100) Prob = {0.03,0.06,0.09};
        for(double i = 0; i < Prob.size(); i++) {
            simulate_percolation_transition(num_simulations, type_percolation, graph_size, "erdos_renyi" , Prob[i]);    
        } 

        
    } else {
        cout << "Invalid model_graph" << endl;
        return 1;
    }

    cout << "Done!" << endl;

}