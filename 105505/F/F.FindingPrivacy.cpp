#include <iostream>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long k, n;
    if (!(cin >> k >> n)) return 0;

    long long k2 = 2 * k;
    long long k3 = 3 * k;

    if (n < k2 - 1 || k3 < n) {
        cout << "*\n";
    } else if (n == k2 - 1) {
        string res;
        res.reserve(n);
        for (long long i = 0; i < k - 1; i++) {
            res += "X-";
        }
        res += 'X';
        cout << res << "\n";
    } else {
        long long n2 = k3 - n;
        long long n3 = n - k2;
        string res;
        res.reserve(n);
        for (long long i = 0; i < n2; i++) {
            res += "X-";
        }
        for (long long i = 0; i < n3; i++) {
            res += "-X-";
        }
        cout << res << "\n";
    }

    return 0;
}
