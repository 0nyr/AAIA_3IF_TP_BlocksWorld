#include "astar.hpp"
#include "state_graph.hpp"

int main(){
    StateGraph g;
    AStar::astar(g.initialState(), g);
    return 0;
}