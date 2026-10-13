// =====================================================================
// ALGORITMO DE PRIM - Arvore Geradora Minima (AGM)
// =====================================================================
//
// O QUE E UMA AGM?
// Imagine varias cidades e estradas possiveis entre elas, cada estrada com
// um custo (o "peso"). Voce quer que TODAS as cidades fiquem conectadas
// gastando o MINIMO possivel. A solucao e um conjunto de estradas que:
//   - conecta todas as cidades;
//   - nao tem ciclos (nao ha estrada sobrando);
//   - tem a menor soma de pesos possivel.
// Isso e a Arvore Geradora Minima. Com N vertices ela sempre tem N-1 arestas.
//
// IDEIA DO ALGORITMO DE PRIM (guloso):
//   1. Comece em um vertice qualquer (a raiz). Ele e a "arvore" inicial.
//   2. Olhe todas as arestas que saem da arvore para vertices FORA dela.
//   3. Escolha a de MENOR peso e adicione-a (o vertice novo entra na arvore).
//   4. Repita ate todos os vertices estarem na arvore.
// Ou seja: a arvore vai "crescendo" sempre pela aresta mais barata disponivel.
//
// COMO IMPLEMENTAMOS: usamos uma fila de prioridade (heap) que sempre nos
// entrega a aresta de menor peso entre as candidatas, sem precisar procurar
// em todas toda vez.
// =====================================================================

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// ii  = par (a, b) de inteiros.
//       Usado na lista de adjacencia como (vizinho, peso da aresta).
typedef pair<int,int> ii;
// iii = trio (a, (b, c)).
//       Usado na fila de prioridade como (peso, (vertice destino, vertice de origem)).
//       O PESO fica na frente de proposito: o pair compara primeiro o campo
//       'first', entao a fila ordena as arestas pelo peso.
typedef pair<int, ii> iii;



// Executa o algoritmo de Prim.
//   adj         : grafo como lista de adjacencia. adj[u] = lista de (vizinho, peso)
//                 de todas as arestas que saem de u.
//   n_vertices  : quantidade de vertices (numerados de 0 a n_vertices-1).
//   raiz        : vertice por onde a arvore comeca.
//   pai         : (saida) pai[v] = vertice que "puxou" v para a arvore, ou seja,
//                 a aresta (pai[v], v) faz parte da AGM. Guarda a arvore resultante.
// Retorna o custo total da AGM, ou -1 se o grafo for desconexo (nao existe AGM).
int prim(vector<vector<ii>>& adj, int n_vertices, int raiz, vector<int>& pai) {
    // na_arvore[v] = true quando v ja foi incluido na AGM. Comeca tudo false.
    vector<bool> na_arvore(n_vertices, false);
    // Ninguem tem pai ainda (-1 = "sem pai"). A raiz ficara com -1 para sempre.
    pai.assign(n_vertices, -1);

    // Fila de prioridade que entrega SEMPRE a aresta de MENOR peso.
    // (O priority_queue do C++ por padrao entrega o MAIOR; o "greater" inverte isso.)
    // Cada item: (peso, (vertice destino, vertice de origem)).
    priority_queue<iii, vector<iii>, greater<iii>> fila;

    // Comecamos "entrando" na raiz. Ela nao chegou por aresta nenhuma,
    // entao usamos peso 0 e origem -1 (nao existe).
    fila.push(iii(0, ii(raiz, -1)));

    int custo_total = 0;     // soma dos pesos das arestas escolhidas
    int qtd_na_arvore = 0;   // quantos vertices ja entraram (para detectar grafo desconexo)

    // Enquanto houver arestas candidatas para analisar...
    while(!fila.empty()) {
        // Pega a aresta mais barata de todas as candidatas e a tira da fila.
        iii topo = fila.top();
        fila.pop();
        int peso = topo.first;          // custo dessa aresta
        int u = topo.second.first;      // vertice para onde ela leva
        int origem = topo.second.second;// vertice de onde ela veio (que ja esta na arvore)

        // Pode acontecer de u ja ter entrado na arvore por outra aresta mais
        // barata (u pode ter sido colocado na fila varias vezes, por vizinhos
        // diferentes). Se ja esta na arvore, essa aresta seria redundante
        // (fecharia um ciclo), entao ignoramos.
        if(na_arvore[u]){
            continue;
        }

        // Aqui u ainda NAO estava na arvore e esta e a aresta mais barata que
        // chega nele: ela faz parte da AGM. Colocamos u na arvore.
        na_arvore[u] = true;
        pai[u] = origem;        // registra a aresta (origem -> u) da AGM
        custo_total += peso;    // soma o custo dela (a raiz soma 0)
        qtd_na_arvore++;

        // Agora que u esta na arvore, novas arestas ficaram disponiveis:
        // todas as que saem de u. Percorremos os vizinhos de u...
        for(size_t i = 0; i < adj[u].size(); i++) {
            int vizinho = adj[u][i].first;       // vertice do outro lado da aresta
            int peso_aresta = adj[u][i].second;  // custo da aresta u-vizinho
            // ...e so nos interessam os que ainda estao FORA da arvore
            // (ligar a alguem que ja esta dentro criaria ciclo).
            if(!na_arvore[vizinho]) {
                // Coloca como candidata: "chegar em 'vizinho' vindo de 'u' custa 'peso_aresta'".
                fila.push(iii(peso_aresta, ii(vizinho, u)));
            }
        }
    }

    // Se a fila acabou e nem todos os vertices entraram na arvore, e porque
    // alguns vertices nao sao alcancaveis a partir da raiz: o grafo e
    // desconexo e nao existe arvore geradora que conecte tudo.
    if(qtd_na_arvore != n_vertices) {
            return -1;
    }

    return custo_total;
}


// Adiciona uma aresta NAO DIRECIONADA entre u e v com peso w.
// Como pode-se ir de u para v e de v para u, registramos nas duas listas.
void adicionar_aresta(vector<vector<ii>>& adj, int u, int v, int w) {
    adj[u].push_back(ii(v,w));
    adj[v].push_back(ii(u,w));
}

int main(){
    // ---------- Exemplo 1: grafo conexo ----------
    // 9 vertices numerados de 0 a 8, que correspondem as letras a..i
    // (0 = a, 1 = b, 2 = c, ...). adj[i] guarda os vizinhos do vertice i.
    int n_vertices = 9;
	vector<vector<ii> > adj(n_vertices);

	// Cada linha cria uma aresta: adicionar_aresta(grafo, vertice1, vertice2, peso)
	adicionar_aresta(adj, 0, 1, 4);   // a-b
	adicionar_aresta(adj, 0, 7, 8);   // a-h
	adicionar_aresta(adj, 1, 2, 8);   // b-c
	adicionar_aresta(adj, 1, 7, 11);  // b-h
	adicionar_aresta(adj, 2, 3, 7);   // c-d
	adicionar_aresta(adj, 2, 5, 4);   // c-f
	adicionar_aresta(adj, 2, 8, 2);   // c-i
	adicionar_aresta(adj, 3, 4, 9);   // d-e
	adicionar_aresta(adj, 3, 5, 14);  // d-f
	adicionar_aresta(adj, 4, 5, 10);  // e-f
	adicionar_aresta(adj, 5, 6, 2);   // f-g
	adicionar_aresta(adj, 6, 7, 1);   // g-h
	adicionar_aresta(adj, 6, 8, 6);   // g-i
	adicionar_aresta(adj, 7, 8, 7);   // h-i

	// Executa Prim a partir do vertice 0 (a).
	// (Em grafo conexo, qualquer raiz da uma AGM com o mesmo custo total.)
	int raiz = 0;
	vector<int> pai;
	int custo_total = prim(adj, n_vertices, raiz, pai);

	// Exibe as arestas da AGM (cada vertice, exceto a raiz, esta ligado ao seu pai) e o custo total.
	// char('a' + numero) converte 0->'a', 1->'b', ... so para imprimir de forma legivel.
	cout << "Arestas da AGM (raiz = " << char('a' + raiz) << "):" << endl;
	for(int v = 0; v < n_vertices; v++)
	{
		if(v != raiz)   // a raiz nao tem pai, entao nao tem aresta para imprimir
		{
			// O vetor 'pai' diz com quem v esta ligado, mas nao guarda o peso.
			// Procura o peso da aresta (pai[v], v) na lista de adjacencia do pai.
			int peso = 0;
			for(size_t i = 0; i < adj[pai[v]].size(); i++)
				if(adj[pai[v]][i].first == v)
					peso = adj[pai[v]][i].second;
			cout << " " << char('a' + pai[v]) << " - " << char('a' + v)
				<< " (peso " << peso << ")" << endl;
		}
	}
	cout << "Custo total: " << custo_total << endl;

	// ---------- Exemplo 2: grafo desconexo ----------
	// Vertices 0-1 estao ligados entre si, e 2-3 entre si, mas nao ha aresta entre os dois grupos.
	// Logo, partindo do 0 nunca conseguimos chegar no 2 e no 3: nao existe AGM.
	int n_desconexo = 4;
	vector<vector<ii> > adj_desconexo(n_desconexo);
	adicionar_aresta(adj_desconexo, 0, 1, 5);
	adicionar_aresta(adj_desconexo, 2, 3, 3);

	vector<int> pai_desconexo;
	int resultado = prim(adj_desconexo, n_desconexo, 0, pai_desconexo);
	cout << endl << "Grafo desconexo: ";
	if(resultado == -1)   // -1 e o sinal que a funcao prim usa para "grafo desconexo"
		cout << "nao existe arvore geradora (grafo desconexo)" << endl;
	else
		cout << "custo total " << resultado << endl;

	return 0;
}
