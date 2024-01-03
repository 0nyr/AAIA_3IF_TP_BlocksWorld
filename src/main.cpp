#include "astar.hpp"
#include "state_graph.hpp"

int main(){
    // StateGraph g;
    StateGraph g(3, 3, heuristic1);

    AStar::astar(g.initialState(), g);
    
    return 0;
}