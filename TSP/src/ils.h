#ifndef ILS_H
#define ILS_H

#include "Data.h"
#include "construction.h"
#include "localsearch.h"
#include "seed.h"
#include "solution.h"
#include <algorithm>
#include <cmath>

Solution ILS(int maxIter, int maxIterILS, Data &data);
void perturbation(Solution& s, Data &data);

#endif
