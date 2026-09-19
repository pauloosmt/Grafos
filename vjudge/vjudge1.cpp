#include <bits/stdc++.h>
using namespace std;

const int BRANCO = 0;
const int PRETO  = 1;

int H, W;
vector<string> mapa;
vector<vector<int> > cor;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void bfs(int si, int sj) {
    queue<pair<int,int> > fila;
    fila.push(make_pair(si, sj));
    cor[si][sj] = PRETO;
    char letra = mapa[si][sj];

    while (!fila.empty()) {
        int i = fila.front().first;
        int j = fila.front().second;
        fila.pop();

        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];

            if (ni < 0 || ni >= H || nj < 0 || nj >= W) continue;
            if (cor[ni][nj] == PRETO) continue;
            if (mapa[ni][nj] != letra) continue;

            cor[ni][nj] = PRETO;
            fila.push(make_pair(ni, nj));
        }
    }
}

bool comparaResultado(const pair<char,int>& a, const pair<char,int>& b) {
    if (a.second != b.second) return a.second > b.second;
    return a.first < b.first;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    for (int caso = 1; caso <= N; caso++) {
        cin >> H >> W;
        mapa.assign(H, "");
        for (int i = 0; i < H; i++) cin >> mapa[i];

        cor.assign(H, vector<int>(W, BRANCO));
        int cont[26] = {0};

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (cor[i][j] == BRANCO) {
                    cont[mapa[i][j] - 'a']++;
                    bfs(i, j);
                }
            }
        }

        vector<pair<char,int> > resultado;
        for (int k = 0; k < 26; k++)
            if (cont[k] > 0)
                resultado.push_back(make_pair((char)('a' + k), cont[k]));

        sort(resultado.begin(), resultado.end(), comparaResultado);

        cout << "World #" << caso << "\n";
        for (size_t k = 0; k < resultado.size(); k++)
            cout << resultado[k].first << ": " << resultado[k].second << "\n";
    }

    return 0;
}