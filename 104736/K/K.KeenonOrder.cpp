#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }

    // 1. Greedy approach
    {
        int cur = 0;
        vector<bool> used(k + 1, false);
        vector<int> perm;
        bool found = false;

        for (int step = 0; step < k; ++step) {
            int best_x = -1;
            int best_pos = -1;
            for (int x = 1; x <= k; ++x) {
                if (used[x]) continue;
                int pos = -1;
                for (int i = cur; i < n; ++i) {
                    if (v[i] == x) {
                        pos = i;
                        break;
                    }
                }
                if (pos == -1) {
                    best_x = x;
                    best_pos = n + 1;
                    break;
                }
                if (pos > best_pos) {
                    best_pos = pos;
                    best_x = x;
                }
            }
            perm.push_back(best_x);
            used[best_x] = true;
            if (best_pos > n) {
                for (int x = 1; x <= k; ++x) {
                    if (!used[x]) perm.push_back(x);
                }
                found = true;
                break;
            }
            cur = best_pos + 1;
        }

        if (found) {
            // Verify perm is indeed not a subsequence
            int c = 0;
            bool is_subseq = true;
            for (int x : perm) {
                int pos = -1;
                for (int i = c; i < n; ++i) {
                    if (v[i] == x) {
                        pos = i;
                        break;
                    }
                }
                if (pos == -1) {
                    is_subseq = false;
                    break;
                }
                c = pos + 1;
            }
            if (!is_subseq) {
                for (int i = 0; i < k; ++i) {
                    cout << perm[i] << (i + 1 == k ? "" : " ");
                }
                cout << "\n";
                return 0;
            }
        }
    }

    // 2. Precompute next occurrence table
    // next_pos[x][i]: first position >= i where v[pos] == x (0-indexed)
    vector<vector<int>> next_pos(k + 1, vector<int>(n + 2, n + 1));
    for (int x = 1; x <= k; ++x) {
        int last = n + 1;
        for (int i = n; i >= 0; --i) {
            if (i < n && v[i] == x) {
                last = i;
            }
            next_pos[x][i] = last;
        }
    }

    int total_masks = 1 << k;
    vector<uint16_t> dp(total_masks, 0);
    vector<uint8_t> parent(total_masks, 0);

    int failing_mask = -1;

    for (int mask = 1; mask < total_masks; ++mask) {
        int best = 0;
        int best_x = 0;
        for (int i = 0; i < k; ++i) {
            if ((mask >> i) & 1) {
                int prev = dp[mask ^ (1 << i)];
                int cand;
                if (prev > n) {
                    cand = n + 1;
                } else {
                    cand = next_pos[i + 1][prev] + 1;
                }
                if (cand > best) {
                    best = cand;
                    best_x = i + 1;
                }
            }
        }
        dp[mask] = (uint16_t)best;
        parent[mask] = (uint8_t)best_x;
        if (best > n) {
            failing_mask = mask;
            break;
        }
    }

    if (failing_mask == -1) {
        cout << "*\n";
        return 0;
    }

    // Reconstruct failing permutation
    vector<int> perm;
    int curr = failing_mask;
    while (curr > 0) {
        int x = parent[curr];
        perm.push_back(x);
        curr ^= (1 << (x - 1));
    }
    reverse(perm.begin(), perm.end());

    vector<bool> in_perm(k + 1, false);
    for (int x : perm) in_perm[x] = true;
    for (int x = 1; x <= k; ++x) {
        if (!in_perm[x]) perm.push_back(x);
    }

    for (int i = 0; i < k; ++i) {
        cout << perm[i] << (i + 1 == k ? "" : " ");
    }
    cout << "\n";

    return 0;
}

