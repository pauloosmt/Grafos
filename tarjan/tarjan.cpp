#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> LA;
vector<int> desc;
vector<int> low;
vector<bool> visitado;
vector<bool> naPilha;
int tempo = 0;

void dfsTarjan(int u) {
   desc[u] = low[u] = tempo;
   tempo++;
   visitado[u] = true;
   naPilha[u] = true;

   for(int v : LA[u]) {
    if(!visitado[v]) {
        dfsTarjan(v);
        low[u] = min(low[u], low[v]);
    }
    else if(naPilha[v]) {
        low[u] = min(low[u], desc[v]);
    }
   }

   naPilha[u] = false;
}

void tarjan(int n) {
    desc.assign(n, -1);
    low.assign(n, -1);
    visitado.assign(n, false);
    naPilha.assign(n, false);
    tempo = 1;

    for(int u = 0; u < n; u++) {
        if(!visitado[u]) {
            dfsTarjan(u);
        }
    }
}

int main() {
    int n = 7;
    LA.assign(n,{});

    LA[0] = {1,2};
    LA[1] = {0, 2};
    LA[2] = {0, 1};
    LA[3] = {2, 4, 5};
    LA[4] = {3, 5};
    LA[5] = {3, 4};
    LA[6] = {3};


    tarjan(n);

    cout << "vertice | desc | low" << endl;
    for (int i = 0; i < n; i++) {
        cout << "   " << i + 1 << "    |  " << desc[i] << "   |  " << low[i] << endl;
    }
}