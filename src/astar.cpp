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
    map[s0].cost = 0;
    map[s0].isGrey = true;
    q.push({s0, g.heuristic(s0)});
    while (!q.empty()) {
        State s = q.top().first;
        q.pop();
        if (!map[s].isGrey) continue;
        nbIter++;
        if (g.isFinal(s)) {
            std::cout << "Optimal solution of length " << map.at(s).cost << " found in " << nbIter << " iterations and "
                      << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
            //printSolution(g, s0, s, map);
            return;
        }
        int nbActions = g.searchActions(s);
        for (int i = 0; i < nbActions; i++) {
            State ss = g.transition(s, i);
            if (map.count(ss) == 0 || map[s].cost + g.getCost(s, i) < map[ss].cost) {
                map[ss].isGrey = true;
                map[ss].cost = map[s].cost + g.getCost(s, i);
                map[ss].pred = s;
                q.push({ss, map[ss].cost + g.heuristic(ss)});
            }
        }
        map[s].isGrey = false;
    }
    std::cout << "The problem has no solution (number of iterations = " << nbIter << "; CPU time = "
              << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << "s)\n";
}

/**
 * @brief Implements the AWA* search algorithm.
 * 
 * AWA* is a variant of A* that uses a weighted heuristic to find a solution
 * faster than A* at the cost of optimality. The weight is a parameter that
 * can be tuned to find a balance between optimality and speed. It basically 
 * increases the importance of the heuristic in the cost function.
 * 
 * The weight needs to be greater or equal to 1 for the algorithm to be admissible.
 * When the weight is equal to 1, AWA* is equivalent to A*.
 * 
 * @param s0 The initial state from where the search begins.
 * @param g The state graph containing all possible states and transitions.
 * @param weight The weight of the heuristic. Must be greater or equal to 1.
 */
void AStar::awa_star(
    const State &s0, StateGraph &g, int weight
) {
    clock_t start = clock();

    int nbIter = 0;
    // Initialize upper bound to "infinity", i.e. the max int value
    // WARN: Do NOT use std::numeric_limits<int>::infinity() here
    // because it gives 0 since infinity is not a concept in integers
    int upperBound = std::numeric_limits<int>::max();
    HashMap map;
    PriorityQueue q;

    // initializations
    map[s0].cost = 0;
    map[s0].isGrey = true;
    q.push({s0, g.heuristic(s0)});

    std::cout << "starting upper bound = " << upperBound << "\n";

    while (!q.empty()) {
        State s = q.top().first;
        q.pop();
        if (!map[s].isGrey) {
            continue; // ignore already visited states
        }
        nbIter++;

        // in AWA*, removed check if the state is final here

        int nbActions = g.searchActions(s);
        for (int i = 0; i < nbActions; i++) {
            State ss = g.transition(s, i);
            if (
                (map.count(ss) == 0) || 
                (map[s].cost + g.getCost(s, i) < map[ss].cost)
            ) {
                map[ss].isGrey = true;
                map[ss].cost = map[s].cost + g.getCost(s, i);
                map[ss].pred = s;

                // AWA* specific
                if (g.isFinal(ss)) {
                    upperBound = map.at(ss).cost; // update upper bound
                    std::cout << "Found solution of length " 
                        << map.at(ss).cost << " in " << nbIter 
                        << " iterations and "
                        << static_cast<double>(clock() - start) / CLOCKS_PER_SEC << " seconds, upper bound = " 
                        << upperBound << ", weight = "
                        << weight << "\n";
                    // DO NOT Return
                } else if (
                    (map.at(ss).cost + g.heuristic(ss)) < upperBound
                ) {
                    q.push(
                        {
                            ss, 
                            map[ss].cost + (weight * g.heuristic(ss))
                        }
                    );
                } 
            }
        }
        map[s].isGrey = false; // set state to visited (black)
    }

    if (upperBound == std::numeric_limits<int>::infinity()) {
        std::cout << "No solution could be found with current input"
            << " parameters (weight = " << weight
            << ", nb of iterations = " << nbIter 
            << ", enlapsed time = " 
            << ((double) (clock() - start)) / CLOCKS_PER_SEC 
            << "s)\n";
    } else {
        std::cout << "Optimal solution of length " << upperBound 
            << " found in " << nbIter << " iterations and "
            << static_cast<double>(clock() - start) / CLOCKS_PER_SEC 
            << " seconds, weight = " << weight << "\n";
    }
}
