#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

long long solve_segment(const string& w) {
    int m = (int)w.size();
    if (m == 0) return 0;

    vector<int> pref(m + 1, 0);
    for (int k = 0; k < m; ++k) {
        int delta = 0;
        if (w[k] == '(') delta = 1;
        else if (w[k] == ')') delta = -1;
        pref[k + 1] = pref[k] + delta;
    }

    int offset = m;
    vector<vector<int>> positions(2 * m + 1);

    for (int k = 0; k < m; ++k) {
        if (w[k] != '+') {
            positions[pref[k] + offset].push_back(k);
        }
    }

    vector<int> last_less(m + 1, -1);
    vector<int> st;
    for (int p = 0; p <= m; ++p) {
        while (!st.empty() && pref[st.back()] >= pref[p]) {
            st.pop_back();
        }
        if (!st.empty()) {
            last_less[p] = st.back();
        }
        st.push_back(p);
    }

    long long count = 0;
    for (int j = 0; j < m; ++j) {
        if (w[j] == '+') continue;
        int p = j + 1;
        int B = pref[p] + offset;
        const auto& vec = positions[B];
        auto it1 = upper_bound(vec.begin(), vec.end(), last_less[p]);
        auto it2 = upper_bound(vec.begin(), vec.end(), j);
        count += (it2 - it1);
    }

    return count;
}

bool is_forbidden(char c1, char c2) {
    if (c1 == '(' && c2 == ')') return true;
    if (c1 == '(' && c2 == '+') return true;
    if (c1 == '+' && c2 == ')') return true;
    if (c1 == '+' && c2 == '+') return true;
    if (c1 == ')' && c2 == '(') return true;
    if (c1 == ')' && c2 == 'a') return true;
    if (c1 == 'a' && c2 == '(') return true;
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;

    for (char& c : s) {
        if (c >= 'a' && c <= 'z') c = 'a';
        else if (c == '+' || c == '-' || c == '*' || c == '/') c = '+';
    }

    long long total = 0;
    int n = (int)s.size();
    int start = 0;
    for (int i = 0; i < n; ++i) {
        if (i + 1 == n || is_forbidden(s[i], s[i + 1])) {
            string seg = s.substr(start, i - start + 1);
            total += solve_segment(seg);
            start = i + 1;
        }
    }

    cout << total << "\n";

    return 0;
}

