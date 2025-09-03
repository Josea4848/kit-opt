#include "solution.h"

void showSolution(const Solution &s) {
  for (int i = 0; i < s.sequence.size() - 1; i++)
    std::cout << s.sequence[i] << " -> ";
  std::cout << s.sequence.back() << endl;
}

void calculateValue(Solution &s, Data &data) {
  s.value = 0;
  for (int i = 0; i < s.sequence.size() - 1; i++) {
    s.value += data.getDistance(s.sequence[i], s.sequence[i + 1]);
  }
}
