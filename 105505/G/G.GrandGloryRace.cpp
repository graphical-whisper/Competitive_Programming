#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

struct Candidate {
    long long dist;
    int leaf;
    int branch;

    bool operator<(const Candidate& other) const {
        if (dist != other.dist) return dist < other.dist;
        return leaf < other.leaf;
    }
};

int n, q;
vector<vector<Edge>> adj;
vector<int> deg;

// For DFS 1
vector<Candidate> down1, down2;

// To store queries: queries[T] = list of {S, qid}
struct Query {
    int s;
    int qid;
};
vector<vector<Query>> queries;
vector<int> ans;

// Winner at each node and claimed counts
vector<int> winner;
vector<int> claimed_by;

void add_cand(Candidate& top1, Candidate& top2, const Candidate& c) {
    if (c.leaf == -1) return;
    if (c.branch == top1.branch) {
        if (c < top1) top1 = c;
    } else if (c.branch == top2.branch) {
        if (c < top1) {
            top2 = top1;
            top1 = c;
        } else if (c < top2) {
            top2 = c;
        }
    } else {
        if (c < top1) {
            top2 = top1;
            top1 = c;
        } else if (c < top2) {
            top2 = c;
        }
    }
}

void dfs1(int u, int p) {
    if (deg[u] == 1) {
        down1[u] = {0, u, u};
    } else {
        down1[u] = {INF, n + 1, -1};
    }
    down2[u] = {INF, n + 1, -1};

    for (auto& edge : adj[u]) {
        int v = edge.to;
        long long w = edge.weight;
        if (v == p) continue;
        dfs1(v, u);
        if (down1[v].dist < INF) {
            Candidate c = {down1[v].dist + w, down1[v].leaf, v};
            add_cand(down1[u], down2[u], c);
        }
    }
}

// Full top candidates at each node
vector<Candidate> full1, full2;

void dfs2(int u, int p) {
    // Answer queries for Crown Village T = u
    for (auto& qry : queries[u]) {
        ans[qry.qid] = claimed_by[qry.s];
    }

    for (auto& edge : adj[u]) {
        int v = edge.to;
        long long w = edge.weight;
        if (v == p) continue;

        // Candidate that u passes to v
        Candidate cand_u;
        if (full1[u].branch != v) {
            cand_u = full1[u];
        } else {
            cand_u = full2[u];
        }

        Candidate from_u = {cand_u.dist + w, cand_u.leaf, u};

        // Old states
        int old_win_u = winner[u];
        int old_win_v = winner[v];
        Candidate old_full1_v = full1[v];
        Candidate old_full2_v = full2[v];

        // Update u's winner: in tree rooted at v, u's subtree excludes v
        int new_win_u = cand_u.leaf;

        // Update v's full candidates: incorporate from_u
        Candidate new_full1_v = full1[v];
        Candidate new_full2_v = full2[v];
        add_cand(new_full1_v, new_full2_v, from_u);
        int new_win_v = new_full1_v.leaf;

        // Apply changes to winner and claimed_by
        winner[u] = new_win_u;
        winner[v] = new_win_v;
        claimed_by[old_win_u]--;
        claimed_by[new_win_u]++;
        claimed_by[old_win_v]--;
        claimed_by[new_win_v]++;

        full1[v] = new_full1_v;
        full2[v] = new_full2_v;

        // Recurse to v
        dfs2(v, u);

        // Revert changes
        full1[v] = old_full1_v;
        full2[v] = old_full2_v;
        claimed_by[new_win_v]--;
        claimed_by[old_win_v]++;
        claimed_by[new_win_u]--;
        claimed_by[old_win_u]++;
        winner[u] = old_win_u;
        winner[v] = old_win_v;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n)) return 0;

    adj.resize(n + 1);
    deg.assign(n + 1, 0);

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        deg[u]++;
        deg[v]++;
    }

    cin >> q;
    queries.resize(n + 1);
    ans.resize(q);
    for (int i = 0; i < q; i++) {
        int s, t;
        cin >> s >> t;
        queries[t].push_back({s, i});
    }

    down1.resize(n + 1);
    down2.resize(n + 1);
    dfs1(1, 0);

    full1 = down1;
    full2 = down2;

    winner.resize(n + 1);
    claimed_by.assign(n + 1, 0);

    // Initial state with root at 1:
    // Every node u is claimed by down1[u].leaf
    for (int i = 1; i <= n; i++) {
        winner[i] = down1[i].leaf;
        claimed_by[winner[i]]++;
    }

    dfs2(1, 0);

    for (int i = 0; i < q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
