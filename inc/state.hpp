#pragma once

#include <vector>
#include <string>

class State {
public:
    State(int nbStacks, int nbBlocs);
    State(const State& s, int j, int k);
    State();

    int getNbBlocs(int i) const;
    char getBloc(int i, int j) const;
    void print() const;
    bool operator==(const State& s) const;

    struct hash {
        std::size_t operator()(const State& s) const;
    };

private:
    std::vector<std::string> stack; // stacks of blocks
};
