#include <iostream>
#include <vector>
#include <queue>

using namespace  std;

#define BRANCO 0
#define CINZA 1
#define PRETO 2

vector<int> BFS_distancias(vector<vector<int>> &lista_adjacencia, int origem)
{
    //estruturas auxiliares
    vector<int> cor(lista_adjacencia.size(), BRANCO);
    vector<int> distancia(lista_adjacencia.size(), 0);
    vector<int> antecessor(lista_adjacencia.size());

    //fila de vertices a serem explorados
    queue<int> fila;

    //iniciando com a origem
    fila.push(origem);
    cor[origem] = CINZA;

    while(!fila.empty()){
        int vertice = fila.front();

        for(int vizinho: lista_adjacencia[vertice]){
            if(cor[vizinho] == BRANCO){
                distancia[vizinho] = distancia[vertice] + 1;
                cor[vizinho] = CINZA;
                antecessor[vizinho] = vertice;
                fila.push(vizinho);
            }
        }

        fila.pop();
        cor[vertice] = PRETO;

    }

    return distancia;
}

int main()
{
    int n = 4; //número de vértices

    //definir a lista de adjacências
    vector<vector<int>> lista_adjacencia = {
        {1, 3},    //vizinhos do vértice 0
        {2},    //vizinhos do vértice 1 
        {3},    //vizinhos do vértice 2
        {0}      //vizinhos do vértice 3
    };

    vector<int> distancias_origem = BFS_distancias(lista_adjacencia, 0);

    for(int i = 0; i < n; i++){

        cout << "Vertice: " << i << " Distancia da origem: " << distancias_origem[i] << endl;
    }

    return 0;
}