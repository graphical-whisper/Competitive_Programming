#include <iostream>
#include <string>
#include <vector>

using namespace std;

const long long MOD = 1000000007;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    long long n;
    if (!(cin >> n)) return 0;

    vector<long long> cnt(26, 0);
    long long inv_single = 0;

    for (char c : s) {
        int idx = c - 'a';
        for (int j = idx + 1; j < 26; ++j) {
            inv_single = (inv_single + cnt[j]) % MOD;
        }
        cnt[idx]++;
    }

    long long g = 0;
    for (int c1 = 0; c1 < 26; ++c1) {
        for (int c2 = c1 + 1; c2 < 26; ++c2) {
            long long pairs = (cnt[c1] % MOD) * (cnt[c2] % MOD) % MOD;
            g = (g + pairs) % MOD;
        }
    }

    long long n_mod = n % MOD;
    long long pairs_between;
    if (n % 2 == 0) {
        pairs_between = ((n / 2) % MOD) * ((n - 1) % MOD) % MOD;
    } else {
        pairs_between = (n % MOD) * (((n - 1) / 2) % MOD) % MOD;
    }

    long long total = (n_mod * inv_single) % MOD;
    total = (total + g * pairs_between) % MOD;

    cout << total << "\n";

    return 0;
}

