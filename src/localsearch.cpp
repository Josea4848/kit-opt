#include "localsearch.h"

bool bestImprovementSwap(Solution &s, Data &data) {
  double best_delta = 0;
  int best_i, best_j;

  for (int i = 1; i < s.sequence.size() - 4; i++) {
    int vi = s.sequence[i];
    int vi_next = s.sequence[i + 1];
    int vi_prev = s.sequence[i - 1];

    for (int j = i + 2; j < s.sequence.size() - 1; j++) {
      int vj = s.sequence[j];
      int vj_next = s.sequence[j + 1];
      int vj_prev = s.sequence[j - 1];

      double delta =
          data.getDistance(vj, vi_prev) + data.getDistance(vj, vi_next) +
          data.getDistance(vi, vj_prev) + data.getDistance(vi, vj_next) -
          data.getDistance(vi, vi_prev) - data.getDistance(vi, vi_next) -
          data.getDistance(vj, vj_prev) - data.getDistance(vj, vj_next);

      if (delta < best_delta) {
        best_delta = std::move(delta);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_delta < 0) {
    std::swap(s.sequence[best_i], s.sequence[best_j]);
    s.value += best_delta;
    return true;
  }

  return false;
}

bool bestImprovement2Opt(Solution &s, Data &data) {
  double best_delta = 0;
  int best_i, best_j;

  for (int i = 1; i < s.sequence.size() - 2; i++) {
    int vi = s.sequence[i];
    int vi_prev = s.sequence[i - 1];

    for (int j = i + 1; j < s.sequence.size() - 2 + (1 % i); j++) {
      int vj = s.sequence[j];
      int vj_next = s.sequence[j + 1];

      double delta =
          data.getDistance(vj, vi_prev) + data.getDistance(vi, vj_next) -
          data.getDistance(vi, vi_prev) - data.getDistance(vj, vj_next);

      if (delta < best_delta) {
        best_delta = std::move(delta);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_delta < 0) {
    std::reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
    s.value += best_delta;
    return true;
  }

  return false;
}

bool bestImprovementOrOpt(Solution &s, Data &data, int n) {
  double best_delta = 0;
  int best_i, best_j;

  for (int i = 1; i < s.sequence.size() - n; i++) {
    int vi_first = s.sequence[i];
    int vi_last = s.sequence[i + n - 1];
    int vi_prev = s.sequence[i - 1];
    int vi_next = s.sequence[i + n];

    std::cout << "i = " << i << ": ";

    for (int j = !(i - 1) * (n + 1); j < s.sequence.size() - 1;
         j += 1 + !(i - j - 2) * (n + 1)) {
      int vj_prev = s.sequence[j];
      int vj_next = s.sequence[j + 1];

      double delta = data.getDistance(vi_first, vj_prev) +
                     data.getDistance(vi_last, vj_next) +
                     data.getDistance(vi_prev, vi_next) -
                     data.getDistance(vi_first, vi_prev) -
                     data.getDistance(vi_last, vi_next) -
                     data.getDistance(vj_prev, vj_next);

      if (delta < best_delta) {
        best_delta = std::move(delta);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_delta < 0) {
    if (best_i < best_j) {
      std::rotate(s.sequence.begin() + best_i, s.sequence.begin() + best_i + n,
                  s.sequence.begin() + best_j + 1);
    } else {
      std::rotate(s.sequence.begin() + best_j + 1, s.sequence.begin() + best_i,
                  s.sequence.begin() + best_i + n);
    }
    s.value += best_delta;
    return true;
  }

  return false;
}
