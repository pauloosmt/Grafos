#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Executa a Busca em Largura (BFS) a partir do vertice u e imprime a distancia
// (em numero de arestas) de u ate todos os demais vertices alcancaveis.
// u: vertice de origem da busca
// adj: lista de adjacencia do grafo (adj[i] contem os vizinhos do vertice i)
// n: quantidade total de vertices do grafo
void bfs(int u, vector<vector<int>>& adj, int n) {
    // Marca quais vertices ja foram visitados, para nao processa-los mais de uma vez.
    vector<bool> visitado(n, false);
    // Guarda a distancia minima (em arestas) de u ate cada vertice; -1 significa "nao alcancado".
    vector<int> distancia(n, -1);

    // Fila usada para visitar os vertices na ordem correta (por niveis de distancia).
    queue<int> fila;

    // Inicializa a busca a partir do vertice de origem.
    visitado[u] = true;
    distancia[u] = 0;
    fila.push(u);

    // Enquanto houver vertices a processar na fila...
    while(!fila.empty()) {
        int atual = fila.front();
        fila.pop();

        // Percorre todos os vizinhos do vertice atual.
        for(int vizinho : adj[atual]) {
            // Se o vizinho ainda nao foi visitado, ele e descoberto agora,
            // a uma distancia igual a distancia do atual + 1.
            if(!visitado[vizinho]) {
                visitado[vizinho] = true;
                distancia[vizinho] = distancia[atual] + 1;
                fila.push(vizinho);
            }
        }
    }

    // Exibe a distancia calculada de u ate cada vertice do grafo.
    for (int i= 0; i < n; i++) {
        cout << "Distancia ate " << i << ": " <<  distancia[i] << endl;
    }
}

int main() {
    // Quantidade de vertices do grafo (vertices 0 a 5).
    int n = 6;

    // Lista de adjacencia representando um grafo nao direcionado:
    // adj[i] contem os vertices diretamente conectados ao vertice i.
    vector<vector<int>> adj(n);
    adj[0] = {1, 2, 3};
    adj[1] = {0, 3};
    adj[2] = {0, 1, 3, 5};
    adj[3] = {0, 2 , 4};
    adj[4] = {3};
    adj[5] = {2};

    // Executa a BFS a partir do vertice 2.
    bfs(2, adj, n);

    return 0;
}