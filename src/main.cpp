#include "construction.h"
#include "localsearch.h"
#include "seed.h"
#include "solution.h"
#include <chrono>
#include <iostream>

int main(int argc, char **argv) {
  // Gerando seed
  genSeed();

  // Lendo dados de instância
  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();

  auto start = chrono::high_resolution_clock::now();

  // Inicializando estrutura da solução
  Solution s = construction(data);

  showSolution(s);
  cout << "custo antes: " << s.value << endl;
  // bestImprovementSwap(s, data);

  bestImprovement2Opt(s, data);

  cout << "custo dps: " << s.value << endl;

  showSolution(s);

  calculateValue(s, data);

  cout << "custo dps do dps: " << s.value << endl;

  // Créditos a samuca pelo benchmark
  auto stop = chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << data.getInstanceName();
  printf(" - %.3lf\n", (double)(duration.count()) / 10000);

  return 0;
}
