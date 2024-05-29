#include "Data.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <fstream>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
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

//Escolhe 3 nós aleatórios e retorna os nós restantes
vector<int> Escolhe3Nos(vector<int> *nos, int dimension) {
    vector<int> nosRestantes;

    //gerando vetor com nós
    for(int i = 2; i <= dimension; i++) {
        nosRestantes.push_back(i);
    }

    for(int i = 0; i < 3; i++) {
        int random_index = rand() % nosRestantes.size();
        nos->insert(nos->begin() + i + 1, nosRestantes[random_index]);
        nosRestantes.erase(nosRestantes.begin() + random_index);
    }
    
    return nosRestantes;
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
    vector<InsertionInfo> custoInsercao((s.sequencia.size() - 1) * CL.size());
    int l = 0;
    for(int a = 0; a < s.sequencia.size() - 1; a++) {
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

    //Adiciona todos os nós em vetor
    s.sequencia.push_back(1);
    s.sequencia.push_back(1);
    vector<int> CL = Escolhe3Nos(&s.sequencia, dimension); //A função seleciona 3 nós aleatórios

    while(!CL.empty()) {
        vector<InsertionInfo> custoInsercao = CalcularCustoInsercao(s, CL, data);
        ordernarEmOrdemCrescente(custoInsercao);        
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao[selecionado], CL);
    };


    calcularValorObj(&s, data);
    return s;
} 

//Best improvement swap
bool bestImprovementSwap(Solucao *s, Data& data) {
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
            double delta = 0;
           
            if(j == i + 1) {
                delta = data.getDistance(vj, vi_prev)  + data.getDistance(vi, vj_next) - data.getDistance(vi, vi_prev) - data.getDistance(vj, vj_next);    
            }
            else { 
            //Custo Novo - anterior nos entre nós que podem ser alterados
                delta =  data.getDistance(vi, vj_prev) + data.getDistance(vi, vj_next) + data.getDistance(vj, vi_prev) +  data.getDistance(vj, vi_next) - data.getDistance(vi, vi_prev) - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_prev) - data.getDistance(vj, vj_next);

            }
                
            
            //custo menor
            if(delta < bestDelta) {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    //delta negativo -> custo menor, então trocamos os nós
    if(bestDelta < 0) {
        std::swap(s->sequencia[best_i], s->sequencia[best_j]);
        s->valorObj += bestDelta;
        return true;
    }

    //não houve redução de custo
    return false;    
} 


bool bestImprovement2Opt(Solucao *s, Data &data) {
    double bestDelta = 0;
    int best_i, best_j;
    size_t seq_size = s->sequencia.size(); //tamanho da sequencia

    for(int i = 0; i < seq_size - 3; i++) {
        int vi = s->sequencia[i];
        int vi_next  = s->sequencia[i + 1];
        int limit = (!i) ? seq_size - 3 : seq_size - 2; // i == 0, o limite é menor, pela condição dos vértices proposta
        for(int j = i + 2; j <= limit; j++) {
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[(j + 1)];

            double delta  = data.getDistance(vi, vj) + data.getDistance(vi_next, vj_next) - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next);
            
            if(delta < bestDelta) {
                best_i = i + 1;
                best_j = j;
                bestDelta = delta;
            } 
        }
    }

    //se houve redução de custo
    if (bestDelta < 0) {
        reverse(s->sequencia.begin() + best_i, s->sequencia.begin() + best_j + 1);
        s->valorObj += bestDelta;
        return true;
    } 

    return false;
}

bool or_opt(Solucao *s , Data &data, int n) {
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequencia.size() - n; i++) {
        int vi_prev = s->sequencia[i - 1];
        int vi_first = s->sequencia[i];
        int vi_last = s->sequencia[i + n - 1];
        int vi_next = s->sequencia[i + n];
        
        for(int j = 0; j < s->sequencia.size() - 1; j++) {
            if((j >= i && j <= i + n - 1) || (i == j + 1))
                continue;
            
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j + 1];
            
            double delta = data.getDistance(vi_first, vj) + data.getDistance(vi_last, vj_next) + data.getDistance(vi_prev, vi_next) - data.getDistance(vi_first, vi_prev) - data.getDistance(vi_last, vi_next) - data.getDistance(vj, vj_next);
            
            //verifica redução de custo            
            if(delta < bestDelta) {
                best_i  = i;
                best_j = j;
                bestDelta = delta;
            } 
        }
        
    }
    
    //se houve redução de custo
    if(bestDelta < 0) {
        int value;
        for(int  i = 0; i < n; i++) {
            if(best_j < best_i) {
                value = s->sequencia[best_i + i];
                s->sequencia.erase(s->sequencia.begin() + best_i + i);
                s->sequencia.insert(s->sequencia.begin() + best_j + 1 + i, value);
            } else {
                value = s->sequencia[best_i];
                s->sequencia.erase(s->sequencia.begin() + best_i);
                s->sequencia.insert(s->sequencia.begin() + best_j, value);
            }
        }    
        
        s->valorObj += bestDelta;
       

        return true;
    }
    
    return false;
}

//Busca local
void buscaLocal(Solucao *s, Data &data) {
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    int  i = 0;
    while(!NL.empty()) {
        int n = rand() % NL.size();
        
        switch(NL[n]) {
            case 1: //Swap
                improved = bestImprovementSwap(s, data);
                break;
            case 2: //2Opt
                improved = bestImprovement2Opt(s, data);
                break;
            case 3: //Reinsertion
                improved = or_opt(s, data, 1);
                break;
            case 4: //orOpt 2
                improved = or_opt(s, data, 2);
                break;
            case 5: //orOpt 3
                improved = or_opt(s, data, 3);
                break;
        }
        //Se houve melhoria
        if(improved) 
            NL = {1, 2, 3, 4, 5};
        else 
            NL.erase(NL.begin() + n);   
    }
}

Solucao pertubacao(Solucao s, Data &data) {
    int v = s.sequencia.size() - 1;
    int random_i = rand()%(v - 2) + 1; //gera índice i aleatório
    
    int size_i = rand()%(v/10) + 1; //gera tamanho aleatório com limite 

    if(size_i < 2) {
        size_i = 2;
    } else {
        size_i--;
    }

    double delta;

    //se ultrapassar o último nó disponível antes de voltar ao ponto inicial
    if(random_i + size_i >= v) 
        size_i = v - random_i - 1;

    int random_j = rand()%(v - size_i - 4) + 1;
    int size_j = rand()%(v/10) + 1;

    if(size_j < 2) {
        size_j = 2;
    } else {
        size_j--;
    }


    //se for depois do primeiro segmento
    if(random_j >= random_i - 1) {
        random_j += size_i + 2;

        if(random_j + size_j >= v)
            size_j = v - random_j - 1;
    }
    else {
        //Não pode ultrapassar as soluções nem ser vizinho (a esquerda) do primeiro segmento
        if(random_j + size_j >= random_i)
            size_j = random_i - random_j - 1;
    }
    
    //primeiro segmento
    int vi_previus = s.sequencia[random_i - 1];
    int vi_first = s.sequencia[random_i];
    int vi_last = s.sequencia[random_i + size_i];
    int vi_next = s.sequencia[random_i + size_i + 1];

    //criando vetor auxiliar para primeiro segmento (i)
    vector<int> i_aux;
    i_aux.insert(i_aux.begin(), s.sequencia.begin() + random_i, s.sequencia.begin() + random_i + size_i + 1);

    //segundo segmento
    int vj_previus = s.sequencia[random_j - 1];
    int vj_first = s.sequencia[random_j];
    int vj_last = s.sequencia[random_j + size_j];
    int vj_next = s.sequencia[random_j + size_j + 1];

    //criando vetor auxiliar para segundo segmento (j)
    vector<int> j_aux;
    j_aux.insert(j_aux.begin(), s.sequencia.begin() + random_j, s.sequencia.begin() + random_j + size_j + 1);

    //o seguimento mais próximo do final será removido primeiramente
    if (random_i > random_j) {
        //remove segmento i
        s.sequencia.erase(s.sequencia.begin() + random_i, s.sequencia.begin() + random_i + size_i + 1);

        //adiciona segmento j auxiliar
        s.sequencia.insert(s.sequencia.begin() + random_i, j_aux.begin(), j_aux.begin() + j_aux.size()); 

        //remove segmento j
        s.sequencia.erase(s.sequencia.begin() + random_j, s.sequencia.begin() + random_j + size_j + 1);

        //insere segmento i
        s.sequencia.insert(s.sequencia.begin() + random_j, i_aux.begin(), i_aux.begin() + i_aux.size());
        

    } else {
        //remove segmento j
        s.sequencia.erase(s.sequencia.begin() + random_j, s.sequencia.begin() + random_j + size_j + 1);

        //insere segmento i auxiliar
        s.sequencia.insert(s.sequencia.begin() + random_j, i_aux.begin(), i_aux.begin() + i_aux.size());

        //remove segmento i
        s.sequencia.erase(s.sequencia.begin() + random_i, s.sequencia.begin() + random_i + size_i + 1);

        //adiciona segmento j auxiliar
        s.sequencia.insert(s.sequencia.begin() + random_i, j_aux.begin(), j_aux.begin() + j_aux.size()); 
    }

    //calculando variação de custo da solução após pertubação
    if(vi_previus == vj_last) {
        delta = data.getDistance(vj_previus, vi_first) + data.getDistance(vi_last, vj_first) + data.getDistance(vj_last, vi_next) - data.getDistance(vj_previus, vj_first) - data.getDistance(vj_last, vj_next) - data.getDistance(vi_last, vi_next);
    }
    else if(vi_next == vj_first) {
        delta = data.getDistance(vj_first, vi_previus) + data.getDistance(vj_last, vi_first) + data.getDistance(vi_last, vj_next) - data.getDistance(vi_previus, vi_first) - data.getDistance(vi_last, vi_next) - data.getDistance(vj_last, vj_next);
    }
    else {
        delta = data.getDistance(vj_previus, vi_first) + data.getDistance(vi_last, vj_next) + data.getDistance(vi_previus, vj_first) + data.getDistance(vj_last, vi_next) - data.getDistance(vi_previus, vi_first) - data.getDistance(vi_last, vi_next) - data.getDistance(vj_previus, vj_first) - data.getDistance(vj_last, vj_next);
    }

    s.valorObj += delta;

    return s;
}

Solucao ILS(int maxIter, int maxIterILS, Data &data, int dimension) {
    Solucao bestOfAll;
    bestOfAll.valorObj = INFINITY;

    for(int i = 0; i < maxIter; i++) {
        Solucao s = Construcao(dimension, data);
        Solucao best = s;

        int iterILS = 0;

        while(iterILS <= maxIterILS) {
            buscaLocal(&s, data);

            if(s.valorObj < best.valorObj) {
                best = s;
                iterILS = 0;
            }
            int estimed = s.valorObj;
            calcularValorObj(&s, data);
            if(estimed != s.valorObj) {
                cout << "Cálculo errado após BuscaLocal()\n";
                exit(EXIT_FAILURE);
            }

            s = pertubacao(best, data);
            iterILS++;

            estimed = s.valorObj;
            calcularValorObj(&s, data);
            if(estimed != s.valorObj) {
                cout << "Cálculo errado após Pertubação()\n";
                exit(EXIT_FAILURE);
            }
        }

        if(best.valorObj < bestOfAll.valorObj) {
            bestOfAll = best;
        }

    }

    return bestOfAll;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    
    //Solucao s;
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();
    Solucao bestOfAll;

    int maxIter = 50;
    int maxIterILS;

    if(n >= 150) {
        maxIterILS = n/2;
    } else {
        maxIterILS = n;
    }

    //medição
    auto inicio = high_resolution_clock::now();
    bestOfAll = ILS(maxIter, maxIterILS, data, n);
    auto fim = high_resolution_clock::now();

    const auto tempo = duration_cast<milliseconds>(fim - inicio);

    string line;
    fstream benchMark_in("benchmark.csv", std::ios::in);

    //verifica se arquivo abriu
    if(benchMark_in.is_open()) {
        getline(benchMark_in, line);

        //verifica se já tem o cabeçalho da tabela
        if(line == "Instância;Tempo;Custo");
        else {
            fstream benchMark_out("benchmark.csv", std::ios::app);
            if(benchMark_out.is_open()) {
                benchMark_out << "Instância;Tempo;Custo\n";
                benchMark_out.close();
            } else {
                cout << "Erro ao abrir/criar benchmark.csv\n"; 
            }
        }
 
        benchMark_in.close();
    } else {
        cout << "Erro ao abrir/criar benchmark.csv\n"; 
    }

    fstream benchMark_out("benchmark.csv", std::ios::app);
    if(benchMark_out.is_open())
        benchMark_out << data.getInstanceName() << ";" << (double)tempo.count()/1000.0 << ";" << bestOfAll.valorObj << endl;
    else {
        cout << "Erro ao abrir/criar benchmark.csv\n"; 
    }

    benchMark_out.close();
}
