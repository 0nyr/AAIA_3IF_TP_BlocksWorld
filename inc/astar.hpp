#pragma once

#include <queue>
#include <unordered_map>
#include "state.hpp"
#include "state_graph.hpp"

struct Info {
    int g;
    State pred;
    bool isGrey;
};

class AStar {
public:
    static void astar(const State &s0, StateGraph &g);
    
private:
    struct Compare {
        bool operator()(std::pair<State, int> const &s1, std::pair<State, int> const &s2);
    };

    using PriorityQueue = std::priority_queue<std::pair<State,int>, std::vector<std::pair<State,int>>, Compare>;
    using HashMap = std::unordered_map<State, Info, State::hash>;

    static void printSolution(StateGraph &g, const State &s0, const State &s, const HashMap &map);
};
