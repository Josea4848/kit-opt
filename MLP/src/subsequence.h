#ifndef SUBSEQUENCE_H
#define SUBSEQUENCE_H
#include "Data.h"
#include "solution.h"
#include <memory.h>
#include <memory>
#include <vector>

typedef struct SubSequence {
  double t, c;
  int w;
  int first, last;
} SubSequence;

SubSequence concatenate(const SubSequence &sigma_1, const SubSequence &sigma_2,
                        Data &data);
void updateAllSubSeq(const Solution &s,
                     std::vector<std::vector<SubSequence>> &subseq_matrix,
                     Data &data);
void updatePartialSubSeq(const Solution &s,
                         std::vector<std::vector<SubSequence>> &subseq_matrix,
                         int start_index, int end_index, Data &data);
#endif
