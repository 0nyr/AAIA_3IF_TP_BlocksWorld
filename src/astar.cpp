#include "astar.hpp"

#include <iostream>
#include <ctime>

/**
 * @brief Comparison operator for priority queue in A* algorithm.
 *
 * This operator compares two state-cost pairs based on their cost, 
 * facilitating the priority queue to order states in increasing order of cost.
 *
 * @param s1 The first state-cost pair.
 * @param s2 The second state-cost pair.
 * @return Returns true if the cost of the first state is greater than the second.
 */
bool AStar::Compare::operator()(std::pair<State, int> const &s1, std::pair<State, int> const &s2) {
    return s1.second > s2.second;
}

/** Recursively print the solution,
 * from the initial state to the final state */

/**
 * @brief Recursively print the solution from the initial state to the final state.
 *
 * This function prints out each step of the solution from the initial state 
 * to the final state by recursively tracing back from the final state to the initial state.
 *
 * @param g The state graph containing all possible states and transitions.
 * @param s0 The initial state of the search.
 * @param s The current state to print.
 * @param map The map containing all state information.
 */
void AStar::printSolution(StateGraph &g, const State &s0, const State &s, const HashMap &map) {
    if (!(s == s0)) {
        printSolution(g, s0, map.at(s).pred, map);
        g.print(map.at(s).pred, s);
    }
}

/**
 * @brief Implements the A* search algorithm.
 *
 * This function performs the A* search algorithm to find the shortest path
 * from an initial state to a goal state as defined by the StateGraph. It uses
 * a priority queue to dynamically select the next best state to explore.
 *
 * @param s0 The initial state from where the search begins.
 * @param g The state graph containing all possible states and transitions.
 */
void AStar::astar(const State &s0, StateGraph &g) {
    clock_t start = clock();
    int nbIter = 0;
    HashMap map;
    PriorityQueue q;
    map[s0].g = 0;
    map[s0].isGrey = true;
    q.push({s0, g.heuristic(s0)});
    while (!q.empty()) {
        State s = q.top().first;
        q.pop();
        if (!map[s].isGrey) continue;
        nbIter++;
        if (g.isFinal(s)) {
            std::cout << "Optimal solution of length " << map.at(s).g << " found in " << nbIter << " iterations and "
                      << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
            printSolution(g, s0, s, map);
            return;
        }
        int nbActions = g.searchActions(s);
        for (int i = 0; i < nbActions; i++) {
            State ss = g.transition(s, i);
            if (map.count(ss) == 0 || map[s].g + g.getCost(s, i) < map[ss].g) {
                map[ss].isGrey = true;
                map[ss].g = map[s].g + g.getCost(s, i);
                map[ss].pred = s;
                q.push({ss, map[ss].g + g.heuristic(ss)});
            }
        }
        map[s].isGrey = false;
    }
    std::cout << "The problem has no solution (number of iterations = " << nbIter << "; CPU time = "
              << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << "s)\n";
}
