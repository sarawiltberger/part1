#ifndef PUZZLE_8_SOLVER_H
#define PUZZLE_8_SOLVER_H

#include <string>
#include <vector>
#include <set>
#include "Puzzle8State.h"

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions);

std::vector<std::string> getBranches(std::string);
std::pair<int, int> stringIndexToPair(int stringIndex);
int pairToStringIndex(std::pair<int, int> indexPair);
std::string switch0AndChar(std::string state, int index1, int index2);
int findCost(std::shared_ptr<Puzzle8State> goalPuzzle);

#endif
