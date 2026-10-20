// =====================================================================
// ALGORITMO DE FLOYD-WARSHALL - Caminhos Minimos entre TODOS os pares
// =====================================================================
//
// O QUE E O PROBLEMA?
// Diferente do Dijkstra e do Bellman-Ford (que calculam a distancia de
// UMA origem s ate todo mundo), aqui queremos a menor distancia entre
// CADA par de vertices (i, j) de uma vez. Aceita pesos negativos, desde
// que nao haja ciclo de peso negativo (nesse caso a resposta nao existe,
// pois a distancia poderia diminuir para sempre).
//
// IDEIA DO ALGORITMO (programacao dinamica):
// Pense assim: "qual a menor distancia de i ate j usando so os vertices
// 0, 1, ..., k como intermediarios permitidos?"
//   - Para k = -1 (nenhum intermediario), a resposta e so a aresta direta
//     i -> j (ou INF se nao existir).
//   - Para cada novo k, ha duas opcoes: OU o melhor caminho de i ate j
//     continua sem passar por k, OU passa por k (indo de i ate k e
//     depois de k ate j, usando intermediarios ja conhecidos). Ficamos
//     com a opcao mais barata.
// Repetindo isso para k = 0, 1, ..., n-1, no final dist[i][j] e a menor
// distancia de i ate j usando QUALQUER vertice como intermediario.
//
// POR QUE A ORDEM DOS LACOS IMPORTA: o laco de k precisa ser o mais
// externo, porque dist[i][k] e dist[k][j] usados numa rodada k precisam
// já refletir os intermediarios 0..k-1 (calculados nas rodadas
// anteriores), e nao podem ainda ter sido "contaminados" pela propria
// rodada k.
//
// RECONSTRUCAO DE CAMINHO: pred[i][j] guarda o vertice que vem IMEDIATAMENTE
// antes de j num caminho minimo de i ate j. Toda vez que achamos um atalho
// passando por k, atualizamos pred[i][j] = pred[k][j] (o predecessor de j
// no trecho k -> j, que agora tambem vale para o caminho completo i -> j).
//
// COMO DETECTAR CICLO NEGATIVO: se, ao final, algum dist[i][i] ficar
// negativo, e porque existe um ciclo que passa por i com peso total
// negativo (normalmente a distancia de um vertice ate ele mesmo e 0).
// =====================================================================

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

const int INF = 1000000000;

// Calcula, em dist, a menor distancia entre cada par de vertices (i, j).
// dist: matriz de entrada/saida. Na entrada, dist[i][j] e o peso da aresta
//       direta i->j (INF se nao existir), e dist[i][i] = 0. Na saida, vira
//       a matriz de menores distancias entre todos os pares.
// pred: (saida) pred[i][j] = vertice imediatamente anterior a j num caminho
//       minimo de i ate j (-1 se nao houver caminho).
// Retorna true se existir ciclo de peso negativo alcancavel; false caso contrario.
bool floydWarshall(vector<vector<int>>& dist, vector<vector<int>>& pred) {
    int n = dist.size();

    // Antes de qualquer intermediario, o predecessor de j (vindo de i) so
    // pode ser o proprio i, e so faz sentido se existir aresta direta i->j.
    pred.assign(n, vector<int>(n, -1));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i != j && dist[i][j] != INF) {
                pred[i][j] = i;
            }
        }
    }

    // k percorre, um a um, os vertices permitidos como intermediario.
    // Precisa ser o laco mais externo (veja explicacao acima).
    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++){
            // Se k e inalcancavel a partir de i, nenhum caminho i -> k -> j existe:
            // pula essa linha inteira sem perder tempo.
            if(dist[i][k] == INF) continue;

            for(int j = 0; j < n; j++) {
                // Se j e inalcancavel a partir de k, tambem nao ha caminho via k.
                if(dist[k][j] == INF) continue;

                // Compara o melhor caminho conhecido i->j com a alternativa
                // de passar por k (i->k->j). Fica com o mais barato.
                if(dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];

                    // O caminho ate j agora vem do trecho k->j, entao o
                    // predecessor de j passa a ser o mesmo que k->j usava.
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }

    // dist[i][i] comeca em 0 (ficar parado em i nao custa nada). Se, depois
    // de todas as rodadas, ficou negativo, e porque existe um ciclo que sai
    // de i e volta para i com custo total negativo.
    for(int i = 0; i < n; i++){
        if(dist[i][i] < 0) return true;
    }

    return false;
}

int main() {
    int n = 4;

    // Matriz de pesos de um grafo direcionado: INF = sem aresta direta, 0 na diagonal.
    vector<vector<int>> dist(n, vector<int>(n, INF));
    for(int i = 0; i < n; i++) dist[i][i] = 0;

    dist[0][1] = 3;
    dist[0][3] = 7;
    dist[1][0] = 8;
    dist[1][2] = 2;
    dist[2][0] = 5;
    dist[2][3] = 1;
    dist[3][0] = 2;

    cout<< "Matriz original: " << endl;
    for(int i = 0; i< n; i++){
        for(int j = 0; j < n; j++) {
            cout<< dist[i][j] << "  ";
        }
        cout << endl;
    }

    vector<vector<int>> pred;
    bool cicloNegativo = floydWarshall(dist, pred);

    if(cicloNegativo) {
        cout << "O grafo possui ciclo de peso negativo" << endl;
        return 0;
    }

    // Exibe a matriz de menores distancias entre todos os pares.
    cout << "Matriz de distancias:" << endl;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(dist[i][j] == INF) cout << setw(4) << "INF";
            else cout << setw(4) << dist[i][j];
        }
        cout << endl;
    }

    // Exibe o caminho minimo do vertice 3 ate cada um dos outros.
    int origem = 3;
    cout << endl << "Caminhos a partir do vertice " << origem << ":" << endl;
    for(int j = 0; j < n; j++) {
        if(j == origem) continue;
        cout << origem << " ate " << j << " (custo " << dist[origem][j] << "): ";
        cout << endl;
    }


    return 0;
}
