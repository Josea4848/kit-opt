#include "ils.h"
#include "solution.h"
#include "subsequence.h"

Solution ILS(int maxIter, int maxIterILS, Data &data) {
  Solution bestOfAll;
  bestOfAll.value = INFINITY;
  int n = data.getDimension();
  std::vector<std::vector<SubSequence>> subseq_matrix(
      n + 1, std::vector<SubSequence>(n + 1));

  for (int i = 0; i < maxIter; i++) {
    int iterILS = 0;

    // Geração de soluções
    Solution best;
    construction(best, data);
    updateAllSubSeq(best, subseq_matrix, data);

    // Primeiro busca local em cima da solução inicial
    localSearch(best, data, subseq_matrix);

    while (iterILS < maxIterILS) {
      Solution s = best;
      perturbation(s, subseq_matrix, data);
      localSearch(s, data, subseq_matrix);

      // houve melhora após pertubação + busca local
      if (s.value < best.value) {
        best = s;
        iterILS = 0;
      } else
        updateAllSubSeq(best, subseq_matrix, data);
      iterILS++;
    }

    if (best.value < bestOfAll.value) {
      bestOfAll = std::move(best);
    }
  }

  return bestOfAll;
}

void perturbation(Solution &s,
                  std::vector<std::vector<SubSequence>> &subseq_matrix,
                  Data &data) {
  int dimension = data.getDimension();
  int i_start, i_end, j_start, j_end, i_size, j_size;
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
    if (j_end < s.sequence.size() - 1 &&
        (j_start > i_end + 1 || j_end < i_start - 1))
      break;
  }

  double new_value;

  // Segmento i antes do segmento j
  if (i_end < j_start) {
    SubSequence sigma_1 = SubSequence::concatenate(
        subseq_matrix[0][i_start - 1], subseq_matrix[j_start][j_end], data);
    sigma_1 = SubSequence::concatenate(
        sigma_1, subseq_matrix[i_end + 1][j_start - 1], data);
    SubSequence sigma_2 = SubSequence::concatenate(
        subseq_matrix[i_start][i_end],
        subseq_matrix[j_end + 1][subseq_matrix.size() - 1], data);
    SubSequence sigma_3 = SubSequence::concatenate(sigma_1, sigma_2, data);
    new_value = sigma_3.c;

    std::rotate(s.sequence.begin() + i_start, s.sequence.begin() + i_end + 1,
                s.sequence.begin() + j_end + 1);
    std::rotate(s.sequence.begin() + i_start,
                s.sequence.begin() + j_start - i_size,
                s.sequence.begin() + j_end - i_size + 1);
    updatePartialSubSeq(s, subseq_matrix, i_start, j_end, data);

  } else if (j_end < i_start) {
    SubSequence sigma_1 = SubSequence::concatenate(
        subseq_matrix[0][j_start - 1], subseq_matrix[i_start][i_end], data);
    sigma_1 = SubSequence::concatenate(
        sigma_1, subseq_matrix[j_end + 1][i_start - 1], data);
    SubSequence sigma_2 = SubSequence::concatenate(
        subseq_matrix[j_start][j_end],
        subseq_matrix[i_end + 1][subseq_matrix.size() - 1], data);
    SubSequence sigma_3 = SubSequence::concatenate(sigma_1, sigma_2, data);
    new_value = sigma_3.c;

    std::rotate(s.sequence.begin() + j_start, s.sequence.begin() + j_end + 1,
                s.sequence.begin() + i_end + 1);
    std::rotate(s.sequence.begin() + j_start,
                s.sequence.begin() + i_start - j_size,
                s.sequence.begin() + i_end - j_size + 1);
    updatePartialSubSeq(s, subseq_matrix, j_start, i_end, data);
  }

  s.value = new_value;
}
