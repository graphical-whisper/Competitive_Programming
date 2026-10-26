#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int to;
    int id;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    vector<int> depth(n + 1, 0);
    vector<int> parent_node(n + 1, 0);
    vector<int> parent_edge(n + 1, 0);
    vector<bool> visited(n + 1, false);
    vector<int> it(n + 1, 0);

    int max_cost = -1;
    int best_u = -1, best_v = -1, best_eid = -1;

    vector<int> stk;
    stk.reserve(n + 1);

    visited[1] = true;
    depth[1] = 0;
    stk.push_back(1);

    while (!stk.empty()) {
        int u = stk.back();
        if (it[u] < (int)adj[u].size()) {
            auto edge = adj[u][it[u]++];
            int v = edge.to;
            int eid = edge.id;

            if (v == parent_node[u]) {
                continue;
            }

            if (!visited[v]) {
                visited[v] = true;
                depth[v] = depth[u] + 1;
                parent_node[v] = u;
                parent_edge[v] = eid;
                stk.push_back(v);
            } else if (depth[v] < depth[u]) {
                // Backward edge from u to ancestor v
                if (depth[v] > max_cost) {
                    max_cost = depth[v];
                    best_u = u;
                    best_v = v;
                    best_eid = eid;
                }
            }
        } else {
            stk.pop_back();
        }
    }

    if (max_cost == -1) {
        cout << "*\n";
        return 0;
    }

    vector<int> cycle;
    cycle.push_back(best_eid);
    int curr = best_u;
    while (curr != best_v) {
        cycle.push_back(parent_edge[curr]);
        curr = parent_node[curr];
    }

    cout << cycle.size() << "\n";
    for (int i = 0; i < (int)cycle.size(); i++) {
        cout << cycle[i] << (i + 1 == (int)cycle.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
