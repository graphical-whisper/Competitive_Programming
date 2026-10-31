#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

int bucket_id_counter = 0;

struct Bucket {
    long long sz;
    long long sum;
    long long f;
    int id;

    bool operator<(const Bucket& other) const {
        if (sum * other.sz != other.sum * sz) {
            return sum * other.sz < other.sum * sz;
        }
        return id < other.id;
    }
};

Bucket merge_buckets(const Bucket& a, const Bucket& b) {
    Bucket c;
    c.sz = a.sz + b.sz;
    c.sum = a.sum + b.sum;
    c.f = a.f + b.f + a.sz * b.sum;
    c.id = ++bucket_id_counter;
    return c;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, r;
    if (!(cin >> n >> r)) return 0;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> parent(n + 1, 0);
    vector<int> order;
    order.reserve(n);
    vector<bool> vis(n + 1, false);

    queue<int> q;
    q.push(r);
    vis[r] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<set<Bucket>*> node_sets(n + 1, nullptr);
    for (int i = 1; i <= n; ++i) {
        node_sets[i] = new set<Bucket>();
    }

    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        set<Bucket>* s = node_sets[u];

        Bucket b_u;
        b_u.sz = 1;
        b_u.sum = u;
        b_u.f = u;
        b_u.id = ++bucket_id_counter;

        while (!s->empty()) {
            auto it = s->begin();
            const Bucket& min_b = *it;
            if (b_u.sum * min_b.sz > min_b.sum * b_u.sz) {
                Bucket merged = merge_buckets(min_b, b_u);
                s->erase(it);
                b_u = merged;
            } else {
                break;
            }
        }
        s->insert(b_u);

        if (u != r) {
            int p = parent[u];
            if (node_sets[p]->size() < s->size()) {
                swap(node_sets[p], s);
            }
            for (const auto& b : *s) {
                node_sets[p]->insert(b);
            }
            delete s;
            node_sets[u] = nullptr;
        }
    }

    set<Bucket>* final_s = node_sets[r];
    vector<Bucket> buckets(final_s->begin(), final_s->end());
    reverse(buckets.begin(), buckets.end());

    long long total_f = 0;
    long long prev_sz = 0;
    for (const auto& b : buckets) {
        total_f += b.f + prev_sz * b.sum;
        prev_sz += b.sz;
    }

    long long sum_all = (long long)n * (n + 1) / 2;
    long long ans = (long long)(n + 1) * sum_all - total_f;

    cout << ans << "\n";

    delete final_s;
    return 0;
}

