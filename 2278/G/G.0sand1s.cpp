#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

static const int MOD = 998244353;

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<vector<int>> adj((size_t)n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[(size_t)u].push_back(v);
        adj[(size_t)v].push_back(u);
    }

    string x, y;
    cin >> x >> y;

    if (n == 1) {
        if (x[0] == y[0]) {
            cout << 0 << "\n";
        } else {
            cout << 1 << "\n";
        }
        return;
    }

    vector<int> depth((size_t)n, 0);
    vector<int> parent((size_t)n, -1);
    vector<bool> vis((size_t)n, false);
    queue<int> q;

    q.push(0);
    vis[0] = true;
    int max_d = 0;
    vector<int> order;
    order.reserve((size_t)n);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        max_d = max(max_d, depth[(size_t)u]);

        for (int v : adj[(size_t)u]) {
            if (!vis[(size_t)v]) {
                vis[(size_t)v] = true;
                depth[(size_t)v] = depth[(size_t)u] + 1;
                parent[(size_t)v] = u;
                q.push(v);
            }
        }
    }

    vector<int> carry((size_t)n, 0);
    vector<int> req_bit((size_t)(max_d + 1), -1);

    for (int u : order) {
        if (u == 0) {
            carry[(size_t)u] = 0;
            continue;
        }
        int p = parent[(size_t)u];
        int val = carry[(size_t)p] + ((y[(size_t)u] - '0') - (x[(size_t)u] - '0'));
        int b = (val % 2 + 2) % 2;
        int c = (val < 0) ? -1 : (val / 2);
        carry[(size_t)u] = c;

        int d = depth[(size_t)u];
        if (req_bit[(size_t)d] == -1) {
            req_bit[(size_t)d] = b;
        } else if (req_bit[(size_t)d] != b) {
            cout << -1 << "\n";
            return;
        }
    }

    long long C1_mod = 0;
    long long pow2 = 1;
    for (int d = 1; d <= max_d; d++) {
        C1_mod = (C1_mod + pow2 * req_bit[(size_t)d]) % MOD;
        pow2 = (pow2 * 2) % MOD;
    }

    long long K_mod = (2 * C1_mod + (y[0] - '0') - (x[0] - '0')) % MOD;
    if (K_mod < 0) K_mod += MOD;

    bool C1_is_zero = true;
    for (int d = 1; d <= max_d; d++) {
        if (req_bit[(size_t)d] != 0) {
            C1_is_zero = false;
            break;
        }
    }

    if (C1_is_zero && ((y[0] - '0') - (x[0] - '0') == -1)) {
        long long add = 1;
        for (int i = 0; i <= max_d; i++) {
            add = (add * 2) % MOD;
        }
        K_mod = (K_mod + add) % MOD;
    }

    cout << K_mod << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 0;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}

