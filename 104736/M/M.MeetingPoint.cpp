#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 4e18;

struct Edge {
    int to;
    long long weight;
};

vector<long long> dijkstra(int n, int start, int skip, const vector<vector<Edge>>& adj) {
    vector<long long> dist(n + 1, INF);
    if (start == skip) return dist;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : adj[u]) {
            if (edge.to == skip) continue;
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    return dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int p, g;
    cin >> p >> g;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    vector<long long> dist1 = dijkstra(n, p, -1, adj);
    vector<long long> dist2 = dijkstra(n, p, g, adj);

    long long target_dist = 2 * dist1[g];
    vector<int> ans;

    for (int v = 1; v <= n; ++v) {
        if (v == p || v == g) continue;
        if (dist1[v] == target_dist && dist2[v] > target_dist) {
            ans.push_back(v);
        }
    }

    if (ans.empty()) {
        cout << "*\n";
    } else {
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}

