#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> x(n + 1), y(n + 1);
    vector<vector<int>> inv(n + 1);

    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
        inv[x[i]].push_back(i);
        inv[y[i]].push_back(i);
    }

    string ans = "";
    ans.reserve(n);

    // Reusable structures for BFS
    vector<int> q(n + 1);
    vector<int> vis_k(n + 1, 0);
    vector<int> vis_x(n + 1, 0);
    vector<int> vis_y(n + 1, 0);

    auto bfs = [&](int start, int forbidden, vector<int>& vis, int mark) {
        int head = 0, tail = 0;
        vis[start] = mark;
        q[tail++] = start;
        while (head < tail) {
            int u = q[head++];
            for (int v : inv[u]) {
                if (v == forbidden) continue;
                if (vis[v] != mark) {
                    vis[v] = mark;
                    q[tail++] = v;
                }
            }
        }
    };

    for (int k = 1; k <= n; k++) {
        bfs(k, k, vis_k, k);
        bfs(x[k], k, vis_x, k);
        bfs(y[k], k, vis_y, k);

        bool can_lose = false;
        for (int s = 1; s <= n; s++) {
            if (vis_k[s] == k && vis_x[s] == k && vis_y[s] == k) {
                can_lose = true;
                break;
            }
        }

        ans.push_back(can_lose ? 'Y' : 'N');
    }

    cout << ans << "\n";

    return 0;
}

