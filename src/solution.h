#ifndef SOLUTION_H
#define SOLUTION_H

#include "Data.h"
#include <iostream>
#include <vector>

typedef struct Solution {
  std::vector<int> sequence;
  double value;
} Solution;

void showSolution(const Solution &s);
void calculateValue(Solution &s, Data &data);

#endif
