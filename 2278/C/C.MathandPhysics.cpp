#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<long long> a((size_t)n);
    long long total_sum = 0;
    bool has_ge_2 = false;
    for (int i = 0; i < n; i++) {
        cin >> a[(size_t)i];
        total_sum += a[(size_t)i];
        if (a[(size_t)i] >= 2) {
            has_ge_2 = true;
        }
    }

    if (!has_ge_2) {
        cout << -1 << "\n";
        return;
    }

    if (n == 2) {
        cout << (total_sum - 1) << "\n";
        return;
    }

    bool interior_has_ge_2 = false;
    for (int i = 1; i < n - 1; i++) {
        if (a[(size_t)i] >= 2) {
            interior_has_ge_2 = true;
            break;
        }
    }

    if (interior_has_ge_2) {
        cout << (total_sum - 1) << "\n";
    } else {
        cout << (a[0] + a[(size_t)(n - 1)] - 1) << "\n";
    }
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

