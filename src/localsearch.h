#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Data.h"
#include "solution.h"
#include <algorithm>

bool bestImprovementSwap(Solution &s, Data &data);
bool bestImprovement2Opt(Solution &s, Data &data);

#endif
