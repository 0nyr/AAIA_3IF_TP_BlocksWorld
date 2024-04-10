#include "astar.hpp"
#include "state_graph.hpp"

#include <iostream>

int main(){
    // first comparison
    int nbStacks = 4;
    int nbBlocs = 8;

    StateGraph g = StateGraph(nbStacks, nbBlocs, 0);

    // for (int i = 0; i <= 4; i++) {
    //     std::cout << "g(" << nbStacks << ", " << nbBlocs << ", heuristic" << i << "): " << std::endl;
    //     g.setHeuristic(i);
    //     AStar::astar(g.initialState(), g);
    // }

    // more advanced computation
    nbBlocs = 16;
    g = StateGraph(nbStacks, nbBlocs, 0);

    for (int i = 2; i <= 4; i += 2) {
        std::cout << "g(" << nbStacks << ", " << nbBlocs << ", heuristic" << i << "): " << std::endl;
        g.setHeuristic(i);
        AStar::astar(g.initialState(), g);
    }

    // // even more advanced computation
    // nbBlocs = 20;

    // std::cout << "g(" << nbStacks << ", " << nbBlocs << ", heuristic" << 4 << "): " << std::endl;
    // g = StateGraph(nbStacks, nbBlocs, 4);
    // AStar::astar(g.initialState(), g);

    // AWA* computations
    nbStacks = 5;
    nbBlocs = 25;
    int weight = 2;

    std::cout << "g(" << nbStacks << ", " << nbBlocs << ", heuristic" << 4 << "): " << std::endl;
    g = StateGraph(nbStacks, nbBlocs, 4);
    AStar::awa_star(g.initialState(), g, weight);
    
    return 0;
}