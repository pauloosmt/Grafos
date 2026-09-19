#include <iostream>
#include <vector>
using namespace std;

// Cores usadas para marcar o estado de cada vertice durante a DFS:
// BRANCO = ainda nao descoberto, CINZA = em processamento (na pilha de recursao),
// PRETO = totalmente processado (ele e todos os seus descendentes ja foram visitados).
const int BRANCO = 0;
const int CINZA = 1;
const int PRETO = 2;

vector<vector<int>> adj;   // lista de adjacencia do grafo
vector<int> cor;           // cor atual de cada vertice
vector<int> desc;          // tempo de descoberta de cada vertice
vector<int> fim;           // tempo de termino (finalizacao) de cada vertice
int tempo = 0;              // contador global de tempo, compartilhado por toda a floresta de busca

// Visita recursivamente o vertice u, classificando cada aresta encontrada
// de acordo com a cor do vertice de destino (algoritmo classico de DFS com timestamps).
void dfsVisit(int u) {
    tempo++;
    desc[u] = tempo;
    cor[u] = CINZA;

    for(int v: adj[u]) {
        if(cor[v] == BRANCO) {
            // v ainda nao foi visitado: a aresta (u, v) faz parte da arvore de busca.
            cout << " Aresta (" << u << ", " << v << ") -> ARVORE" << endl;
            dfsVisit(v);
        }
        else if(cor[v] == CINZA) {
            // v esta na pilha de recursao atual (ainda sendo processado): aresta de retorno,
            // indica um ciclo no grafo.
            cout << " Aresta (" << u << ", " << v << ") -> RETORNO" << endl;
        }
        else {
            // v ja foi totalmente processado (PRETO). Se u foi descoberto antes de v,
            // e uma aresta de avanco (para um descendente na mesma arvore);
            // caso contrario, e uma aresta cruzada (liga ramos diferentes da arvore).
            if (desc[u] < desc[v])
                cout << " Aresta (" << u << ", " << v << ") -> AVANÇO" << endl;
            else
                cout << " Aresta (" << u << ", " << v << ") -> CRUZADA" << endl;
        }
    }

    cor[u] = PRETO;
    tempo++;
    fim[u] = tempo;
    cout << "Terminando " << u << " no tempo " << tempo << endl;
}

// DFS padrao (CLRS): percorre todos os vertices do grafo, iniciando uma nova
// arvore de busca a cada vertice ainda BRANCO. Nao recebe vertice inicial pois
// precisa cobrir tambem vertices inalcancaveis a partir de um unico ponto de partida
// (grafos desconexos).
void dfs(int n) {
    cor.assign(n, BRANCO);
    desc.assign(n, -1);
    fim.assign(n, -1);
    tempo = 0;

    for(int u = 0; u < n; u++) {
        if(cor[u] == BRANCO) {
            cout << "Iniciando nova arvore em " << u << endl;
            dfsVisit(u);
        }
    }
}

int main() {
    // Quantidade de vertices do grafo (vertices 0 a 5), incluindo um componente
    // desconexo (4 e 5) para exercitar o loop sobre todos os vertices.
    int n = 6;
    adj.assign(n, {});

    // Lista de adjacencia representando um grafo direcionado.
    adj[0] = {1};
    adj[1] = {3};
    adj[2] = {0, 3};
    adj[3] = {1};
    adj[4] = {5};
    adj[5] = {};

    dfs(n);

    // Exibe, para cada vertice, os tempos de descoberta e termino calculados pela DFS.
    for(int i = 0; i < n; i++) {
        cout << "Vertice " << i << ": desc= " << desc[i] << ", fim= " << fim[i] << endl;
    }

    return 0;
}
