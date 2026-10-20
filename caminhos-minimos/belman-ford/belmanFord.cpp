// =====================================================================
// ALGORITMO DE BELLMAN-FORD - Caminho Minimo de Origem Unica
// =====================================================================
//
// O QUE E O PROBLEMA?
// O mesmo do Dijkstra: dado um grafo ponderado e um vertice de origem s,
// queremos a menor distancia de s ate CADA outro vertice. A diferenca e
// que aqui as arestas PODEM ter peso negativo (o que quebraria o
// Dijkstra). Em troca, o Bellman-Ford e mais lento.
//
// IDEIA DO ALGORITMO (programacao dinamica / relaxamento repetido):
//   1. Comece com dist[s] = 0 e dist[todo o resto] = infinito.
//   2. "Relaxe" TODAS as arestas do grafo: para cada aresta (u, v, peso),
//      se chegar em v passando por u fica mais barato que o melhor valor
//      atual de dist[v], atualize dist[v].
//   3. Repita o passo 2 um total de N-1 vezes. Por que N-1? Porque um
//      caminho minimo simples (sem repetir vertice) tem no maximo N-1
//      arestas, entao depois de N-1 rodadas de relaxamento toda distancia
//      minima real ja foi encontrada.
//   4. Se parar de haver melhoria antes das N-1 rodadas, as distancias ja
//      estabilizaram e dá pra encerrar mais cedo.
//
// COMO DETECTAR CICLO NEGATIVO? Se existir um ciclo de peso total
// negativo alcancavel a partir de s, a distancia minima nao esta bem
// definida (pode diminuir para sempre, repetindo o ciclo). Por isso,
// depois das N-1 rodadas fazemos mais UMA passada de relaxamento: se
// ainda for possivel melhorar alguma distancia, e porque existe esse
// ciclo negativo.
// =====================================================================

#include <iostream>
#include <vector>
using namespace std;

const long long INF = 1e18;

struct Aresta {
    int u, v, w; // aresta de u para v com peso w (pode ser negativo)
};

// Executa o algoritmo de Bellman-Ford a partir do vertice s.
// Aceita pesos negativos e detecta ciclos negativos alcancaveis a partir de s.
// s: vertice de origem
// arestas: lista de arestas do grafo
// n: quantidade total de vertices
// Retorna false se existir ciclo negativo alcancavel; true caso contrario.
bool bellmanFord(int s, vector<Aresta>& arestas, int n) {
    // Menor distancia conhecida de s ate cada vertice. Comeca tudo "infinito"
    // (inalcancavel), exceto a propria origem, que esta a distancia 0 dela mesma.
    vector<long long> dist(n, INF);
    // Pai de cada vertice no caminho minimo (para reconstruir o caminho, se precisar).
    vector<int> pai(n, -1);
    dist[s] = 0;

    // Relaxa todas as arestas n-1 vezes: um caminho minimo tem no maximo n-1 arestas,
    // entao n-1 rodadas sao suficientes para propagar a distancia minima real a todo mundo.
    for(int i = 0; i < n - 1; i++) {
        bool mudou = false;
        for(auto& a : arestas) {
            // So faz sentido relaxar a partir de a.u se ele ja for alcancavel.
            if(dist[a.u] != INF && dist[a.u] + a.w < dist[a.v]) {
                dist[a.v] = dist[a.u] + a.w;
                pai[a.v] = a.u;
                mudou = true;
            }
        }
        // Se uma rodada inteira nao melhorou nada, as distancias ja estabilizaram
        // e nao ha motivo para continuar ate a n-1-esima rodada.
        if(!mudou) break;
    }

    // Uma n-esima passada que ainda relaxe alguma aresta so e possivel se existir
    // um ciclo de peso negativo nesse caminho (senao, n-1 rodadas ja bastavam).
    for(auto& a : arestas) {
        if(dist[a.u] != INF && dist[a.u] + a.w < dist[a.v]) {
            cout << "Ciclo negativo detectado!" << endl;
            return false;
        }
    }

    for(int i = 0; i < n; i++) {
        cout << "Distancia ate " << i << ": ";
        if(dist[i] == INF) cout << "inalcancavel" << endl;
        else cout << dist[i] << " (pai: " << pai[i] << ")" << endl;
    }
    return true;
}

int main() {
    // Grafo direcionado ponderado com vertices 0 a 4 e pesos negativos
    // (por isso precisa de Bellman-Ford, e nao Dijkstra).
    int n = 5;
    vector<Aresta> arestas = {
        {0, 1, 6}, {0, 2, 7},
        {1, 2, 8}, {1, 3, 5}, {1, 4, -4},
        {2, 3, -3}, {2, 4, 9},
        {3, 1, -2},
        {4, 0, 2}, {4, 3, 7}
    };

    // Calcula e imprime a menor distancia do vertice 0 ate todos os demais
    // (ou avisa se houver ciclo negativo alcancavel).
    bellmanFord(0, arestas, n);

    return 0;
}
