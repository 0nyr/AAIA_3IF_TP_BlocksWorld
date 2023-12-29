#include "astar.hpp"

#include <iostream>
#include <ctime>

bool AStar::Compare::operator()(std::pair<State, int> const &s1, std::pair<State, int> const &s2) {
    return s1.second > s2.second;
}

void AStar::printSolution(StateGraph &g, const State &s0, const State &s, const HashMap &map) {
    if (!(s == s0)) {
        printSolution(g, s0, map.at(s).pred, map);
        g.print(map.at(s).pred, s);
    }
}

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
