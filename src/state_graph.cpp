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
    if (heuristicChoice < 0 || heuristicChoice > 4) {
        std::cout << "The heuristic choice must be in {0, 1, 2, 3, 4}\n";
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
        case 2:
            return heuristic2(s);
            break;
        case 3:
            return heuristic3(s);
            break;
        case 4:
            return heuristic4(s);
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

/**
 * @brief Heuristic about the number of blocks not on the last 
 * stack, plus twice the number of blocks b such that b is on 
 * the last stack but will have to be removed from this stack 
 * to add and/or remove other blocks under it.
 */
int StateGraph::heuristic2(const State &s) const {
    int nbBlocsLastStack = s.getNbBlocs(nbStacks-1);
    int nbBlocsNotLastStack = nbBlocs - nbBlocsLastStack;

    int nbBlocsLastStackToMove = 0;
    for (int i=0; i<nbBlocsLastStack; i++) {
        if (s.getBloc(nbStacks-1, i) != 'a'+nbBlocs-1-i) {
            // The block is not in the right place
            // this block, and any after it, will have to be moved
            nbBlocsLastStackToMove = nbBlocsLastStack - i;
            break;
        }
    }
    return nbBlocsNotLastStack + 2*nbBlocsLastStackToMove;
}

/**
 * @brief Number of blocks of Ei not on the last stack, 
 * plus the number of blocks above each block not on the 
 * last stack.
 */
int StateGraph::heuristic3(const State &s) const {
    int nbBlocsLastStack = s.getNbBlocs(nbStacks-1);
    int nbBlocsNotLastStack = nbBlocs - nbBlocsLastStack;

    int nbBlocsAbove = 0;
    for (int i=0; i<nbStacks-1; i++) {
        int nbBlocsStack = s.getNbBlocs(i);
        if (nbBlocsStack <= 1) continue;
        // each block above first one counts
        // then -1 for the next one, -2 for the next one, etc.
        // we use the formula for the sum of the first n integers
        // 1 + 2 + ... + n = n*(n+1)/2
        // NOTE: here, n = nbBlocsStack-1
        nbBlocsAbove += (nbBlocsStack*(nbBlocsStack-1))/2;
    }

    return nbBlocsNotLastStack + nbBlocsAbove;
}

/**
 * @brief Composite heuristic combining the features of heuristic1 and heuristic2 with an additional twist.
 * 
 * This heuristic aims to more accurately estimate the minimum number of moves required 
 * to reach the goal by not only considering the number of blocks not on the last stack (from heuristic1) 
 * and the blocks on the last stack that need to be moved (from heuristic2), but also by considering 
 * the arrangement of blocks on all other stacks.
 *
 * Specifically, it adds to the sum:
 * - The number of blocks not in the last stack (heuristic1).
 * - Twice the number of blocks on the last stack that need to be moved to reach the goal state (heuristic2).
 * - For each stack (except the last one), it increments the heuristic sum for every block 
 *   that is not a bigger letter than the one below it. This captures the fact that blocks
 *   that states where blocks not on the last stack are in increasing order will be easier to 
 *   reach the goal state from than states where they are not in increasing order.
 * 
 * For example, consider the following states:
 * 
 *  State 1:       State 2:
 *  3  1           4  2  
 *  4  2  0        3  1  0
 *  #  #  #        #  #  #
 *
 * One can do by hand that State 2 is 7 moves away from the goal state, while State 1 is needs much more.
 * 
 * By combining these elements, heuristic4 provides an improved estimate over heuristic1 and heuristic2 
 * individually by capturing more nuances of the block arrangement. This underestimation is important as it 
 * ensures the heuristic remains admissible, never overestimating the true cost to reach the goal, which 
 * is a critical property for A* and related algorithms to guarantee they find the shortest path.
 *
 * @param s The current state to evaluate.
 * @return An integer representing the heuristic cost estimate to reach the goal state from the given state.
 */
int StateGraph::heuristic4(const State &s) const {
    // heuristic 1
    int nbBlocsLastStack = s.getNbBlocs(nbStacks-1);
    int hSum = nbBlocs - nbBlocsLastStack;

    // heuristic 2
    int nbBlocsLastStackToMove = 0;
    for (int i=0; i<nbBlocsLastStack; i++) {
        if (s.getBloc(nbStacks-1, i) != 'a'+nbBlocs-1-i) {
            // The block is not in the right place
            // this block, and any after it, will have to be moved
            nbBlocsLastStackToMove = nbBlocsLastStack - i;
            break;
        }
    }
    hSum = hSum + 2*nbBlocsLastStackToMove;

    // heuristic 4 (tested and working alternative implementation)
    // for(int i = 0; i<nbStacks-1; i++){
    //     int maxStackLetter = 0;
    //     for(int j = 0; j<s.getNbBlocs(i); j++){
    //         if(maxStackLetter < s.getBloc(i, j)){
    //             maxStackLetter = s.getBloc(i, j);
    //         } else {
    //             hSum++;
    //         }
    //     }
    // }

    // heuristic 4, alternative implementation
    // count the number of misplaced blocks on all stacks except the last one
    // WARN: check that the nb of stacks goes well from 0 to nbStacks-2 INCLUDED
    for (int i = 0; i <= nbStacks-2; i++) {
        for (int j = 0; j < s.getNbBlocs(i); j++) {
            if (s.getBloc(i, j) > s.getBloc(i, j+1)) {
                hSum++;
            }
        }
    }

    return hSum;
}

