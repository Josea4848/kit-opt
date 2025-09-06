#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Data.h"
#include "seed.h"
#include "solution.h"
#include "subsequence.h"
#include <algorithm>

bool bestImprovementSwap(Solution &s, Data &data,
                         std::vector<std::vector<SubSequence>> &subseq_matrix);
bool bestImprovement2Opt(Solution &s, Data &data,
                         std::vector<std::vector<SubSequence>> &subseq_matrix);
bool bestImprovementOrOpt(Solution &s, Data &data, int n,
                          std::vector<std::vector<SubSequence>> &subseq_matrix);
void localSearch(Solution &s, Data &data,
                 std::vector<std::vector<SubSequence>> &subseq_matrix);

#endif
