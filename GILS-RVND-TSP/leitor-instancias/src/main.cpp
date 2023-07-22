#include "Data.h"
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    vector<int> sequencia;
    double valorObj;
} Solucao;

// Exibe solução
void exibirSolucao(Solucao *s) {
for(int i = 0; i < s->sequencia.size() - 1; i++)
    cout << s->sequencia[i] << " -> ";
    cout << s->sequencia.back() << endl;
}

// Calcula 
void calcularValorObj(Solucao *s, Data& dados){
    s->valorObj = 0;
    for(int i = 0; i < s->sequencia.size() - 1; i++) {
        s->valorObj += dados.getDistance(s->sequencia[i], s->sequencia[i + 1]);
    }
}

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;

    return 0;
}
