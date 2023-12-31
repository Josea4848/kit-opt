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

//Inseri vértice na solução e remove K do CL
void inserirNaSolucao(Solucao& s, InsertionInfo info, vector<int>& CL) {
    s.sequencia.insert(s.sequencia.begin() + 1 + info.arestaRemovida, info.noInserido);

    //Removendo K de CL
    for(size_t i = 0; i < CL.size(); i++) {
        if(CL[i] == info.noInserido) {
            CL.erase(CL.begin() + i);
            break;
        }
    }
}

// Exibe solução
void exibirSolucao(Solucao *s) {
    for(int i = 0; i < s->sequencia.size() - 1; i++) {
        cout << s->sequencia[i] << " -> ";
    }
    cout << s->sequencia.back() << endl;
}

// Ordena em ordem crescente de custo
void ordernarEmOrdemCrescente(vector<InsertionInfo>& conjunto) {
    while(true) {
        bool mudou = false;

        for(size_t i = 0; i < conjunto.size() - 1; i++) {
            if(conjunto[i].custo > conjunto[i + 1].custo) {
                InsertionInfo vectAux = conjunto[i];
                conjunto[i] = conjunto[i + 1];
                conjunto[i + 1] = vectAux;
                mudou = true;
            }
        }
        //Caso não haja mais alteração, a ordenação está completa
        if(!mudou)
            break;
    }
}

// Calcula a distância total percorrida em tal
void calcularValorObj(Solucao *s, Data& dados){
    s->valorObj = 0;
    for(int i = 0; i < s->sequencia.size() - 1; i++) {
        s->valorObj += dados.getDistance(s->sequencia[i], s->sequencia[i + 1]);
    }
}

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
        ordernarEmOrdemCrescente(custoInsercao);
        srand(time(NULL));
        double alpha = rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao[selecionado], CL);
    }

    return s;
} 

//Best improvement
bool bestImprovement(Solucao *s, Data& data) {
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequencia.size() - 1; i++) {
        int vi = s->sequencia[i];
        int vi_prev = s->sequencia[i - 1]; //previous
        int vi_next = s->sequencia[i + 1]; //next

        for(int j = i + 1; j < s->sequencia.size() - 1; j++) {
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j + 1];
            int vj_prev = s->sequencia[j - 1];
            double delta = data.getDistance(vi_prev,vi) -data.getDistance(vi,vi_next) + data.getDistance(vi_prev,vj) + data.getDistance(vj, vi_next) - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next) + data.getDistance(vj_prev,vi) + data.getDistance(vi, vj_next);

            if(delta < bestDelta) {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0) {
        std::swap(s->sequencia[best_i], s->sequencia[best_j]);
        s->valorObj += bestDelta;
        return true;
    }

    return false;    
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
