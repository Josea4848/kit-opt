#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Data.h"
#include "seed.h"
#include "solution.h"
#include <algorithm>

bool bestImprovementSwap(Solution &s, Data &data);
bool bestImprovement2Opt(Solution &s, Data &data);
bool bestImprovementOrOpt(Solution &s, Data &data, int n);
void localSearch(Solution &s, Data &data);

#endif
