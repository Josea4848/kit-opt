#include "localsearch.h"
#include "solution.h"
#include "subsequence.h"

void localSearch(Solution &s, Data &data,
                 std::vector<std::vector<SubSequence>> &subseq_matrix) {
  std::vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;

  while (NL.size()) {
    std::uniform_int_distribution<> range(0, NL.size() - 1);
    int n = range(gen);

    switch (NL[n]) {
    case 1:
      improved = bestImprovementSwap(s, data, subseq_matrix);
      break;
    case 2:
      improved = bestImprovement2Opt(s, data, subseq_matrix);
      break;
    case 3:
      improved = bestImprovementOrOpt(s, data, 1, subseq_matrix);
      break;
    case 4:
      improved = bestImprovementOrOpt(s, data, 2, subseq_matrix);
      break;
    case 5:
      improved = bestImprovementOrOpt(s, data, 3, subseq_matrix);
      break;
    }

    if (improved) {
      NL = {1, 2, 3, 4, 5};
    } else {
      NL.erase(NL.begin() + n);
    }
  }
}

bool bestImprovementSwap(Solution &s, Data &data,
                         std::vector<std::vector<SubSequence>> &subseq_matrix) {
  double best_value = s.value;
  int matrix_dimension = data.getDimension();
  int best_i, best_j;

  for (int i = 1; i < s.sequence.size() - 4; i++) {
    for (int j = i + 2; j < s.sequence.size() - 1; j++) {
      SubSequence sigma_1 = SubSequence::concatenate(subseq_matrix[0][i - 1],
                                                     subseq_matrix[j][j], data);
      SubSequence sigma_2 =
          SubSequence::concatenate(sigma_1, subseq_matrix[i + 1][j - 1], data);
      SubSequence sigma_3 =
          SubSequence::concatenate(sigma_2, subseq_matrix[i][i], data);
      SubSequence sigma_4 = SubSequence::concatenate(
          sigma_3, subseq_matrix[j + 1][subseq_matrix.size() - 1], data);

      // Sequência com menor valor
      if (sigma_4.c < best_value) {
        best_value = std::move(sigma_4.c);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_value < s.value) {
    std::swap(s.sequence[best_i], s.sequence[best_j]);
    s.value = std::move(best_value);
    updatePartialSubSeq(s, subseq_matrix, best_i, best_j, data);
    return true;
  }

  return false;
}

bool bestImprovement2Opt(Solution &s, Data &data,
                         std::vector<std::vector<SubSequence>> &subseq_matrix) {
  double best_value = s.value;
  int matrix_dimension = data.getDimension();
  int best_i, best_j;

  for (int i = 1; i < s.sequence.size() - 2; i++) {
    for (int j = i + 1; j < s.sequence.size() - 2 + (1 % i); j++) {
      SubSequence sigma1 = SubSequence::concatenate(subseq_matrix[0][i - 1],
                                                    subseq_matrix[j][i], data);
      SubSequence sigma2 = SubSequence::concatenate(
          sigma1, subseq_matrix[j + 1][matrix_dimension], data);

      if (sigma2.c < best_value) {
        best_value = std::move(sigma2.c);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_value < s.value) {
    std::reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
    s.value = std::move(best_value);
    updatePartialSubSeq(s, subseq_matrix, best_i, best_j, data);
    return true;
  }

  return false;
}

bool bestImprovementOrOpt(
    Solution &s, Data &data, int n,
    std::vector<std::vector<SubSequence>> &subseq_matrix) {
  double best_value = s.value;
  int matrix_dimension = data.getDimension();
  int best_i, best_j;

  // Faz todos os movimentos após o segmento
  for (int i = 1; i < s.sequence.size() - n - 2; i++) {
    for (int j = i + n; j < s.sequence.size() - 1; j++) {
      SubSequence sigma_1 = SubSequence::concatenate(
          subseq_matrix[0][i - 1], subseq_matrix[i + n][j], data);
      SubSequence sigma_2 =
          SubSequence::concatenate(sigma_1, subseq_matrix[i][i + n - 1], data);
      SubSequence sigma_3 = SubSequence::concatenate(
          sigma_2, subseq_matrix[j + 1][matrix_dimension], data);

      if (sigma_3.c < best_value) {
        best_value = std::move(sigma_3.c);
        best_i = i;
        best_j = j;
      }
    }
  }

  // Faz todos os movimentos antes do segmento
  for (int i = 2; i < s.sequence.size() - n - 1; i++) {
    for (int j = 0; j < i - 1; j++) {
      SubSequence sigma_1 = SubSequence::concatenate(
          subseq_matrix[0][j], subseq_matrix[i][i + n - 1], data);
      SubSequence sigma_2 =
          SubSequence::concatenate(sigma_1, subseq_matrix[j + 1][i - 1], data);
      SubSequence sigma_3 = SubSequence::concatenate(
          sigma_2, subseq_matrix[i + n][matrix_dimension], data);

      if (sigma_3.c < best_value) {
        best_value = std::move(sigma_3.c);
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_value < s.value) {
    if (best_i < best_j) {
      std::rotate(s.sequence.begin() + best_i, s.sequence.begin() + best_i + n,
                  s.sequence.begin() + best_j + 1);
      updatePartialSubSeq(s, subseq_matrix, best_i, best_j, data);

    } else {
      std::rotate(s.sequence.begin() + best_j + 1, s.sequence.begin() + best_i,
                  s.sequence.begin() + best_i + n);
      updatePartialSubSeq(s, subseq_matrix, best_j + 1, best_i + n - 1, data);
    }

    s.value = std::move(best_value);
    return true;
  }

  return false;
}
