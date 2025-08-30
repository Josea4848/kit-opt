#include "construction.h"
#include "seed.h"
#include "solution.h"
#include <iostream>

int main(int argc, char **argv) {
  // Gerando seed
  genSeed();

  // Lendo dados de instância
  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();

  // Inicializando estrutura da solução
  Solution s = construction(data);

  showSolution(s);

  cout << s.value << endl;

  calculateValue(s, data);

  cout << s.value << endl;

  return 0;
}
