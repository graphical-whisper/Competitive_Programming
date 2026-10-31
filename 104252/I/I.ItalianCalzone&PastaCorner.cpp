#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C;
    if (!(cin >> R >> C)) return 0;

    int N = R * C;
    vector<vector<int>> grid(R, vector<int>(C));

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            cin >> grid[r][c];
        }
    }

    vector<vector<int>> adj(N + 1);
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            int u = grid[r][c];
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                    int v = grid[nr][nc];
                    if (v > u) {
                        adj[u].push_back(v);
                    }
                }
            }
        }
    }

    static bitset<10001> reach[10001];

    int max_dishes = 0;
    for (int val = N; val >= 1; val--) {
        reach[val].set(val);
        for (int nxt : adj[val]) {
            reach[val] |= reach[nxt];
        }
        max_dishes = max(max_dishes, (int)reach[val].count());
    }

    cout << max_dishes << "\n";
    return 0;
}

