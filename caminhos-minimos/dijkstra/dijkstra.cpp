// =====================================================================
// ALGORITMO DE DIJKSTRA - Caminho Minimo de Origem Unica
// =====================================================================
//
// O QUE E O PROBLEMA?
// Dado um grafo ponderado e um vertice de origem s, queremos saber a
// menor distancia (soma de pesos) de s ate CADA outro vertice do grafo.
// Isso so funciona se nenhuma aresta tiver peso negativo: com pesos
// negativos o algoritmo pode dar resposta errada (nesse caso, use
// Bellman-Ford em vez de Dijkstra).
//
// IDEIA DO ALGORITMO (guloso):
//   1. Comece com dist[s] = 0 e dist[todo o resto] = infinito.
//   2. Repita: pegue, entre os vertices ainda nao finalizados, o de MENOR
//      distancia conhecida (u). Essa distancia ja e a minima possivel
//      para u (nao existe aresta negativa que possa melhora-la depois).
//   3. "Relaxe" as arestas que saem de u: para cada vizinho v, se chegar
//      em v passando por u fica mais barato que o melhor valor atual de
//      dist[v], atualize dist[v].
//   4. Repita ate processar todos os vertices alcancaveis.
//
// COMO IMPLEMENTAMOS: usamos uma fila de prioridade (min-heap) que
// sempre entrega o vertice de menor distancia conhecida, sem precisar
// procurar entre todos a cada passo. Como um vertice pode entrar na fila
// mais de uma vez (toda vez que sua distancia melhora), descartamos as
// entradas desatualizadas quando elas saem da fila.
// =====================================================================

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const long long INF = 1e18;

// Executa o algoritmo de Dijkstra a partir do vertice s e imprime a menor distancia
// de s ate todos os demais vertices. Funciona apenas com pesos NAO negativos.
// s: vertice de origem
// adj: lista de adjacencia (adj[u] contem pares {vizinho, peso})
// n: quantidade total de vertices do grafo
void dijkstra(int s, vector<vector<pair<int,int>>>& adj, int n) {
    // Menor distancia conhecida de s ate cada vertice. Comeca tudo "infinito"
    // (inalcancavel), exceto a propria origem, que esta a distancia 0 dela mesma.
    vector<long long> dist(n, INF);
    // Pai de cada vertice no caminho minimo (para reconstruir o caminho, se precisar).
    vector<int> pai(n, -1);

    // Fila de prioridade que entrega SEMPRE o vertice de MENOR distancia.
    // (O priority_queue do C++ por padrao entrega o MAIOR; o "greater" inverte isso.)
    // Cada item: {distancia estimada, vertice}.
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;

    dist[s] = 0;
    pq.push({0, s});

    while(!pq.empty()) {
        // Pega o vertice com a menor distancia estimada entre os candidatos.
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Essa entrada ficou desatualizada: u pode ter sido colocado na fila
        // varias vezes (uma por melhoria de distancia), e ja processamos uma
        // versao mais barata dele antes. Ignora a antiga.
        if(d > dist[u]) continue;

        // u ja tem sua distancia minima definitiva: tenta usa-la para melhorar
        // (relaxar) a distancia de cada vizinho.
        for(auto [v, w] : adj[u]) {
            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pai[v] = u;
                // Distancia de v melhorou: coloca (de novo) na fila com o novo valor.
                pq.push({dist[v], v});
            }
        }
    }

    for(int i = 0; i < n; i++) {
        cout << "Distancia ate " << i << ": ";
        if(dist[i] == INF) cout << "inalcancavel" << endl;
        else cout << dist[i] << " (pai: " << pai[i] << ")" << endl;
    }
}

int main() {
    // Grafo direcionado ponderado com vertices 0 a 5 (todos os pesos nao negativos).
    int n = 6;
    vector<vector<pair<int,int>>> adj(n);
    adj[0] = {{1, 7}, {2, 9}, {5, 14}};
    adj[1] = {{0, 7}, {2, 10}, {3, 15}};
    adj[2] = {{0, 9}, {1, 10}, {3, 11}, {5, 2}};
    adj[3] = {{1, 15}, {2, 11}, {4, 6}};
    adj[4] = {{3, 6}, {5, 9}};
    adj[5] = {{0, 14}, {2, 2}, {4, 9}};

    // Calcula e imprime a menor distancia do vertice 0 ate todos os demais.
    dijkstra(0, adj, n);

    return 0;
}
