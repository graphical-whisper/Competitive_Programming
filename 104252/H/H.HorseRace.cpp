#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool dfsKuhn(int u, const vector<vector<int>>& adj, vector<int>& matchR, vector<bool>& vis) {
    for (int v : adj[u]) {
        if (!vis[v]) {
            vis[v] = true;
            if (matchR[v] < 0 || dfsKuhn(matchR[v], adj, matchR, vis)) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<string> horse_names(N);
    unordered_map<string, int> horse_to_id;
    for (int i = 0; i < N; i++) {
        cin >> horse_names[i];
        horse_to_id[horse_names[i]] = i;
    }

    int R;
    cin >> R;

    vector<int> min_pos(N, 1);
    vector<int> race_count_at_W(N + 1, 0);
    vector<vector<int>> count_in_W(N + 1, vector<int>(N, 0));

    for (int r = 0; r < R; r++) {
        int M, W;
        cin >> M >> W;
        race_count_at_W[W]++;
        for (int j = 0; j < M; j++) {
            string hname;
            cin >> hname;
            int hid = horse_to_id[hname];
            min_pos[hid] = max(min_pos[hid], W);
            count_in_W[W][hid]++;
        }
    }

    // Build bipartite graph
    vector<vector<int>> adj(N);
    for (int h = 0; h < N; h++) {
        for (int p = 1; p <= N; p++) {
            if (p < min_pos[h]) continue;
            if (race_count_at_W[p] > 0 && count_in_W[p][h] < race_count_at_W[p]) {
                continue;
            }
            adj[h].push_back(p - 1);
        }
    }

    vector<int> matchR(N, -1);
    vector<bool> vis(N, false);
    for (int i = 0; i < N; i++) {
        fill(vis.begin(), vis.end(), false);
        dfsKuhn(i, adj, matchR, vis);
    }

    for (int p = 0; p < N; p++) {
        cout << horse_names[matchR[p]] << (p == N - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}

