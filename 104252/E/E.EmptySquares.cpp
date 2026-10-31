#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solve(int n, int k, int e) {
    int a = e;
    int b = n - k - e;

    if (a == b && b == k) {
        if (k >= 5) return 0;
        if (k == 4) return 2;
        if (k == 3) return 3;
        if (k == 2) return 3;
        if (k == 1) return 2;
        if (k == 0) return 0;
    }

    if (b == k) {
        swap(a, b);
    }

    if (a <= 4 && b <= 4) {
        vector<int> tiles;
        for (int x = 1; x <= 4; x++) {
            if (x != k && x <= n) tiles.push_back(x);
        }
        int min_empty = a + b;
        int m = tiles.size();
        int total_ways = 1;
        for (int i = 0; i < m; i++) total_ways *= 3;

        for (int mask = 0; mask < total_ways; mask++) {
            int cur = mask;
            int sum_a = 0, sum_b = 0;
            for (int t : tiles) {
                int choice = cur % 3;
                cur /= 3;
                if (choice == 1) sum_a += t;
                else if (choice == 2) sum_b += t;
            }
            if (sum_a <= a && sum_b <= b) {
                min_empty = min(min_empty, (a - sum_a) + (b - sum_b));
            }
        }
        return min_empty;
    }

    if (b < 5) {
        swap(a, b);
    }

    if (a >= 5) return 0;

    vector<int> tiles;
    for (int x = 1; x <= 4; x++) {
        if (x != k && x != b && x <= n) tiles.push_back(x);
    }
    int best = 0;
    int m = tiles.size();
    for (int mask = 0; mask < (1 << m); mask++) {
        int s = 0;
        for (int i = 0; i < m; i++) {
            if ((mask >> i) & 1) s += tiles[i];
        }
        if (s <= a && s > best) best = s;
    }
    return a - best;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k, e;
    if (cin >> n >> k >> e) {
        cout << solve(n, k, e) << "\n";
    }

    return 0;
}

