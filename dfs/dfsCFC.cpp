#include <iostream>
#include <vector>
#include <stack>
using namespace std;

const int BRANCO = 0;
const int CINZA = 1;
const int PRETO = 2;

int n;
vector<vector<int>> LA;
vector<vector<int>> LA_T;
vector<int> cor;
vector<int> componente;
stack<int> pilhaFim;

void dfsVisit(int u) {
    cor[u] = CINZA;
    for(int v : LA[u]) {
        if(cor[v] == BRANCO) {
            dfsVisit(v);
        }
    }
    cor[u] = PRETO;
    pilhaFim.push(u);
}

void dfsVisit2(int u, int idComponente){
    cor[u] = CINZA;
    componente[u] = idComponente;
    for(int v : LA_T[u]) {
        if(cor[v] == BRANCO) {
            dfsVisit2(v, idComponente);
        }
    }
    cor[u] = PRETO;
}

void transpor() {
    LA_T.assign(n, {});
    for(int u = 0; u < n; u++) {
        for(int v: LA[u]) {
            LA_T[v].push_back(u);
        }
    }
}
   
int cfcDFS() {
    cor.assign(n, BRANCO);
    while(!pilhaFim.empty()) {
        pilhaFim.pop();
    }
    for(int u = 0; u < n; u++){
        if(cor[u] == BRANCO){
            dfsVisit(u);
        }
    }   

    transpor();

    cor.assign(n, BRANCO);
    componente.assign(n, -1);
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
    n = 6;
   LA.assign(n, {});
   LA[0] = {3, 1};
   LA[1] = {3, 2};
   LA[2] = {3, 2};
   LA[3] = {0};
   LA[4] = {};
   LA[5] = {4};

    int totalCFC = cfcDFS();

    vector<vector<int>> grupos(totalCFC);
    for (int i = 0; i < n; i++)
        grupos[componente[i]].push_back(i);

    cout << "Numero de CFCs: " << totalCFC << endl;

    for (int c = 0; c < totalCFC; c++) {
        cout << "CFC " << c << ": ";
        for (int v : grupos[c]) cout << "v" << v << " ";
        cout << endl;
    }
    
}