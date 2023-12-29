#include "state.hpp"
#include <iostream>

State::State(int nbStacks, int nbBlocs) {
    stack = std::vector<std::string>(nbStacks, "");
    for (int j = 0; j < nbBlocs; j++) stack[j % nbStacks] += 'a' + j;
}

State::State(const State& s, int j, int k) {
    int nbStacks = s.stack.size();
    stack = std::vector<std::string>(nbStacks);
    for (int i = 0; i < nbStacks; i++) {
        if (i == j) stack[i] = s.stack[i].substr(0, s.stack[i].length() - 1);
        else if (i == k) stack[i] = s.stack[i] + s.stack[j].substr(s.stack[j].length() - 1, 1);
        else stack[i] = s.stack[i];
    }
}

State::State() {}

int State::getNbBlocs(int i) const {
    return stack[i].length();
}

char State::getBloc(int i, int j) const {
    return stack[i][j];
}

void State::print() const {
    for (std::size_t k = 0; k < stack.size(); k++) {
        std::cout << "stack[" << k << "]=" << stack[k] << " ";
    }
    std::cout << "\n";
}

std::size_t State::hash::operator()(const State& s) const {
    std::string st = s.stack[0];
    for (std::size_t i = 1; i < s.stack.size(); i++) st += " " + s.stack[i];
    return std::hash<std::string>{}(st);
}

bool State::operator==(const State& s) const {
    return stack == s.stack;
}
