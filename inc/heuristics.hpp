#pragma once

#include <functional>

#include "state.hpp"

typedef std::function<int( const State & s )> Heuristic;

// all the heuristics
int defaultHeuristic(const State &s);
int heuristic1(const State &s);