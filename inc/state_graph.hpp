#pragma once

#include <vector>
#include "state.hpp"

class StateGraph {
public:
    // constructors
    StateGraph();
    StateGraph(int nbStacks, int nbBlocs, int selectedHeuristic);

    // destructor
    ~StateGraph();

    // methods
    State initialState() const;
    bool isFinal(const State &s) const;
    int searchActions(const State &s);
    int getCost(const State &s, int i) const;
    State transition(const State &s, int i);
    void print(const State &s, const State &s_succ);

    int heuristic(const State &s) const;

    // setters
    void setHeuristic(int selectedHeuristic);

    // getters
    int getNbStacks() const;
    int getNbBlocs() const;

private:
    std::vector<int> actions;
    int nbBlocs; // Number of blocs
    int nbStacks; // Number of stacks
    int selectedHeuristic; // Heuristic function

    // private methods
    // WARN: when adding a new heuristic, don't forget to add it to the switch in heuristic()
    int defaultHeuristic(const State &s) const;
    int heuristic1(const State &s) const;
    int heuristic2(const State &s) const;
    int heuristic3(const State &s) const;
    int heuristic4(const State &s) const;
    int heuristic5(const State &s) const;
};
