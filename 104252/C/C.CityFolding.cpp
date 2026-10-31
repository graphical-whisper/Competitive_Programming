#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    unsigned long long p, h;
    if (!(cin >> n >> p >> h)) return 0;

    vector<unsigned long long> req_h(n + 1);
    req_h[n] = h;
    for (int k = n; k >= 1; k--) {
        unsigned long long s_k = 1ULL << k;
        unsigned long long half_s = s_k >> 1;
        if (req_h[k] <= half_s) {
            req_h[k - 1] = req_h[k];
        } else {
            req_h[k - 1] = s_k + 1ULL - req_h[k];
        }
    }

    string ans = "";
    unsigned long long cur_p = p;
    for (int k = 1; k <= n; k++) {
        unsigned long long half_len = 1ULL << (n - k);
        unsigned long long full_len = half_len << 1;
        unsigned long long s_k = 1ULL << k;
        unsigned long long half_s = s_k >> 1;

        bool needs_upper = (req_h[k] > half_s);

        if (cur_p <= half_len) {
            if (needs_upper) {
                ans += 'L';
                cur_p = half_len - cur_p + 1ULL;
            } else {
                ans += 'R';
            }
        } else {
            if (needs_upper) {
                ans += 'R';
                cur_p = full_len + 1ULL - cur_p;
            } else {
                ans += 'L';
                cur_p = cur_p - half_len;
            }
        }
    }

    cout << ans << "\n";
    return 0;
}

