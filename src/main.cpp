#include "ils.h"
#include "seed.h"
#include "solution.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#define NUM_THREADS 10

// typedef struct result {
//     double value;
//     double time;
// } result;

// // Função que retorna o resultado ao invés de usar mutex
// result executeILS(Data &data, int maxIter, int maxIterILS) {
//     auto start = std::chrono::high_resolution_clock::now();
//     Solution s = ILS(maxIter, maxIterILS, data);
//     auto stop = std::chrono::high_resolution_clock::now();

//     double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 1000.0;
//     return {s.value, duration};
// }

int main(int argc, char **argv) {
    std::fstream file;
    double sum_time = 0, sum_value = 0;

    // Gerando seed
    genSeed();

    // Lendo dados de instância
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();
    int maxIter = 50;
    int maxIterILS = n >= 150 ? n / 2 : n;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0 ; i < 10; i++) {
      Solution s = ILS(maxIter, maxIterILS, data);
      sum_value += s.value;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 10000.0;

    // Vetor para armazenar resultados de cada thread
    // std::vector<result> results(NUM_THREADS);

    // Criando e executando threads
    // std::vector<std::thread> threads;
    // for (int i = 0; i < NUM_THREADS; i++) {
    //     threads.emplace_back([&, i]() {
    //         results[i] = executeILS(data, maxIter, maxIterILS);
    //     });
    // }

    // // Esperar todas terminarem
    // for (auto &t : threads) t.join();

    // // Somando resultados
    // for (auto &r : results) {
    //     sum_time += r.time;
    //     sum_value += r.value;
    // }

    // Benchmark
    file.open("benchmark.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo\n";
        return -1;
    }

    file << "\nInstância: " << data.getInstanceName()
         << "\nMédia tempo: " << duration 
         << "\nMédia valor: " << sum_value / 10 << std::endl;

    file.close();
    return 0;
}
