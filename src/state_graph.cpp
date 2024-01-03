#include "state_graph.hpp"
#include <iostream>

/** 
 * @brief Constructs a bloc world planning problem.
 */
StateGraph::StateGraph() {
    std::cout << "Enter the number of stacks: ";
    std::cin >> nbStacks;
    std::cout << "Enter the number of blocs: ";
    std::cin >> nbBlocs;
    if (nbStacks <= 0 || nbBlocs <= 0) {
        std::cout << "The number of stacks and the number of blocs must be positive numbers\n";
        exit(0);
    }

    // Which heuristic to use?
    std::cout << "+----- Which heuristic do you want to use?\n";
    std::cout << "+- 0. Default heuristic (always 0, useless) \n";
    std::cout << "+- 1. Number of blocks not in the last stack\n";

    int heuristicChoice;
    std::cin >> heuristicChoice;
    if (heuristicChoice < 0 || heuristicChoice > 1) {
        std::cout << "The heuristic choice must be 0 or 1\n";
        exit(0);
    }
    this->selectedHeuristic = heuristicChoice;
}

/**
 * @brief Construcs a bloc world planning problem with nbStacks stacks and nbBlocs blocs
 * @param nbStacks The number of stacks
 * @param nbBlocs The number of blocs
 */
StateGraph::StateGraph(
    int nbStacks, int nbBlocs, int selectedHeuristic
) : nbStacks(nbStacks), nbBlocs(nbBlocs), selectedHeuristic(selectedHeuristic) {}

/**
 * @brief Destroys the bloc world planning problem
 */
StateGraph::~StateGraph() {}

/**
 * @brief Get the initial state
 * @return The initial state
 */
State StateGraph::initialState() const {
    return State(nbStacks, nbBlocs);
}

/**
 * @brief Check if a state is final
 * @param s The state to check
 * @return True if s is a final state, false otherwise
 */
bool StateGraph::isFinal(const State &s) const {
    for (int i=0; i<nbStacks-1; i++)
        if (s.getNbBlocs(i)>0) return false;
    for (int i=0; i<nbBlocs; i++)
        if (s.getBloc(nbStacks-1, i) != 'a'+nbBlocs-1-i) return false;
    return true;
}

/** 
 * @brief Search all possible actions from state s
 * @param s The state from which the search is performed
 * @return The number of possible actions
 */
int StateGraph::searchActions(const State &s) {
    actions.clear();
    for (int i=0; i<nbStacks; i++) {
        if (s.getNbBlocs(i)>0) {
            for (int j=0; j<nbStacks; j++) {
                if (i != j) actions.push_back(i*nbStacks + j);
            }
        }
    }
    return actions.size();
}

/** 
 * @brief Compute the heuristic value of state s
 * @param s The state for which the heuristic value is computed
 * @return The heuristic value of state s
 */
int StateGraph::heuristic(const State &s) const {
    switch (this->selectedHeuristic) {
        case 1:
            return heuristic1(s);
            break;
        default:
            return defaultHeuristic(s);
            break;
    }
}

/** 
 * @brief Apply the i-th action to state s
 * @param s The state to which the action is applied
 * @param i The index of the action to apply
 * @return The successor state
 * @precondition 0<=i<searchActions(s)
 */
State StateGraph::transition(const State &s, int i) {
    
    return State(s, actions[i]/nbStacks, actions[i]%nbStacks);
}

/** 
 * @brief Get the cost of the i-th action from state s
 * @param s The state from which the action is applied
 * @param i The index of the action
 * @return The cost of the action
 * @precondition 0<=i<searchActions(s)
 */
int StateGraph::getCost(const State &s, int i) const {
    return 1;
}

/** 
 * @brief Print the transition from state s to state s_succ
 * @param s The state from which the transition is performed
 * @param s_succ The successor state
 */
void StateGraph::print(const State &s, const State &s_succ) {
    static State s0 = initialState();
    if (s == s0) {
        printf("Init: ");
        s.print();
    }
    // Print the action that has been used to go from s to s_succ
    for (int i=0; i<nbStacks; i++) {
        if (s.getNbBlocs(i)>0) {
            for (int j=0; j<nbStacks; j++) {
                if (i != j && State(s, i, j) == s_succ) {
                    printf("%d->%d: ", i, j);
                    s_succ.print();
                    return;
                }
            }
        }
    }
}

/** 
 * @brief Set the heuristic function
 * @param h The heuristic function
 */
void StateGraph::setHeuristic(int selectedHeuristic) {
    this->selectedHeuristic = selectedHeuristic;
}

/**
 * @brief Default heuristic function. Always returns 0.
 */
int StateGraph::defaultHeuristic(const State &s) const {
    return 0;
}

/**
 * @brief Heuristic function that returns the number of blocks not in the last stack.
 */
int StateGraph::heuristic1(const State &s) const {
    int nbBlocsLastStack = s.getNbBlocs(nbStacks-1);
    return nbBlocs - nbBlocsLastStack;
}



