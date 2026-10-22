// =====================================================================
// ALGORITMO DE KRUSKAL - Arvore Geradora Minima (AGM)
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
// IDEIA DO ALGORITMO DE KRUSKAL (guloso):
//   1. Ordene TODAS as arestas do grafo da mais barata para a mais cara.
//   2. Percorra as arestas nessa ordem. Para cada uma:
//        - se ela liga dois vertices que AINDA NAO estao conectados entre si
//          (por nenhum caminho de arestas ja aceitas), ACEITE a aresta;
//        - se eles ja estao conectados, REJEITE (aceitar fecharia um ciclo).
//   3. Pare quando tiver N-1 arestas aceitas.
//
// Diferenca para o Prim: o Prim faz UMA arvore crescer a partir da raiz.
// O Kruskal comeca com varias "ilhas" (cada vertice sozinho) e vai
// juntando as ilhas com as arestas mais baratas ate sobrar uma so.
//
// QUANTAS ARVORES TERA A FLORESTA GERADORA MINIMA? Comecamos com N ilhas
// (uma por vertice), e cada aresta aceita junta duas ilhas em uma so,
// reduzindo o numero de ilhas em exatamente 1. Logo, depois de aceitar
// "arestas_aceitas" arestas, sobram:
//   numero_de_componentes = N_vertices - arestas_aceitas
// Se o grafo for conexo, arestas_aceitas = N-1 e sobra 1 so componente
// (uma arvore). Se for desconexo, sobra mais de um componente: a floresta
// geradora minima tem exatamente "numero_de_componentes" arvores.
//
// COMO SABER SE DOIS VERTICES JA ESTAO CONECTADOS? Usamos a estrutura
// "Union-Find" (conjuntos disjuntos), implementada abaixo: cada ilha e um
// conjunto, e conseguimos perguntar "a e b estao na mesma ilha?" e
// "junte a ilha de a com a de b" de forma muito rapida.
// =====================================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Uma aresta do grafo: liga os vertices u e v e tem um custo (peso).
// Aqui guardamos so a lista de arestas, sem lista de adjacencia, porque o
// Kruskal so precisa percorrer todas as arestas em ordem de peso.
struct Aresta {
    int u, v, peso;
};

// Funcao de comparacao usada na ordenacao: a vem antes de b se for mais barata.
bool menor_peso(const Aresta& a, const Aresta& b) {
    return a.peso < b.peso;
}

// ---------------------------------------------------------------------
// UNION-FIND (conjuntos disjuntos)
// ---------------------------------------------------------------------
// Cada conjunto (ilha) e representado como uma pequena arvore, e o
// "representante" do conjunto e a raiz dessa arvore.
//   pai_conjunto[x]     : o "pai" de x dentro da arvore do conjunto.
//                         Se pai_conjunto[x] == x, entao x e a raiz (o representante).
//   tamanho_conjunto[r] : quantos vertices tem o conjunto cujo representante e r.
// Dois vertices estao na mesma ilha se, e somente se, tem o MESMO representante.
vector<int> pai_conjunto;
vector<int> tamanho_conjunto;

// Comeca com cada vertice sozinho em seu proprio conjunto (nenhuma aresta aceita ainda).
void inicializar_conjuntos(int n_vertices) {
    pai_conjunto.assign(n_vertices, 0);
    tamanho_conjunto.assign(n_vertices, 1);   // cada conjunto tem 1 vertice
    for(int i = 0; i < n_vertices; i++) {
        pai_conjunto[i] = i;                  // cada um e o proprio representante
    }
}

// Descobre o representante (a raiz) do conjunto onde x esta.
int achar(int x) {
    // Se x e o proprio pai, ele e a raiz: achamos o representante.
    if(pai_conjunto[x] == x) {
        return x;
    }
    // Senao, subimos ate a raiz. Aproveitamos para fazer "compressao de
    // caminho": ligamos x direto na raiz, assim as proximas buscas por x
    // (e por quem passa por ele) ficam quase instantaneas.
    pai_conjunto[x] = achar(pai_conjunto[x]);
    return pai_conjunto[x];
}

// Junta o conjunto de a com o conjunto de b.
// Retorna true se juntou, ou false se a e b ja estavam no mesmo conjunto.
bool unir(int a, int b) {
    // Troca a e b pelos representantes dos seus conjuntos.
    a = achar(a);
    b = achar(b);
    // Mesmo representante = mesma ilha = ja estao conectados.
    // Ligar os dois de novo formaria um ciclo, entao nao faz nada.
    if(a == b) {
        return false;
    }

    // "Uniao por tamanho": garantimos que 'a' e o conjunto MAIOR. Pendurar o
    // menor no maior mantem as arvores baixas e a busca (achar) rapida.
    if(tamanho_conjunto[a] < tamanho_conjunto[b]) {
        swap(a,b);
    }

    // O representante do conjunto menor passa a apontar para o do maior:
    // os dois conjuntos agora sao um so.
    pai_conjunto[b] = a;
    tamanho_conjunto[a] += tamanho_conjunto[b];
    return true;

}

// ---------------------------------------------------------------------
// Executa o algoritmo de Kruskal.
//   arestas    : lista com todas as arestas do grafo (sera reordenada aqui dentro).
//   n_vertices : quantidade de vertices (numerados de 0 a n_vertices-1).
//   agm        : (saida) as arestas escolhidas para a AGM, na ordem em que foram aceitas.
// Retorna o custo total da AGM, ou -1 se o grafo for desconexo (nao existe AGM).
//   Obs.: no caso desconexo, 'agm' fica com uma "floresta" (varias arvores separadas).
// ---------------------------------------------------------------------
int kruskal(vector<Aresta>& arestas, int n_vertices, vector<Aresta>& agm) {
    // PASSO 1: ordena as arestas da mais barata para a mais cara.
    // (stable_sort mantem a ordem original entre arestas de mesmo peso.)
    stable_sort(arestas.begin(), arestas.end(), menor_peso);
    // Cada vertice comeca sozinho na sua ilha.
    inicializar_conjuntos(n_vertices);

    int custo_total = 0;   // soma dos pesos das arestas aceitas

    // PASSO 2: percorre as arestas da mais barata para a mais cara.
    for(size_t i = 0; i< arestas.size(); i++) {
        // unir() faz as duas coisas de uma vez:
        //  - se u e v estavam em ilhas diferentes, junta as ilhas e devolve true
        //    (a aresta e util: conecta coisas novas);
        //  - se ja estavam na mesma ilha, devolve false (aresta descartada,
        //    pois formaria um ciclo).
        if(unir(arestas[i].u, arestas[i].v)) {
            agm.push_back(arestas[i]);          // aceita a aresta na AGM
            custo_total += arestas[i].peso;     // soma o custo dela

            // PASSO 3: uma arvore com N vertices tem exatamente N-1 arestas.
            // Ao chegar nisso, tudo ja esta conectado: nao precisa olhar o resto.
            if((int) agm.size() == n_vertices - 1)
                break;
        }
    }

    // Se terminamos as arestas e nao chegamos a N-1 aceitas, sobraram ilhas
    // separadas que nenhuma aresta consegue ligar: o grafo e desconexo e
    // nao existe arvore geradora.
    if((int) agm.size() != n_vertices -1) {
        return -1;
    }

    return custo_total;
}

int main() {
	// ---------- Exemplo 1: grafo conexo (o mesmo do slide "Esquema geral", CLRS) ----------
	// Vertices a..i correspondem a 0..8 (0 = a, 1 = b, 2 = c, ...).
	int n_vertices = 9;
	vector<Aresta> arestas;
	// Cada linha cria uma aresta no formato {vertice1, vertice2, peso}.
	arestas.push_back({0, 1, 4});    // a-b
	arestas.push_back({0, 7, 8});    // a-h
	arestas.push_back({1, 2, 8});    // b-c
	arestas.push_back({1, 7, 11});   // b-h
	arestas.push_back({2, 3, 7});    // c-d
	arestas.push_back({2, 5, 4});    // c-f
	arestas.push_back({2, 8, 2});    // c-i
	arestas.push_back({3, 4, 9});    // d-e
	arestas.push_back({3, 5, 14});   // d-f
	arestas.push_back({4, 5, 10});   // e-f
	arestas.push_back({5, 6, 2});    // f-g
	arestas.push_back({6, 7, 1});    // g-h
	arestas.push_back({6, 8, 6});    // g-i
	arestas.push_back({7, 8, 7});    // h-i

	vector<Aresta> agm;
	int custo_total = kruskal(arestas, n_vertices, agm);

	// Exibe as arestas aceitas, na ordem em que o algoritmo as escolheu, e o custo total.
	// char('a' + numero) converte 0->'a', 1->'b', ... so para imprimir de forma legivel.
	cout << "Arestas da AGM (na ordem em que foram aceitas):" << endl;
	for(size_t i = 0; i < agm.size(); i++)
		cout << " " << char('a' + agm[i].u) << " - " << char('a' + agm[i].v)
			<< " (peso " << agm[i].peso << ")" << endl;
	cout << "Custo total: " << custo_total << endl;

	// ---------- Exemplo 2: grafo desconexo ----------
	// Vertices 0-1 estao ligados entre si, e 2-3 entre si, mas nao ha aresta entre os dois grupos.
	// Logo, o Kruskal aceita as 2 arestas, mas precisaria de 3 (N-1) para conectar tudo: nao existe AGM.
	int n_desconexo = 4;
	vector<Aresta> arestas_desconexo;
	arestas_desconexo.push_back({0, 1, 5});
	arestas_desconexo.push_back({2, 3, 3});

	vector<Aresta> floresta;
	int resultado = kruskal(arestas_desconexo, n_desconexo, floresta);
	cout << endl << "Grafo desconexo: ";
	if(resultado == -1)   // -1 e o sinal que a funcao kruskal usa para "grafo desconexo"
		cout << "nao existe arvore geradora (floresta com " << floresta.size() << " arestas)" << endl;
	else
		cout << "custo total " << resultado << endl;

	return 0;
}
