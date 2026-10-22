// =====================================================================
// ALGORITMO DE PRIM (MATRIZ DE ADJACENCIA) - Floresta Geradora Minima
// =====================================================================
//
// DIFERENCAS PARA O prim01.cpp:
//   1. O grafo e guardado em MATRIZ de adjacencia, nao em lista.
//   2. Em vez de fila de prioridade (heap), usamos o Prim "classico" O(V^2):
//      um vetor 'custo_ate' com o menor custo conhecido para ligar cada
//      vertice a arvore, e a cada passo procuramos o menor valor varrendo o vetor.
//      Com matriz isso e natural e, em grafos densos, e tao bom quanto o heap.
//   3. Se o grafo for desconexo, o prim01 devolve -1 ("nao existe AGM"). Aqui
//      devolvemos a FLORESTA GERADORA MINIMA (uma AGM por componente conexa)
//      e dizemos quais vertices pertencem a cada componente.
//
// IDEIA DO ALGORITMO DE PRIM (guloso):
//   1. Comece em um vertice qualquer (a raiz). Ele e a "arvore" inicial.
//   2. Olhe todas as arestas que saem da arvore para vertices FORA dela.
//   3. Escolha a de MENOR peso e adicione-a (o vertice novo entra na arvore).
//   4. Repita ate nao haver mais nenhuma aresta saindo da arvore.
//
// COMO OBTER A FLORESTA: quando o passo 4 termina e ainda existem vertices
// fora da arvore, eles nao sao alcancaveis a partir da raiz: pertencem a
// OUTRA componente. Entao escolhemos um deles como nova raiz e rodamos o Prim
// de novo, abrindo uma nova arvore. Repetimos ate todos os vertices estarem
// em alguma arvore. Cada raiz iniciada = uma componente = uma arvore da floresta.
// =====================================================================

#include <iostream>
#include <vector>
using namespace std;

// Valor que significa "nao existe aresta" na matriz (e "ainda inalcancavel"
// no vetor custo_ate). Precisa ser maior que qualquer peso possivel.
const int INF = 1000000000;

// Executa o Prim em todas as componentes do grafo.
//   matriz      : matriz[u][v] = peso da aresta u-v, ou INF se nao existe aresta.
//                 O grafo e nao direcionado, entao matriz[u][v] == matriz[v][u].
//   n_vertices  : quantidade de vertices (numerados de 0 a n_vertices-1).
//   pai         : (saida) pai[v] = vertice que "puxou" v para a arvore, ou seja,
//                 a aresta (pai[v], v) faz parte da floresta. Fica -1 nas raizes.
//   componentes : (saida) componentes[c] = vertices da componente c. A primeira
//                 posicao de cada lista e a raiz daquela arvore.
// Retorna o custo total da floresta (soma dos pesos das arestas escolhidas).
//   Obs.: se o grafo for conexo, ha 1 so componente e a floresta e a propria AGM.
int prim_floresta(vector<vector<int> >& matriz, int n_vertices,
                  vector<int>& pai, vector<vector<int> >& componentes) {
    // na_arvore[v] = true quando v ja foi incluido em alguma arvore da floresta.
    vector<bool> na_arvore(n_vertices, false);
    // custo_ate[v] = menor peso de uma aresta ligando v a arvore ATUAL.
    // INF = nenhuma aresta conhecida ainda.
    vector<int> custo_ate(n_vertices, INF);
    // Ninguem tem pai ainda (-1 = "sem pai"). As raizes ficam com -1 para sempre.
    pai.assign(n_vertices, -1);

    int custo_total = 0;   // soma dos pesos das arestas escolhidas

    // Tenta cada vertice como possivel raiz. Se ele ja esta em uma arvore,
    // pertence a uma componente que ja foi explorada: pula. Senao, ele e o
    // primeiro vertice de uma componente nova.
    for(int raiz = 0; raiz < n_vertices; raiz++) {
        if(na_arvore[raiz]) {
            continue;
        }

        // Abre uma nova componente. A raiz "chega" a arvore com custo 0.
        componentes.push_back(vector<int>());
        int c = (int) componentes.size() - 1;
        custo_ate[raiz] = 0;

        // Roda o Prim a partir da raiz. Cada volta coloca UM vertice na arvore.
        while(true) {
            // PASSO 1: entre os vertices FORA da arvore, acha o mais barato de
            // ligar a ela (menor custo_ate). Varre o vetor inteiro: e o "heap" da matriz.
            int u = -1;
            for(int v = 0; v < n_vertices; v++) {
                if(!na_arvore[v] && custo_ate[v] < INF && (u == -1 || custo_ate[v] < custo_ate[u])) {
                    u = v;
                }
            }

            // Nenhum vertice fora da arvore e alcancavel: esta componente terminou.
            if(u == -1) {
                break;
            }

            // PASSO 2: u entra na arvore pela aresta (pai[u], u), ja registrada
            // em pai[] quando custo_ate[u] foi atualizado (a raiz fica com -1).
            na_arvore[u] = true;
            componentes[c].push_back(u);
            custo_total += custo_ate[u];   // a raiz soma 0

            // PASSO 3: com u na arvore, novas arestas ficaram disponiveis.
            // Percorre a LINHA u da matriz (todos os possiveis vizinhos de u).
            for(int v = 0; v < n_vertices; v++) {
                // So interessa: existir aresta u-v, v estar FORA da arvore (senao
                // criaria ciclo) e essa aresta ser mais barata que a melhor
                // conhecida ate agora para chegar em v.
                if(matriz[u][v] != INF && !na_arvore[v] && matriz[u][v] < custo_ate[v]) {
                    custo_ate[v] = matriz[u][v];   // novo melhor custo para chegar em v
                    pai[v] = u;                    // chegando em v vindo de u
                }
            }
        }
    }

    return custo_total;
}

// Adiciona uma aresta NAO DIRECIONADA entre u e v com peso w.
// Como pode-se ir de u para v e de v para u, preenchemos as duas posicoes.
void adicionar_aresta(vector<vector<int> >& matriz, int u, int v, int w) {
    matriz[u][v] = w;
    matriz[v][u] = w;
}

// Imprime o resultado de forma legivel.
// char('a' + numero) converte 0->'a', 1->'b', ... so para imprimir de forma legivel.
void imprimir_resultado(const vector<vector<int> >& matriz, const vector<int>& pai,
                        const vector<vector<int> >& componentes, int custo_total) {
    cout << "Arestas da floresta:" << endl;
    for(size_t v = 0; v < pai.size(); v++) {
        if(pai[v] != -1)   // as raizes nao tem pai, entao nao tem aresta para imprimir
            // O peso e lido direto da matriz: e so olhar a posicao [pai][v].
            cout << " " << char('a' + pai[v]) << " - " << char('a' + (int) v)
                << " (peso " << matriz[pai[v]][v] << ")" << endl;
    }
    cout << "Custo total: " << custo_total << endl;

    cout << "Numero de componentes: " << componentes.size();
    if(componentes.size() == 1)
        cout << " (grafo conexo: a floresta e uma AGM)";
    cout << endl;
    for(size_t c = 0; c < componentes.size(); c++) {
        cout << " Componente " << c + 1 << " (raiz " << char('a' + componentes[c][0]) << "):";
        for(size_t j = 0; j < componentes[c].size(); j++)
            cout << " " << char('a' + componentes[c][j]);
        cout << endl;
    }
}

int main() {
    // ---------- Exemplo 1: grafo conexo (o mesmo do prim01) ----------
    // 9 vertices numerados de 0 a 8, que correspondem as letras a..i
    // (0 = a, 1 = b, 2 = c, ...). Comeca sem nenhuma aresta (tudo INF).
    int n_vertices = 9;
    vector<vector<int> > matriz(n_vertices, vector<int>(n_vertices, INF));

    // Cada linha cria uma aresta: adicionar_aresta(matriz, vertice1, vertice2, peso)
    adicionar_aresta(matriz, 0, 1, 4);   // a-b
    adicionar_aresta(matriz, 0, 7, 8);   // a-h
    adicionar_aresta(matriz, 1, 2, 8);   // b-c
    adicionar_aresta(matriz, 1, 7, 11);  // b-h
    adicionar_aresta(matriz, 2, 3, 7);   // c-d
    adicionar_aresta(matriz, 2, 5, 4);   // c-f
    adicionar_aresta(matriz, 2, 8, 2);   // c-i
    adicionar_aresta(matriz, 3, 4, 9);   // d-e
    adicionar_aresta(matriz, 3, 5, 14);  // d-f
    adicionar_aresta(matriz, 4, 5, 10);  // e-f
    adicionar_aresta(matriz, 5, 6, 2);   // f-g
    adicionar_aresta(matriz, 6, 7, 1);   // g-h
    adicionar_aresta(matriz, 6, 8, 6);   // g-i
    adicionar_aresta(matriz, 7, 8, 7);   // h-i

    vector<int> pai;
    vector<vector<int> > componentes;
    int custo_total = prim_floresta(matriz, n_vertices, pai, componentes);

    cout << "Exemplo 1 - grafo conexo" << endl;
    imprimir_resultado(matriz, pai, componentes, custo_total);

    // ---------- Exemplo 2: grafo desconexo ----------
    // Vertices a-b, c-d-e e f (isolado): tres componentes, sem arestas entre elas.
    int n_desconexo = 6;
    vector<vector<int> > matriz2(n_desconexo, vector<int>(n_desconexo, INF));
    adicionar_aresta(matriz2, 0, 1, 5);   // a-b
    adicionar_aresta(matriz2, 2, 3, 3);   // c-d
    adicionar_aresta(matriz2, 3, 4, 1);   // d-e
    adicionar_aresta(matriz2, 2, 4, 6);   // c-e (forma ciclo c-d-e: nao entra na floresta)

    vector<int> pai2;
    vector<vector<int> > componentes2;
    int custo2 = prim_floresta(matriz2, n_desconexo, pai2, componentes2);

    cout << endl << "Exemplo 2 - grafo desconexo" << endl;
    imprimir_resultado(matriz2, pai2, componentes2, custo2);

    return 0;
}
