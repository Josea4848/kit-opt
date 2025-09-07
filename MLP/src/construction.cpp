#include "construction.h"
#include "solution.h"

// Constrói solução inicial
void construction(Solution &s, Data &data) {
  int dimension = data.getDimension();
  double delay_sum = 0;

  // Escolhendo alpha aleatório
  std::uniform_int_distribution<> range(0, 25);
  double alpha = range(gen) / 100.0;

  // Inicializando solução com primeiro nó
  s.value = 0;
  s.sequence.reserve(dimension + 1);
  s.sequence.push_back(1);

  std::vector<int> cl;

  // Inicializando vector com todos os vértices que podem ser escolhidos
  for (int i = 2; i <= dimension; i++) {
    cl.push_back(i);
  }

  int r = 1;
  // Enquanto houver nós restantes
  while (cl.size()) {
    std::sort(cl.begin(), cl.end(), [r, &data](int a, int b) {
      return data.getDistance(r, a) < data.getDistance(r, b);
    });
    std::uniform_int_distribution<> rcl_range(0, alpha * (double)cl.size());
    int random_index = rcl_range(gen);
    int c = cl[random_index];
    r = c;
    // Inserindo na solução e removendo de CL
    delay_sum += data.getDistance(s.sequence.back(), c);

    s.sequence.push_back(c);
    s.value += delay_sum;
    std::iter_swap(cl.begin() + random_index, cl.end() - 1);
    cl.pop_back();
  }

  s.value += delay_sum + data.getDistance(s.sequence.back(), 1);
  s.sequence.push_back(1);
}
