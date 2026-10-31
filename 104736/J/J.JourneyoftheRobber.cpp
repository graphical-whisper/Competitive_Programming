#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int n;
vector<vector<int>> adj;
vector<bool> removed;
vector<int> sz;

struct Cand {
    int dist;
    int id;

    bool operator<(const Cand& other) const {
        if (dist != other.dist) return dist < other.dist;
        return id < other.id;
    }
};

vector<Cand> best_ans;

void get_sz(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v != p && !removed[v]) {
            get_sz(v, u);
            sz[u] += sz[v];
        }
    }
}

int get_centroid(int u, int p, int total) {
    for (int v : adj[u]) {
        if (v != p && !removed[v] && sz[v] > total / 2) {
            return get_centroid(v, u, total);
        }
    }
    return u;
}

void get_nodes(int u, int p, int d, vector<pair<int, int>>& nodes) {
    nodes.push_back({u, d});
    for (int v : adj[u]) {
        if (v != p && !removed[v]) {
            get_nodes(v, u, d + 1, nodes);
        }
    }
}

void decompose(int u) {
    get_sz(u, 0);
    int c = get_centroid(u, 0, sz[u]);

    vector<pair<int, int>> nodes;
    get_nodes(c, 0, 0, nodes);

    sort(nodes.begin(), nodes.end());

    int m = (int)nodes.size();
    vector<Cand> suff(m + 1, {INF, INF});
    for (int i = m - 1; i >= 0; --i) {
        Cand cur = {nodes[i].second, nodes[i].first};
        suff[i] = min(cur, suff[i + 1]);
    }

    for (int i = 0; i < m; ++i) {
        int my_id = nodes[i].first;
        int d_to_c = nodes[i].second;

        auto it = upper_bound(nodes.begin(), nodes.end(), make_pair(my_id, INF));
        int idx = it - nodes.begin();
        if (idx < m) {
            Cand best_v = suff[idx];
            Cand cand = {d_to_c + best_v.dist, best_v.id};
            best_ans[my_id] = min(best_ans[my_id], cand);
        }
    }

    removed[c] = true;
    for (int v : adj[c]) {
        if (!removed[v]) {
            decompose(v);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    adj.assign(n + 1, vector<int>());
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    removed.assign(n + 1, false);
    sz.assign(n + 1, 0);
    best_ans.assign(n + 1, {INF, INF});

    decompose(1);

    for (int i = 1; i <= n; ++i) {
        int ans = (i == n ? n : best_ans[i].id);
        cout << ans << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}

