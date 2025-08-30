#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "Data.h"
#include "seed.h"
#include "solution.h"
#include <algorithm>

typedef struct InsertionInfo {
  int inserted_index;
  int removed_edge;
  double cost;
} InsertionInfo;

Solution construction(Data &data);
void chooseThreeNodes(std::vector<int> &v, Solution &s, Data &data);
std::vector<InsertionInfo>
calculateInsertion(const Solution &s, const std::vector<int> &cl, Data &data);
void insertSolution(Solution &s, InsertionInfo &selected, std::vector<int> &cl);
#endif
