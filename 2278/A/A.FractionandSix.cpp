#include <iostream>
#include <string>
#include <vector>

using namespace std;

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    string p, q;
    cin >> p >> q;

    if (n - m != 3) {
        cout << "NO\n";
        return;
    }

    vector<int> cnt_p(26, 0), cnt_q(26, 0);
    for (char c : p) {
        cnt_p[c - 'a']++;
    }
    for (char c : q) {
        cnt_q[c - 'a']++;
    }

    bool ok = true;
    for (int i = 0; i < 26; i++) {
        char c = (char)('a' + i);
        int expected = (c == 's' || c == 'i' || c == 'x') ? 1 : 0;
        if (cnt_p[i] - cnt_q[i] != expected) {
            ok = false;
            break;
        }
    }

    if (ok) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
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

