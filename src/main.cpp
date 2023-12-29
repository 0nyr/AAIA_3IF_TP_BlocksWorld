#include "astar.hpp"

int main(){
    StateGraph g;
    AStar::astar(g.initialState(), g);
    return 0;
}