#include "ils.h"
#include "seed.h"
#include "solution.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define NUM_THREADS 5

typedef struct result {
  double value;
  double time;
} result;

std::mutex mtx;
std::vector<result> results;

void executeILS(char *path, int qtd_arg) {
  // Lendo dados de instância
  auto data = Data(qtd_arg, path);
  data.read();
  int n = data.getDimension();
  int maxIter = 10;
  int maxIterILS = std::min(100, n);

  auto start = std::chrono::high_resolution_clock::now();
  Solution s = ILS(maxIter, maxIterILS, data);
  auto stop = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)
          .count() /
      1000.0;
  mtx.lock();
  results.push_back({s.value, duration});
  mtx.unlock();
}

int main(int argc, char **argv) {
  std::fstream file;
  double sum_time = 0, sum_value = 0;

  // Gerando seed
  genSeed();

  // Lendo dados de instância
  auto data = Data(argc, argv[1]);
  data.read();
  int n = data.getDimension();
  int maxIter = 10;
  int maxIterILS = std::min(100, n);

  // Criando e executando threads
  for (int k = 0; k < 2; k++) {
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
      threads.emplace_back(executeILS, argv[1], argc);
    }

    for (auto &t : threads)
      t.join();
  }

  for (auto &r : results) {

    cout << "Valor: " << r.value << endl;
    cout << "TEMPO: " << r.time << endl;
    sum_time += r.time;
    sum_value += r.value;
  }

  // Benchmark
  file.open("Benchmark/benchmark_10_final.txt", std::ios::app);
  if (!file.is_open()) {
    std::cerr << "Erro ao abrir o arquivo\n";
    return -1;
  }

  file << "\nInstância: " << data.getInstanceName() << std::endl;
  file << "Média valor: " << sum_value / (double)NUM_THREADS / 2.0 << std::endl;
  file << std::fixed << std::setprecision(10);
  file << "Média tempo: " << sum_time / (double)NUM_THREADS / 2.0 << std::endl;

  file.close();
  return 0;
}
