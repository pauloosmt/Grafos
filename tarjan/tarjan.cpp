#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> LA;    // lista de adjacencia do grafo
vector<int> desc;          // desc[v]: tempo (pre-ordem) em que v foi descoberto pela DFS
vector<int> low;           // low[v]: menor desc alcancavel a partir de v usando no maximo uma aresta de retorno
vector<bool> visitado;     // visitado[v]: true se v ja foi descoberto pela DFS
vector<bool> naPilha;      // naPilha[v]: true enquanto v ainda esta na pilha de recursao (chamadas em aberto)
int tempo = 0;              // contador global, incrementado a cada vertice descoberto

// Visita u e propaga o low-link para o seu chamador (algoritmo de Tarjan).
void dfsTarjan(int u, int pai) {
   desc[u] = low[u] = tempo;   // ao descobrir u, low[u] comeca igual ao proprio tempo de descoberta
   tempo++;
   visitado[u] = true;
   naPilha[u] = true;          // u entra na pilha de recursao (ainda pode fechar uma CFC com seus ancestrais)

   for(int v : LA[u]) {
    if(v == pai) continue;
    if(!visitado[v]) {
        // v ainda nao foi descoberto: aresta de arvore, desce recursivamente
        dfsTarjan(v, u);
        // low[u] aproveita qualquer ancestral mais antigo que v conseguiu alcancar
        low[u] = min(low[u], low[v]);
    }
    else if(naPilha[v]) {
        // v ja foi descoberto e ainda esta na pilha: aresta de retorno, fecha um ciclo
        // usa desc[v] (nao low[v]) pois v pode nao ser ancestral direto de u
        low[u] = min(low[u], desc[v]);
    }
    // se v ja foi descoberto mas nao esta mais na pilha, pertence a outra CFC ja fechada: ignora
   }

   naPilha[u] = false;   // u termina de ser processado e sai da pilha de recursao
}

// Prepara as estruturas auxiliares e executa a DFS de Tarjan a partir de todo vertice ainda nao visitado
// (necessario para cobrir grafos desconexos).
void tarjan(int n) {
    desc.assign(n, -1);
    low.assign(n, -1);
    visitado.assign(n, false);
    naPilha.assign(n, false);
    tempo = 1;

    for(int u = 0; u < n; u++) {
        if(!visitado[u]) {
            dfsTarjan(u, -1);
        }
    }
}

int main() {
    int n = 8;
    LA.assign(n,{});

    // Grafo direcionado de exemplo: vertices 0,1,2 formam um ciclo entre si,
    // e vertices 3,4,5 formam outro ciclo, com 6 apontando para 3.
    LA[0] = {1,2};
    LA[1] = {0, 2};
    LA[2] = {0, 1, 3};
    LA[3] = {2, 4, 5};
    LA[4] = {3, 5};
    LA[5] = {3, 4, 6};
    LA[6] = {5,7};
    LA[7] = {6};


    tarjan(n);

    // Vertices de uma mesma CFC terminam com o mesmo valor de low.
    cout << "vertice | desc | low" << endl;
    for (int i = 0; i < n; i++) {
        cout << "   " << i + 1 << "    |  " << desc[i] << "   |  " << low[i] << endl;
    }
}
