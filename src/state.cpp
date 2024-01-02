#include "state.hpp"
#include <iostream>

/**
 * @brief Constructs a state with nbStacks stacks and nbBlocs blocs
 */
State::State(int nbStacks, int nbBlocs) {
    stack = std::vector<std::string>(nbStacks, "");
    for (int j = 0; j < nbBlocs; j++) stack[j % nbStacks] += 'a' + j;
}

/**
 * @brief Constructs a state from a previous state s by moving 
 * the bloc on top of stack j to the top of stack k
 * 
 * @param s The previous state
 * @param j The stack from which the bloc is moved
 * @param k The stack to which the bloc is moved
*/
State::State(const State& s, int j, int k) {
    int nbStacks = s.stack.size();
    stack = std::vector<std::string>(nbStacks);
    for (int i = 0; i < nbStacks; i++) {
        if (i == j) stack[i] = s.stack[i].substr(0, s.stack[i].length() - 1);
        else if (i == k) stack[i] = s.stack[i] + s.stack[j].substr(s.stack[j].length() - 1, 1);
        else stack[i] = s.stack[i];
    }
}

/**
 * @brief Constructs an empty state
 */
State::State() {}

/**
 * @brief Get the number of blocs on stack i
 * @param i The stack
 * @return The number of blocs on stack i
 */
int State::getNbBlocs(int i) const {
    return stack[i].length();
}

/**
 * @brief Get j-th bloc on stack i
 * @param i The stack
 * @param j The bloc's index on stack i
 * @return The j-th bloc on stack i
*/
char State::getBloc(int i, int j) const {
    return stack[i][j];
}

/**
 * @brief Print the state with its stacks
 */
void State::print() const {
    for (std::size_t k = 0; k < stack.size(); k++) {
        std::cout << "stack[" << k << "]=" << stack[k] << " ";
    }
    std::cout << "\n";
}

/**
 * @brief Get the hash of a state
 * @param s The state
 * @return The hash of the state
 */
std::size_t State::hash::operator()(const State& s) const {
    std::string st = s.stack[0];
    for (std::size_t i = 1; i < s.stack.size(); i++) st += " " + s.stack[i];
    return std::hash<std::string>{}(st);
}

/**
 * @brief Check if two states are equal
 * @param s The state to compare with
 * @return True if the two states are equal, false otherwise
 */
bool State::operator==(const State& s) const {
    return stack == s.stack;
}
