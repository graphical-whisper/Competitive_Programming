#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Maximum Bipartite Matching using Kuhn's algorithm
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

int maxBipartiteMatching(int nL, int nR, const vector<vector<int>>& adj) {
    vector<int> matchR(nR, -1);
    vector<bool> vis(nR, false);
    int count = 0;
    for (int i = 0; i < nL; i++) {
        fill(vis.begin(), vis.end(), false);
        if (dfsKuhn(i, adj, matchR, vis)) {
            count++;
        }
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n1;
    if (!(cin >> n1)) return 0;
    vector<vector<int>> adj1(n1 + 1);
    for (int i = 0; i < n1 - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj1[u].push_back(v);
        adj1[v].push_back(u);
    }

    int n2;
    if (!(cin >> n2)) return 0;
    vector<vector<int>> adj2(n2 + 1);
    for (int i = 0; i < n2 - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj2[u].push_back(v);
        adj2[v].push_back(u);
    }

    if (n2 > n1) {
        cout << "N\n";
        return 0;
    }
    if (n2 == 1) {
        cout << "Y\n";
        return 0;
    }

    // Root T2 at vertex 1
    vector<vector<int>> children2(n2 + 1);
    vector<int> order2;
    order2.reserve(n2);
    vector<bool> vis2(n2 + 1, false);
    queue<int> q;

    q.push(1);
    vis2[1] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order2.push_back(u);
        for (int v : adj2[u]) {
            if (!vis2[v]) {
                vis2[v] = true;
                children2[u].push_back(v);
                q.push(v);
            }
        }
    }
    reverse(order2.begin(), order2.end());

    // can_embed[u][p][v]:
    // u in 1..n1, p in 0..n1, v in 1..n2
    static bool can_embed[105][105][105];

    for (int v : order2) {
        const auto& chV = children2[v];
        int num_ch = chV.size();

        for (int u = 1; u <= n1; u++) {
            // p can be 0 or any neighbor of u
            vector<int> possible_p;
            possible_p.reserve(adj1[u].size() + 1);
            possible_p.push_back(0);
            for (int nb : adj1[u]) possible_p.push_back(nb);

            for (int p : possible_p) {
                vector<int> candW;
                candW.reserve(adj1[u].size());
                for (int w : adj1[u]) {
                    if (w != p) candW.push_back(w);
                }

                if (num_ch > (int)candW.size()) {
                    can_embed[u][p][v] = false;
                    continue;
                }
                if (num_ch == 0) {
                    can_embed[u][p][v] = true;
                    continue;
                }

                vector<vector<int>> matchAdj(num_ch);
                for (int i = 0; i < num_ch; i++) {
                    int c = chV[i];
                    for (int j = 0; j < (int)candW.size(); j++) {
                        int w = candW[j];
                        if (can_embed[w][u][c]) {
                            matchAdj[i].push_back(j);
                        }
                    }
                }

                int matched = maxBipartiteMatching(num_ch, candW.size(), matchAdj);
                can_embed[u][p][v] = (matched == num_ch);
            }
        }
    }

    bool found = false;
    for (int r1 = 1; r1 <= n1; r1++) {
        if (can_embed[r1][0][1]) {
            found = true;
            break;
        }
    }

    cout << (found ? "Y\n" : "N\n");
    return 0;
}

