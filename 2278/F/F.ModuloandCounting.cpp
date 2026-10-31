#include <iostream>
#include <vector>

using namespace std;

static const int MOD = 998244353;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // dp[v] represents the number of ways to reach value v at current step s.
    // v goes from 0 to m (representing core value v + 2).
    vector<int> dp((size_t)(m + 1), 0);
    dp[0] = 1;

    long long total_ans = 0;

    vector<int> pref((size_t)(m + 2), 0);
    vector<int> next_dp((size_t)(m + 1), 0);

    for (int s = 1; s <= n - 1; s++) {
        for (int v = 0; v <= m; v++) {
            int val = pref[(size_t)v] + dp[(size_t)v];
            if (val >= MOD) val -= MOD;
            pref[(size_t)(v + 1)] = val;
        }

        for (int v = 0; v <= m; v++) {
            int L = v / 2;
            int sum = pref[(size_t)(v + 1)] - pref[(size_t)L];
            if (sum < 0) sum += MOD;
            next_dp[(size_t)v] = sum;
        }

        dp = next_dp;
        total_ans += dp[(size_t)m];
        if (total_ans >= MOD) total_ans -= MOD;
    }

    cout << total_ans << "\n";
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

