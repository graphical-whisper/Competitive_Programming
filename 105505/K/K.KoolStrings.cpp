#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int k;
    string s;
    if (!(cin >> k >> s)) return 0;

    int n = s.size();

    if (k == 2) {
        // For k = 2, only two alternating strings are valid: "0101..." and "1010..."
        int diff0 = 0; // target starting with '0'
        int diff1 = 0; // target starting with '1'

        for (int i = 0; i < n; i++) {
            char expected0 = (i % 2 == 0 ? '0' : '1');
            if (s[i] != expected0) diff0++;

            char expected1 = (i % 2 == 0 ? '1' : '0');
            if (s[i] != expected1) diff1++;
        }

        if (diff0 <= diff1) {
            string res(n, ' ');
            for (int i = 0; i < n; i++) {
                res[i] = (i % 2 == 0 ? '0' : '1');
            }
            cout << diff0 << " " << res << "\n";
        } else {
            string res(n, ' ');
            for (int i = 0; i < n; i++) {
                res[i] = (i % 2 == 0 ? '1' : '0');
            }
            cout << diff1 << " " << res << "\n";
        }
        return 0;
    }

    // For k >= 3, each run of consecutive identical characters of length L >= k
    // can be processed independently with exactly floor(L / k) flips.
    int total_flips = 0;
    string res = s;

    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && s[j] == s[i]) {
            j++;
        }
        int len = j - i;
        if (len >= k) {
            int m = len / k;
            total_flips += m;
            bool is_multiple = (len % k == 0);

            for (int step = 1; step <= m; step++) {
                int flip_idx;
                if (step == m && is_multiple) {
                    // Shift the last flip one position left to avoid touching the next run
                    flip_idx = j - 2;
                } else {
                    flip_idx = i + step * k - 1;
                }
                res[flip_idx] = (res[flip_idx] == '0' ? '1' : '0');
            }
        }
        i = j;
    }

    cout << total_flips << " " << res << "\n";

    return 0;
}
