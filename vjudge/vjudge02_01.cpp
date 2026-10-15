#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> ii;
typedef pair<int, ii> iii;

const int INF = 1000000000;

int menor_custo(vector<vector<ii> >& adj, vector<int>& preco, int n_vertices, int capacidade, int origem, int destino) {
	vector<vector<int> > dist(n_vertices, vector<int>(capacidade + 1, INF));
	priority_queue<iii, vector<iii>, greater<iii> > fila;

	dist[origem][0] = 0;
	fila.push(iii(0, ii(origem, 0)));

	while(!fila.empty())
	{
		iii topo = fila.top();
		fila.pop();
		int custo = topo.first;
		int u = topo.second.first;
		int f = topo.second.second;

		if(custo > dist[u][f])
			continue;

		if(u == destino)
			return custo;

		if(f < capacidade)
		{
			int novo_custo = custo + preco[u];
			if(novo_custo < dist[u][f + 1])
			{
				dist[u][f + 1] = novo_custo;
				fila.push(iii(novo_custo, ii(u, f + 1)));
			}
		}

		for(size_t i = 0; i < adj[u].size(); i++)
		{
			int v = adj[u][i].first;
			int d = adj[u][i].second;
			if(f >= d && custo < dist[v][f - d])
			{
				dist[v][f - d] = custo;
				fila.push(iii(custo, ii(v, f - d)));
			}
		}
	}

	return -1;
}

int main() {
	int n_vertices, n_estradas;
	cin >> n_vertices >> n_estradas;

	vector<int> preco(n_vertices);
	for(int i = 0; i < n_vertices; i++)
		cin >> preco[i];

	vector<vector<ii> > adj(n_vertices);
	for(int i = 0; i < n_estradas; i++)
	{
		int u, v, d;
		cin >> u >> v >> d;
		adj[u].push_back(ii(v, d));
		adj[v].push_back(ii(u, d));
	}

	int n_consultas;
	cin >> n_consultas;
	for(int i = 0; i < n_consultas; i++)
	{
		int capacidade, origem, destino;
		cin >> capacidade >> origem >> destino;

		int resultado = menor_custo(adj, preco, n_vertices, capacidade, origem, destino);
		if(resultado == -1)
			cout << "impossible" << endl;
		else
			cout << resultado << endl;
	}

	return 0;
}