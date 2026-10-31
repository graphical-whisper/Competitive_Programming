#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

bool is_palindrome_base(long long n, long long b) {
    static int digits[70];
    int len = 0;
    while (n > 0) {
        digits[len++] = n % b;
        n /= b;
    }
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        if (digits[i] != digits[j]) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    if (!(cin >> n)) return 0;

    vector<long long> ans;

    // Group 1: palindromes with >= 3 digits.
    // Minimum 3-digit palindrome in base b is 101_b = b^2 + 1 <= n => b <= sqrt(n - 1)
    long long max_b1 = (long long)sqrt(n - 1);
    while ((max_b1 + 1) * (max_b1 + 1) <= n - 1) {
        max_b1++;
    }
    while (max_b1 * max_b1 > n - 1) {
        max_b1--;
    }

    for (long long b = 2; b <= max_b1; ++b) {
        if (n % b == 0) continue;
        if (is_palindrome_base(n, b)) {
            ans.push_back(b);
        }
    }

    // Group 2: palindromes with exactly 2 digits: n = d * b + d = d * (b + 1)
    // where 1 <= d <= b - 1 => d * (d + 2) <= n
    // b = n / d - 1
    // As d decreases, b increases.
    long long max_d = (long long)sqrt(n + 1);
    while (max_d * (max_d + 2) > n) {
        max_d--;
    }

    for (long long d = max_d; d >= 1; --d) {
        if (n % d == 0) {
            long long b = n / d - 1;
            if (b >= 2) {
                ans.push_back(b);
            }
        }
    }

    if (ans.empty()) {
        cout << "*\n";
    } else {
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << ans[i] << (i + 1 == ans.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}

