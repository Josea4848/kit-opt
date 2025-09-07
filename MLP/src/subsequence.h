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

  inline static SubSequence concatenate(const SubSequence &sigma_1,
                                        const SubSequence &sigma_2,
                                        Data &data) {
    SubSequence sigma;
    double temp = data.getDistance(sigma_1.last, sigma_2.first);

    sigma.w = sigma_1.w + sigma_2.w;
    sigma.t = sigma_1.t + temp + sigma_2.t;
    sigma.c = sigma_1.c + sigma_2.w * (sigma_1.t + temp) + sigma_2.c;
    sigma.first = sigma_1.first;
    sigma.last = sigma_2.last;

    return sigma;
  }
} SubSequence;

void updateAllSubSeq(const Solution &s,
                     std::vector<std::vector<SubSequence>> &subseq_matrix,
                     Data &data);
void updatePartialSubSeq(const Solution &s,
                         std::vector<std::vector<SubSequence>> &subseq_matrix,
                         int start_index, int end_index, Data &data);
#endif
