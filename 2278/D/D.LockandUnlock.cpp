#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void solve() {
    int n;
    if (!(cin >> n)) return;

    vector<long long> a((size_t)n);
    for (int i = 0; i < n; i++) {
        cin >> a[(size_t)i];
    }

    sort(a.begin(), a.end());

    long long min_ops = a[(size_t)(n - 1)] - a[0];
    long long median = a[(size_t)(n / 2)];

    long long min_cost = 0;
    for (int i = 0; i < n; i++) {
        min_cost += std::abs(a[(size_t)i] - median);
    }

    cout << min_cost << " " << min_ops << "\n";
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

