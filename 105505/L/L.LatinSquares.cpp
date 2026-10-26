#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, t;
    if (!(cin >> n >> t)) return 0;

    vector<int> p(n + 1), q(n + 1);
    for (int i = 1; i <= n; i++) {
        p[i] = i;
        q[i] = i;
    }

    for (int k = 0; k < t; k++) {
        char type;
        int i, j;
        cin >> type >> i >> j;
        if (type == 'R') {
            swap(p[i], p[j]);
        } else {
            swap(q[i], q[j]);
        }
    }

    // Decompose permutation p into disjoint cycles
    vector<bool> vis_p(n + 1, false);
    vector<vector<int>> row_cycles;
    for (int i = 1; i <= n; i++) {
        if (!vis_p[i]) {
            vector<int> cycle;
            int curr = i;
            while (!vis_p[curr]) {
                vis_p[curr] = true;
                cycle.push_back(curr);
                curr = p[curr];
            }
            row_cycles.push_back(cycle);
        }
    }

    // Decompose permutation q into disjoint cycles
    vector<bool> vis_q(n + 1, false);
    vector<vector<int>> col_cycles;
    for (int i = 1; i <= n; i++) {
        if (!vis_q[i]) {
            vector<int> cycle;
            int curr = i;
            while (!vis_q[curr]) {
                vis_q[curr] = true;
                cycle.push_back(curr);
                curr = q[curr];
            }
            col_cycles.push_back(cycle);
        }
    }

    int cycle_len = row_cycles[0].size();

    // All row cycles must have the same length
    for (const auto& c : row_cycles) {
        if ((int)c.size() != cycle_len) {
            cout << "*\n";
            return 0;
        }
    }

    // All column cycles must have the same length as the row cycles
    for (const auto& c : col_cycles) {
        if ((int)c.size() != cycle_len) {
            cout << "*\n";
            return 0;
        }
    }

    int L = cycle_len;
    int K = n / L;

    // Map each row to (cycle_index a, position in cycle x)
    vector<int> row_cycle_idx(n + 1), row_pos(n + 1);
    for (int a = 0; a < K; a++) {
        for (int x = 0; x < L; x++) {
            int r = row_cycles[a][x];
            row_cycle_idx[r] = a;
            row_pos[r] = x;
        }
    }

    // Map each col to (cycle_index b, position in cycle y)
    vector<int> col_cycle_idx(n + 1), col_pos(n + 1);
    for (int b = 0; b < K; b++) {
        for (int y = 0; y < L; y++) {
            int c = col_cycles[b][y];
            col_cycle_idx[c] = b;
            col_pos[c] = y;
        }
    }

    // Construct the invariant Latin square
    for (int i = 1; i <= n; i++) {
        int a = row_cycle_idx[i];
        int x = row_pos[i];
        for (int j = 1; j <= n; j++) {
            int b = col_cycle_idx[j];
            int y = col_pos[j];

            int group = (a + b) % K;
            int offset = (y - x + L) % L;
            int val = group * L + offset + 1;

            cout << val << (j == n ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
