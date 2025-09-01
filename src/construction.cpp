#include "construction.h"
#include "solution.h"
#include <algorithm>
#include <random>

// Constrói solução inicial
void construction(Solution &s, Data &data) {
  int dimension = data.getDimension();

  s.value = 0;
  s.sequence.reserve(dimension + 1);

  std::vector<int> v, cl;
  // Inicializando vector com todos os vértices que podem ser escolhidos
  for (int i = 2; i <= dimension; i++) {
    v.push_back(i);
  }

  // Gera sequência inicial e remove do conjunto inicial
  chooseThreeNodes(v, s, data);
  cl = std::move(v);

  // Enquanto houver nós restantes
  while (cl.size()) {
    std::vector<InsertionInfo> insertions = calculateInsertion(s, cl, data);
    std::uniform_int_distribution<> range(0, insertions.size() - 1);
    std::sort(insertions.begin(), insertions.end(),
              [](const InsertionInfo &a, const InsertionInfo &b) {
                return a.cost < b.cost;
              });
    int selected = range(gen);
    insertSolution(s, insertions[selected], cl);
  }

}

// Inicializa sequência com nó fixo e três nós escolhidos aleatoriamente
void chooseThreeNodes(std::vector<int> &v, Solution &s, Data &data) {
  int rd_index, node;
  s.sequence.push_back(1);

  // Faz a escolha dos três nós
  for (int i = 0; i < 3; i++) {
    uniform_int_distribution<> range(0, v.size() - 1);
    rd_index = range(gen);
    s.sequence.push_back(v[rd_index]);
    std::iter_swap(v.begin() + rd_index, v.end() - 1);
    s.value += data.getDistance(s.sequence[i], s.sequence[i + 1]);
    v.pop_back();
  }

  s.sequence.push_back(1);
  s.value += data.getDistance(s.sequence[s.sequence.size() - 2],
                              s.sequence[s.sequence.size() - 1]);
}

// Calcula todas as possíveis inserções
std::vector<InsertionInfo>
calculateInsertion(const Solution &s, const std::vector<int> &CL, Data &data) {
  std::vector<InsertionInfo> insertions((s.sequence.size() - 1) * CL.size());
  int l = 0;

  for (int a = 0; a < s.sequence.size() - 1; a++) {
    int i = s.sequence[a];
    int j = s.sequence[a + 1];

    for (int b = 0; b < CL.size(); b++) {
      int k = CL[b];
      insertions[l].cost = data.getDistance(k, i) + data.getDistance(k, j) -
                           data.getDistance(i, j);
      insertions[l].inserted_index = b;
      insertions[l].removed_edge = a;
      ++l;
    }
  }

  return insertions;
}

// Insere novo nó na solução e remove do conjunto CL
void insertSolution(Solution &s, InsertionInfo &selected,
                    std::vector<int> &cl) {
  s.sequence.insert(s.sequence.begin() + selected.removed_edge + 1,
                    cl[selected.inserted_index]);
  s.value += selected.cost;
  std::iter_swap(cl.begin() + selected.inserted_index, cl.end() - 1);
  cl.pop_back();
}
