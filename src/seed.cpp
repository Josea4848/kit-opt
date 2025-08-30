#include "seed.h"

std::mt19937 gen;

void genSeed() {
  std::random_device rd;
  gen.seed(rd());
};
