// =====================================================================
// PROBLEMA: CARGA MAXIMA ENTRE DUAS CIDADES (usando Kruskal)
// =====================================================================
//
// O PROBLEMA:
// Temos cidades ligadas por estradas, e cada estrada suporta uma carga
// maxima (em toneladas). Um caminhao que vai da cidade ORIGEM ate a cidade
// DESTINO so pode carregar o que a estrada MAIS FRACA do caminho suportar.
// Queremos escolher o caminho em que essa estrada mais fraca seja a MAIS
// FORTE possivel, e dizer quantas toneladas o caminhao consegue levar.
//   Exemplo: caminho A (estradas 10, 3, 8) -> aguenta 3 toneladas
//            caminho B (estradas  5, 6, 7) -> aguenta 5 toneladas
//            O melhor e o B: resposta = 5.
//
// A IDEIA (Kruskal "ao contrario"):
// No Kruskal normal ordenamos as arestas da MENOR para a MAIOR para montar
// uma arvore de custo minimo. Aqui ordenamos da MAIOR para a MENOR, ou seja,
// primeiro as estradas mais fortes, e vamos juntando as cidades (Union-Find).
// No instante em que ORIGEM e DESTINO passam a estar conectadas, a estrada
// que acabamos de aceitar e a mais fraca do melhor caminho: como so usamos
// estradas tao fortes quanto ela ou mais, ela e o "gargalo" e a resposta.
//
// Se acabarem as estradas e as duas cidades nunca se conectarem, nao existe
// caminho e a resposta e 0.
// =====================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

// Uma estrada: liga as cidades u e v e suporta 'peso' toneladas.
struct Aresta {
	int u;
	int v;
	int peso;
};

// Comparacao para ordenar: a vem antes de b se suporta MAIS carga
// (ordem decrescente: as estradas mais fortes primeiro).
bool maior_peso(const Aresta& a, const Aresta& b) {
	return a.peso > b.peso;
}

// ---------------------------------------------------------------------
// UNION-FIND (conjuntos disjuntos)
// ---------------------------------------------------------------------
// Serve para responder rapido "estas duas cidades ja estao conectadas?".
// Cada grupo de cidades conectadas e um conjunto, com um "representante".
//   pai_conjunto[x]     : pai de x na arvore do conjunto (se for o proprio x, x e o representante).
//   tamanho_conjunto[r] : quantas cidades tem o conjunto cujo representante e r.
// Duas cidades estao conectadas se, e so se, tem o mesmo representante.
vector<int> pai_conjunto;
vector<int> tamanho_conjunto;

// Comeca com cada cidade sozinha no seu proprio conjunto.
void inicializar_conjuntos(int n_vertices) {
	pai_conjunto.assign(n_vertices, 0);
	tamanho_conjunto.assign(n_vertices, 1);
	for(int i = 0; i < n_vertices; i++)
		pai_conjunto[i] = i;
}

// Descobre o representante do conjunto de x.
// Faz "compressao de caminho": liga x direto no representante para
// deixar as proximas buscas mais rapidas.
int achar(int x) {
	if(pai_conjunto[x] == x)
		return x;
	pai_conjunto[x] = achar(pai_conjunto[x]);
	return pai_conjunto[x];
}

// Junta o conjunto de a com o de b.
// Retorna true se juntou, false se ja estavam no mesmo conjunto.
bool unir(int a, int b) {
	a = achar(a);
	b = achar(b);
	if(a == b)
		return false;   // ja conectadas: a estrada nao traz nada de novo

	// Pendura o conjunto menor no maior para manter as arvores baixas.
	if(tamanho_conjunto[a] < tamanho_conjunto[b])
		swap(a, b);
	pai_conjunto[b] = a;
	tamanho_conjunto[a] += tamanho_conjunto[b];
	return true;
}

// As cidades vem como NOMES (texto), mas o Union-Find trabalha com numeros
// (0, 1, 2...). Esta funcao traduz: se o nome ja foi visto, devolve o numero
// dele; se e a primeira vez, da o proximo numero livre e guarda no mapa.
int obter_id(map<string, int>& ids, const string& nome) {
	map<string, int>::iterator it = ids.find(nome);
	if(it != ids.end())
		return it->second;          // nome ja conhecido
	int novo_id = ids.size();       // proximo numero livre
	ids[nome] = novo_id;
	return novo_id;
}

// Calcula a carga maxima que da para levar de 'origem' ate 'destino'.
int carga_maxima(vector<Aresta>& arestas, int n_vertices, int origem, int destino) {
	// Estradas mais fortes primeiro.
	sort(arestas.begin(), arestas.end(), maior_peso);
	// Cada cidade comeca isolada.
	inicializar_conjuntos(n_vertices);

	for(size_t i = 0; i < arestas.size(); i++)
	{
		// Tenta ligar as duas cidades da estrada atual.
		if(unir(arestas[i].u, arestas[i].v))
		{
			// Acabamos de juntar dois grupos. Origem e destino ja estao
			// no mesmo grupo? Entao esta estrada foi a que completou o
			// caminho, e ela e a mais fraca dele: e a resposta.
			if(achar(origem) == achar(destino))
				return arestas[i].peso;
		}
	}

	// Usamos todas as estradas e origem e destino nunca se conectaram.
	return 0;
}

int main() {
	int n_vertices, n_estradas;
	int cenario = 0;   // numero do caso de teste, so para a saida

	// A entrada tem varios casos de teste e termina com "0 0".
	while(cin >> n_vertices >> n_estradas && (n_vertices != 0 || n_estradas != 0))
	{
		map<string, int> ids;       // nome da cidade -> numero
		vector<Aresta> arestas;

		// Le cada estrada: cidade1 cidade2 carga.
		for(int i = 0; i < n_estradas; i++)
		{
			string nome_u, nome_v;
			int peso;
			cin >> nome_u >> nome_v >> peso;

			Aresta a;
			a.u = obter_id(ids, nome_u);
			a.v = obter_id(ids, nome_v);
			a.peso = peso;
			arestas.push_back(a);
		}

		// Por ultimo vem a cidade de partida e a de chegada.
		string nome_origem, nome_destino;
		cin >> nome_origem >> nome_destino;
		int origem = obter_id(ids, nome_origem);
		int destino = obter_id(ids, nome_destino);

		cenario++;
		cout << "Scenario #" << cenario << endl;
		cout << carga_maxima(arestas, n_vertices, origem, destino) << " tons" << endl;
		cout << endl;   // linha em branco entre os cenarios
	}

	return 0;
}
