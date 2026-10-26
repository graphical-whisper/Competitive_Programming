#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const int MOD = 1000000007;
const int K_BATCH = 150;

struct Call {
    int to;
    int count;
};

int n, m, e;
vector<long long> B;
vector<long long> B_start;
vector<long long> T_val;
vector<vector<Call>> adj;
vector<int> in_deg;
vector<int> topo;

// C_mat[x][y]: number of paths from x to y in call multigraph modulo MOD
vector<vector<int>> C_mat;

void compute_topo_and_C() {
    // Kahn's algorithm for topological sorting
    vector<int> deg = in_deg;
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for (auto& edge : adj[u]) {
            deg[edge.to] -= edge.count;
            if (deg[edge.to] == 0) {
                q.push(edge.to);
            }
        }
    }

    // Allocate C_mat: (n + 1) x (n + 1)
    C_mat.assign(n + 1, vector<int>(n + 1, 0));

    // Compute C_mat in reverse topological order
    for (int i = n - 1; i >= 0; i--) {
        int u = topo[i];
        C_mat[u][u] = 1;
        for (auto& edge : adj[u]) {
            int v = edge.to;
            long long cnt = edge.count;
            for (int y = 1; y <= n; y++) {
                if (C_mat[v][y]) {
                    C_mat[u][y] = (C_mat[u][y] + cnt * C_mat[v][y]) % MOD;
                }
            }
        }
    }
}

void recompute_T() {
    for (int i = n - 1; i >= 0; i--) {
        int u = topo[i];
        long long cur = B[u] % MOD;
        for (auto& edge : adj[u]) {
            cur = (cur + (long long)edge.count * T_val[edge.to]) % MOD;
        }
        T_val[u] = cur;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    B.resize(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> B[i];
    }

    cin >> m;
    vector<vector<int>> raw_calls(n + 1);
    in_deg.assign(n + 1, 0);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        raw_calls[u].push_back(v);
        in_deg[v]++;
    }

    // Compress multiple calls between the same pair (u, v)
    adj.resize(n + 1);
    for (int u = 1; u <= n; u++) {
        if (raw_calls[u].empty()) continue;
        sort(raw_calls[u].begin(), raw_calls[u].end());
        int last = raw_calls[u][0];
        int count = 1;
        for (size_t i = 1; i < raw_calls[u].size(); i++) {
            if (raw_calls[u][i] == last) {
                count++;
            } else {
                adj[u].push_back({last, count});
                last = raw_calls[u][i];
                count = 1;
            }
        }
        adj[u].push_back({last, count});
    }

    compute_topo_and_C();

    B_start = B;
    T_val.assign(n + 1, 0);
    recompute_T();

    cin >> e;

    vector<int> modified_funcs;
    vector<bool> is_modified(n + 1, false);
    int num_updates = 0;
    long long query_k = 0;
    long long summary = 0;

    for (int ev = 0; ev < e; ev++) {
        char type;
        cin >> type;
        if (type == 'U') {
            int idx;
            long long val;
            cin >> idx >> val;
            B[idx] = val;
            if (!is_modified[idx]) {
                is_modified[idx] = true;
                modified_funcs.push_back(idx);
            }
            num_updates++;
            if (num_updates >= K_BATCH) {
                recompute_T();
                for (int u : modified_funcs) {
                    B_start[u] = B[u];
                    is_modified[u] = false;
                }
                modified_funcs.clear();
                num_updates = 0;
            }
        } else {
            int j;
            cin >> j;
            query_k++;

            long long cur_T = T_val[j];
            for (int u : modified_funcs) {
                long long delta = (B[u] - B_start[u]) % MOD;
                if (delta < 0) delta += MOD;
                if (delta != 0 && C_mat[j][u] != 0) {
                    cur_T = (cur_T + (long long)C_mat[j][u] * delta) % MOD;
                }
            }

            summary = (summary + query_k * cur_T) % MOD;
        }
    }

    cout << summary << "\n";

    return 0;
}
