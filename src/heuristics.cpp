#include "heuristics.hpp"

int defaultHeuristic(const State &s) {
    return 0;
}

// Number of blocks not in the last stack
int heuristic1(const State &s) {
    int nbBlocs = s.getNbBlocs(0);
    int nbStacks = s.getNbBlocs(1);
    int nbBlocsLastStack = s.getNbBlocs(nbStacks - 1);
    return nbBlocs - nbBlocsLastStack;
}

