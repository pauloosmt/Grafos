// =====================================================================
// ALGORITMO DE KRUSKAL - Floresta Geradora Minima e suas componentes
// =====================================================================
//
// DIFERENCA PARA O kruskall01.cpp:
// No kruskall01, se o grafo for desconexo, o Kruskal devolve -1 ("nao existe
// AGM"). Aqui, em vez de desistir, aceitamos o resultado como ele e: uma
// FLORESTA GERADORA MINIMA, ou seja, uma AGM para cada componente conexa do
// grafo. Alem das arestas e do custo, a funcao tambem devolve QUAIS vertices
// pertencem a cada componente.
//
// IDEIA DO ALGORITMO (a mesma do Kruskal normal):
//   1. Ordene TODAS as arestas da mais barata para a mais cara.
//   2. Percorra as arestas nessa ordem. Para cada uma:
//        - se ela liga duas "ilhas" diferentes, ACEITE e junte as ilhas;
//        - se os vertices ja estao na mesma ilha, REJEITE (fecharia um ciclo).
//   3. Percorra TODAS as arestas (nao ha o que esperar de N-1 aceitas, pois
//      talvez nunca cheguemos nisso se o grafo for desconexo).
//
// QUANTAS COMPONENTES? Comecamos com N ilhas e cada aresta aceita reduz esse
// numero em exatamente 1. Logo:
//   numero_de_componentes = N_vertices - arestas_aceitas
// Se o resultado for 1, o grafo e conexo e a floresta e uma unica AGM.
//
// COMO DESCOBRIR QUEM ESTA EM QUAL COMPONENTE? No final, as ilhas do
// Union-Find SAO as componentes: dois vertices estao na mesma componente se,
// e somente se, tem o mesmo representante (achar). Basta agrupar os vertices
// pelo representante.
// =====================================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Uma aresta do grafo: liga os vertices u e v e tem um custo (peso).
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
    // Senao, subimos ate a raiz, fazendo "compressao de caminho": ligamos x
    // direto na raiz para que as proximas buscas sejam quase instantaneas.
    pai_conjunto[x] = achar(pai_conjunto[x]);
    return pai_conjunto[x];
}

// Junta o conjunto de a com o conjunto de b.
// Retorna true se juntou, ou false se a e b ja estavam no mesmo conjunto.
bool unir(int a, int b) {
    // Troca a e b pelos representantes dos seus conjuntos.
    a = achar(a);
    b = achar(b);
    // Mesmo representante = mesma ilha = ja estao conectados (formaria ciclo).
    if(a == b) {
        return false;
    }

    // "Uniao por tamanho": garantimos que 'a' e o conjunto MAIOR e pendurar o
    // menor nele mantem as arvores baixas e a busca (achar) rapida.
    if(tamanho_conjunto[a] < tamanho_conjunto[b]) {
        swap(a,b);
    }

    // O representante do conjunto menor passa a apontar para o do maior.
    pai_conjunto[b] = a;
    tamanho_conjunto[a] += tamanho_conjunto[b];
    return true;
}

// ---------------------------------------------------------------------
// Executa o Kruskal devolvendo a floresta geradora minima.
//   arestas     : lista com todas as arestas do grafo (sera reordenada aqui dentro).
//   n_vertices  : quantidade de vertices (numerados de 0 a n_vertices-1).
//   floresta    : (saida) as arestas aceitas, na ordem em que foram aceitas.
//   componentes : (saida) componentes[c] e a lista de vertices da componente c.
//                 Cada componente corresponde a uma arvore da floresta.
// Retorna o custo total da floresta (soma dos pesos das arestas aceitas).
//   Obs.: se o grafo for conexo, a floresta tem 1 so componente e e a propria AGM.
// ---------------------------------------------------------------------
int kruskal_floresta(vector<Aresta>& arestas, int n_vertices,
                     vector<Aresta>& floresta, vector<vector<int> >& componentes) {
    // PASSO 1: ordena as arestas da mais barata para a mais cara.
    stable_sort(arestas.begin(), arestas.end(), menor_peso);
    // Cada vertice comeca sozinho na sua ilha.
    inicializar_conjuntos(n_vertices);

    int custo_total = 0;   // soma dos pesos das arestas aceitas

    // PASSO 2: percorre TODAS as arestas, da mais barata para a mais cara.
    // Nao ha "break" por N-1 arestas: num grafo desconexo isso nunca acontece,
    // e num conexo as arestas restantes seriam todas rejeitadas de qualquer jeito.
    for(size_t i = 0; i < arestas.size(); i++) {
        // unir() junta as ilhas e devolve true se u e v estavam em ilhas
        // diferentes; devolve false se ja estavam na mesma (formaria ciclo).
        if(unir(arestas[i].u, arestas[i].v)) {
            floresta.push_back(arestas[i]);     // aceita a aresta na floresta
            custo_total += arestas[i].peso;     // soma o custo dela
        }
    }

    // PASSO 3: agrupa os vertices por componente.
    // indice_da_componente[r] guarda qual posicao de 'componentes' pertence ao
    // representante r (-1 = esse representante ainda nao apareceu).
    vector<int> indice_da_componente(n_vertices, -1);
    for(int v = 0; v < n_vertices; v++) {
        int raiz = achar(v);                    // representante da ilha de v
        if(indice_da_componente[raiz] == -1) {
            // Primeiro vertice desta ilha: abre uma nova componente.
            indice_da_componente[raiz] = (int) componentes.size();
            componentes.push_back(vector<int>());
        }
        componentes[indice_da_componente[raiz]].push_back(v);
    }

    return custo_total;
}

// Imprime o resultado de forma legivel.
// char('a' + numero) converte 0->'a', 1->'b', ... so para imprimir de forma legivel.
void imprimir_resultado(const vector<Aresta>& floresta,
                        const vector<vector<int> >& componentes, int custo_total) {
    cout << "Arestas da floresta (na ordem em que foram aceitas):" << endl;
    for(size_t i = 0; i < floresta.size(); i++)
        cout << " " << char('a' + floresta[i].u) << " - " << char('a' + floresta[i].v)
            << " (peso " << floresta[i].peso << ")" << endl;
    cout << "Custo total: " << custo_total << endl;

    cout << "Numero de componentes: " << componentes.size();
    if(componentes.size() == 1)
        cout << " (grafo conexo: a floresta e uma AGM)";
    cout << endl;
    for(size_t c = 0; c < componentes.size(); c++) {
        cout << " Componente " << c + 1 << ":";
        for(size_t j = 0; j < componentes[c].size(); j++)
            cout << " " << char('a' + componentes[c][j]);
        cout << endl;
    }
}

int main() {
    // ---------- Exemplo 1: grafo conexo (o mesmo do kruskall01, CLRS) ----------
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

    vector<Aresta> floresta;
    vector<vector<int> > componentes;
    int custo_total = kruskal_floresta(arestas, n_vertices, floresta, componentes);

    cout << "Exemplo 1 - grafo conexo" << endl;
    imprimir_resultado(floresta, componentes, custo_total);

    // ---------- Exemplo 2: grafo desconexo ----------
    // Vertices a-b, c-d-e e f (isolado): tres componentes, sem arestas entre elas.
    int n_desconexo = 6;
    vector<Aresta> arestas_desconexo;
    arestas_desconexo.push_back({0, 1, 5});   // a-b
    arestas_desconexo.push_back({2, 3, 3});   // c-d
    arestas_desconexo.push_back({3, 4, 1});   // d-e
    arestas_desconexo.push_back({2, 4, 6});   // c-e (forma ciclo c-d-e: sera rejeitada)

    vector<Aresta> floresta2;
    vector<vector<int> > componentes2;
    int custo2 = kruskal_floresta(arestas_desconexo, n_desconexo, floresta2, componentes2);

    cout << endl << "Exemplo 2 - grafo desconexo" << endl;
    imprimir_resultado(floresta2, componentes2, custo2);

    return 0;
}
