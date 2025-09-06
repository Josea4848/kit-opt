#include "subsequence.h"
#include "solution.h"
#include <iterator>

SubSequence concatenate(const SubSequence &sigma_1, const SubSequence &sigma_2,
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

void updateAllSubSeq(const Solution &s,
                     std::vector<std::vector<SubSequence>> &subseq_matrix,
                     Data &data) {
  int n = s.sequence.size();

  // Subsequências com um único vértice
  for (int i = 0; i < n; i++) {
    int v = s.sequence[i];
    subseq_matrix[i][i].t = 0;
    subseq_matrix[i][i].c = 0;
    subseq_matrix[i][i].w = (i > 0);
    subseq_matrix[i][i].first = v;
    subseq_matrix[i][i].last = v;
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j], data);
    }
  }

  // Subsequências invertidas
  for (int i = n - 1; i >= 0; i--) {
    for (int j = i - 1; j >= 0; j--) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j], data);
    }
  }
}

// Atualização parcial da matriz de sub-sequências
void updatePartialSubSeq(const Solution &s,
                         std::vector<std::vector<SubSequence>> &subseq_matrix,
                         int start_index, int end_index, Data &data) {
  int n = s.sequence.size();

 

  // Subsequências com um único vértice
  for (int i = start_index; i <= end_index; i++) {
    int v = s.sequence[i];
    subseq_matrix[i][i].t = 0;
    subseq_matrix[i][i].c = 0;
    subseq_matrix[i][i].w = (i > 0);
    subseq_matrix[i][i].first = v;
    subseq_matrix[i][i].last = v;
  }

  

  for (int i = 0; i <= start_index; i++) {
    for (int j = start_index; j < n; j++) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j], data);
    }
  }

  for (int i = start_index; i <= end_index; i++) {
    for (int j = i + 1; j < n; j++) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j], data);
    }
  }

  // Subsequências invertidas
  for (int i = n - 1; i > end_index; i--) {
    for (int j = end_index; j >= 0; j--) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j], data);
    }
  }

  // Subsequências invertidas
  for (int i = end_index; i >= start_index; i--) {
    for (int j = i - 1; j >= 0; j--) {
      subseq_matrix[i][j] =
          concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j], data);
    }
  }
}
