#include<iostream>
using namespace std;

// Calcula o fecho transitivo de um grafo direcionado usando o algoritmo de Warshall.
// A matriz resultante indica, para cada par (i, j), se existe algum caminho
// (direto ou passando por vertices intermediarios) de i ate j.
// matriz_adjacencia: matriz n_vertices x n_vertices com 1 onde ha aresta direta e 0 caso contrario.
// Retorna uma nova matriz (alocada dinamicamente) com a alcancabilidade entre todos os vertices.
int** warshall(int** matriz_adjacencia, int n_vertices) {
	// Aloca a matriz de resultado e a inicializa como uma copia da matriz de adjacencia original,
	// ja que inicialmente a unica forma de "alcancar" um vertice e por meio de uma aresta direta.
	int** matriz_warshall = new int*[n_vertices];
	for(int i = 0; i < n_vertices; i++)
	{
		matriz_warshall[i] = new int[n_vertices];
		for(int j = 0; j < n_vertices; j++)
			matriz_warshall[i][j] = matriz_adjacencia[i][j];
	}

	// Nucleo do algoritmo de Warshall: para cada vertice k, verifica se ele pode
	// servir de "ponte" entre um vertice i e um vertice j.
	// k = vertice intermediario considerado na iteracao atual
	// i = vertice de origem
	// j = vertice de destino
	// Se i ja alcanca k, e k alcanca j, entao i passa a alcancar j tambem.
	for(int k = 0; k < n_vertices; k++)
		for(int i = 0; i < n_vertices; i++)
			for(int j = 0; j < n_vertices; j++)
				matriz_warshall[i][j] = matriz_warshall[i][j] || (matriz_warshall[i][k] && matriz_warshall[k][j]);

	return matriz_warshall;
}

int main() {
	// Quantidade de vertices do grafo (vertices 0, 1, 2 e 3).
	int n_vertices = 4;

	// Aloca dinamicamente a matriz de adjacencia n_vertices x n_vertices.
	int** matriz_adjacencia = new int*[n_vertices];
	for(int i = 0; i < n_vertices; i++) matriz_adjacencia[i] = new int[n_vertices];

	// Define as arestas direcionadas do grafo de exemplo:
	// 0 -> 2, 2 -> 1, 1 -> 0, 1 -> 3
	matriz_adjacencia[0][2] = 1;
	matriz_adjacencia[2][1] = 1;
	matriz_adjacencia[1][0] = 1;
	matriz_adjacencia[1][3] = 1;


	// Aplica o algoritmo de Warshall sobre a matriz de adjacencia para obter
	// a matriz de alcancabilidade (fecho transitivo) do grafo.
	int** matriz_warshall = warshall(matriz_adjacencia, n_vertices);

	// Exibe, para cada vertice, quais outros vertices ele consegue alcancar
	// (seja por aresta direta, seja atraves de outros vertices).
	cout << "alcancabilidade do grafo (direta e transitiva)" << endl;
	for(int i = 0; i < n_vertices; i++) {
		cout << "vertice " << i + 1 << ": ";
		for(int j = 0; j < n_vertices; j++) {
			if(matriz_warshall[i][j])
				cout << j + 1 << " ";
		}
		cout << endl;
	}

	return 0;
}
