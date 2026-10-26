#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXX = 195;
const unsigned long long CAP = 4000000000000000007ULL;

struct Fenwick {
    int n;
    vector<int> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {
        for (int i = 1; i <= n; i++) {
            tree[i] += 1;
            int parent = i + (i & -i);
            if (parent <= n) tree[parent] += tree[i];
        }
    }
    void remove(int idx) {
        for (int i = idx; i <= n; i += i & -i) {
            tree[i] -= 1;
        }
    }
    int find_kth(int k) {
        k++; // Convert 0-indexed k to 1-indexed count
        int idx = 0;
        for (int step = 1 << 18; step > 0; step >>= 1) {
            if (idx + step <= n && tree[idx + step] < k) {
                idx += step;
                k -= tree[idx];
            }
        }
        return idx + 1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    unsigned long long K;
    if (!(cin >> n >> K)) return 0;

    // dp[i][x]: number of permutations of size i with x inversions
    vector<vector<unsigned long long>> dp(n + 1, vector<unsigned long long>(MAXX, 0));
    dp[0][0] = 1;

    for (int i = 1; i <= n; i++) {
        unsigned long long sum = 0;
        for (int x = 0; x < MAXX; x++) {
            sum += dp[i - 1][x];
            if (sum > CAP) sum = CAP;
            if (x >= i) {
                sum -= dp[i - 1][x - i];
            }
            dp[i][x] = sum;
        }
    }

    // Phase 1: Determine the exact number of inversions target_x
    int target_x = 0;
    while (target_x < MAXX && K > dp[n][target_x]) {
        K -= dp[n][target_x];
        target_x++;
    }

    // Phase 2: Construct the K-th lexicographic permutation with target_x inversions
    Fenwick ft(n);
    vector<int> ans(n);
    int cur_x = target_x;

    for (int pos = 0; pos < n; pos++) {
        int rem_n = n - pos;
        if (cur_x == 0) {
            int val = ft.find_kth(0);
            ft.remove(val);
            ans[pos] = val;
            continue;
        }

        int max_j = min(rem_n - 1, cur_x);
        for (int j = 0; j <= max_j; j++) {
            unsigned long long ways = dp[rem_n - 1][cur_x - j];
            if (K <= ways) {
                int val = ft.find_kth(j);
                ft.remove(val);
                ans[pos] = val;
                cur_x -= j;
                break;
            } else {
                K -= ways;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        cout << ans[i] << (i + 1 == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
