#pragma once

#include "state.hpp"
#include "state_graph.hpp"

#include <queue>
#include <unordered_map>
#include <limits>

/**
 * @brief Information about a state, in the state graph.
*/
struct StateNodeInfo {
    int cost;
    State pred;
    bool isGrey; // grey = not explored yet, black = explored
};

class AStar {
public:
    static void astar(const State &s0, StateGraph &g);
    static void awa_star(const State &s0, StateGraph &g, int weight);
    
private:
    struct Compare {
        bool operator()(std::pair<State, int> const &s1, std::pair<State, int> const &s2);
    };

    using PriorityQueue = std::priority_queue<std::pair<State,int>, std::vector<std::pair<State,int>>, Compare>;
    using HashMap = std::unordered_map<State, StateNodeInfo, State::hash>;

    static void printSolution(StateGraph &g, const State &s0, const State &s, const HashMap &map);
};
