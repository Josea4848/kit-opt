#include "ils.h"
#include "localsearch.h"
#include "seed.h"
#include "solution.h"
#include <algorithm>
#include <cmath>
#include <random>

Solution ILS(int maxIter, int maxIterILS, Data &data) {
  Solution bestOfAll;
  bestOfAll.value = INFINITY;

  for (int i = 0; i < maxIter; i++) {
    int iterILS = 0;

    // Geração de soluções
    Solution s; 
    construction(s, data);
    Solution best = s;

    while (iterILS < maxIterILS) {
      localSearch(s, data);

      // Houve melhora após pertubação + busca local
      if (s.value < best.value) {
        best = std::move(s);
        iterILS = 0;
      }

      s = best;
      perturbation(s, data);
      iterILS++;
    }

    if (best.value < bestOfAll.value) {
      bestOfAll = std::move(best);
    }
  }

  return bestOfAll;
}

void perturbation(Solution &s, Data &data) {
  int dimension = data.getDimension();
  int i_start, i_end, j_start, j_end, i_size, j_size;
  int vi_first, vi_last, vi_prev, vi_next, vj_first, vj_last, vj_prev, vj_next;

  std::uniform_int_distribution<> index_dist(1, s.sequence.size() - 3);

  // Primeiro segmento
  i_start = index_dist(gen);
  std::uniform_int_distribution<> size_dist(
      2,
      std::min(dimension - i_start, (int)std::ceil((double)dimension / 10.0)));

  i_size = size_dist(gen);
  i_end = i_size + i_start - 1;

  while (true) {
    // Segundo segmento
    j_start = index_dist(gen);
    j_size = size_dist(gen);
    j_end = j_size + j_start - 1;

    // Será necessário gerar novos segmentos
    if (j_end < s.sequence.size() - 1 && (j_start > i_end + 1 || j_end < i_start - 1))
      break;
  }

  vi_first = s.sequence[i_start];
  vi_last = s.sequence[i_end];
  vi_prev = s.sequence[i_start - 1];
  vi_next = s.sequence[i_end + 1];

  vj_first = s.sequence[j_start];
  vj_last = s.sequence[j_end];
  vj_prev = s.sequence[j_start - 1];
  vj_next = s.sequence[j_end + 1];

  // Segmento i antes do segmento j
  if (i_end < j_start) {
    std::rotate(s.sequence.begin() + i_start, s.sequence.begin() + i_end + 1,
                s.sequence.begin() + j_end + 1);
    std::rotate(s.sequence.begin() + i_start,
                s.sequence.begin() + j_start - i_size,
                s.sequence.begin() + j_end - i_size + 1);
  } else if (j_end < i_start) {
    std::rotate(s.sequence.begin() + j_start, s.sequence.begin() + j_end + 1,
                s.sequence.begin() + i_end + 1);
    std::rotate(s.sequence.begin() + j_start,
                s.sequence.begin() + i_start - j_size,
                s.sequence.begin() + i_end - j_size + 1);
  }


  s.value += data.getDistance(vj_first, vi_prev) +
               data.getDistance(vj_last, vi_next) +
               data.getDistance(vi_first, vj_prev) +
               data.getDistance(vi_last, vj_next) -
               data.getDistance(vi_first, vi_prev) -
               data.getDistance(vi_last, vi_next) -
               data.getDistance(vj_first, vj_prev) -
               data.getDistance(vj_last, vj_next);
  
}
