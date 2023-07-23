#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

typedef struct {
    vector<int> sequencia;
    double valorObj;
} Solucao;

//InsertInfo
typedef struct {
    int noInserido; // cidade (nó) que será inserida
    int arestaRemovida; // percurso (aresta) que será removido
    double custo; // custo de inserção de k
} InsertionInfo;

//Escolhe 3 nós aleatórios
vector<int> Escolhe3Nos(vector<int>& nos) {
    //Gera semente para o rand()
    srand (time(NULL));
    vector<int> randomNos(3);
    for(int i = 0; i < 3; i++) {
        size_t index = rand() % nos.size();
        randomNos[i] = nos[index];
        nos.erase(nos.begin() + index);
    }
    
    return randomNos;
}

// Exibe solução
void exibirSolucao(Solucao *s) {
    for(int i = 0; i < s->sequencia.size() - 1; i++) {
        cout << s->sequencia[i] << " -> ";
    }
    cout << s->sequencia.back() << endl;
}

// Calcula a distância total percorrida em tal
void calcularValorObj(Solucao *s, Data& dados){
    s->valorObj = 0;
    for(int i = 0; i < s->sequencia.size() - 1; i++) {
        s->valorObj += dados.getDistance(s->sequencia[i], s->sequencia[i + 1]);
    }
}

//

// Calcula o custo de inserção de um conjunto
vector<InsertionInfo> CalcularCustoInsercao(Solucao& s, vector<int>& CL, Data &dados) {
    int numSeq = s.sequencia.size();
    vector<InsertionInfo> custoInsercao((numSeq - 1) * CL.size());
    int l = 0;
    for(int a = 0; a < numSeq - 1; a++) {
        int i = s.sequencia[a];
        int j = s.sequencia[a + 1];
        for(auto k : CL) {
            custoInsercao[l].custo = dados.getDistance(i, k) + dados.getDistance(j, k) - dados.getDistance(i, j);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    }

    return custoInsercao;
}

//Construcao
Solucao Construcao(size_t dimension, Data& data) {
    Solucao s;    
    vector<int> nos; //contém todos os nós;
    //Adiciona todos os nós em vetor
    for (size_t i = 1; i < dimension; i++) {
        nos.push_back(i);
    }
    s.sequencia = Escolhe3Nos(nos); //A função seleciona 3 nós e os remove do array
    vector<int> CL = nos; //O restanto dos nós são passados para CL
    //Limpa o vetor original
    nos.clear();

    while(!CL.empty()) {
        vector<InsertionInfo> custoInsercao = CalcularCustoInsercao(s, CL, data);


    }

    
} 


int main(int argc, char** argv) {

    /* auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    //data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;  */


    return 0;
}
