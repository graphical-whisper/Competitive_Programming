#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<long long> b((size_t)n);
    for (int i = 0; i < n; i++) {
        long long a;
        cin >> a;
        if (i % 2 == 0) {
            b[(size_t)i] = a;
        } else {
            b[(size_t)i] = -a;
        }
    }

    sort(b.begin(), b.end());

    int n_odd = n / 2;
    long long ans = 0;
    for (int i = 0; i < n_odd; i++) {
        ans -= b[(size_t)i];
    }
    for (int i = n_odd; i < n; i++) {
        ans += b[(size_t)i];
    }

    cout << ans << "\n";
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

