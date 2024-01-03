#include "astar.hpp"
#include "state_graph.hpp"

#include <iostream>

int main(){
    // first comparison
    int nbStacks = 4;
    int nbBlocs = 8;

    std::cout << "g(" << nbStacks << ", " << nbBlocs << ", defaultHeuristic): " << std::endl;
    StateGraph g(nbStacks, nbBlocs, 0);
    AStar::astar(g.initialState(), g);

    std::cout << "g(" << nbStacks << ", " << nbBlocs << ", heuristic1): " << std::endl;
    g.setHeuristic(1);
    AStar::astar(g.initialState(), g);
    
    return 0;
}