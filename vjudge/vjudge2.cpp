#include <iostream>
#include <vector>
#include <stack>
using namespace std;

const int BRANCO = 0;
const int CINZA = 1;
const int PRETO = 2;

int n, m;
vector<vector<int>> LA;
vector<vector<int>> LA_T;
vector<int> cor;
vector<int> componente;
stack<int> pilhaFim;

void dfsVisit(int u) {
    cor[u] = CINZA;
    for(int v: LA[u]) {
        if(cor[v] == BRANCO) {
            dfsVisit(v);
        }
    }
    cor[u] = PRETO;
    pilhaFim.push(u);
}

void dfsVisit2(int u, int idComponente) {
    cor[u] = CINZA;
    componente[u] = idComponente;
    for(int v: LA_T[u]){
        if(cor[v] == BRANCO) {
            dfsVisit2(v, idComponente);
        }
    }
    cor[u] = PRETO;
}

void transpor(){
    
    LA_T.assign(n + 1, {});
    for(int u = 1; u <= n; u++) {
        for(int v: LA[u]) {
            LA_T[v].push_back(u);
        }
    }
}

int cfcDFS() {
    cor.assign(n + 1, BRANCO);
    while(!pilhaFim.empty()) {
        pilhaFim.pop();
    }
    for(int u = 1; u <= n; u++){
        if(cor[u] == BRANCO) {
            dfsVisit(u);
        }
    }

    transpor();

    cor.assign(n + 1, BRANCO);
    componente.assign(n + 1, -1);
    int idComponente = 0;
    while(!pilhaFim.empty()) {
        int u = pilhaFim.top();
        pilhaFim.pop();
        if(cor[u] == BRANCO) {
            dfsVisit2(u, idComponente);
            idComponente++;
        }
    }

    return idComponente;
}

int main() {
   
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int casos;
    cin >> casos;

    while(casos--) {
        cin >> n >> m;
        LA.assign(n + 1, {});

        
        vector<pair<int,int>> arestas(m);
        for(int i = 0; i < m; i++) {
            int x, y;
            cin >> x >> y;
            LA[x].push_back(y);
            arestas[i] = {x, y};
        }

        int totalCFC = cfcDFS();

        
        vector<int> grauEntrada(totalCFC, 0);
        for(auto& [x, y] : arestas) {
            if(componente[x] != componente[y]) {
                grauEntrada[componente[y]]++;
            }
        }

        int resposta = 0;
        for(int c = 0; c < totalCFC; c++) {
            if(grauEntrada[c] == 0) {
                resposta++;
            }
        }

        cout << resposta << endl;
    }

    return 0;
}

