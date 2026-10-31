#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Line {
    long long m, c;
    inline long long eval(long long x) const {
        return m * x + c;
    }
};

struct Node {
    int lc, rc;
    Line line;
    bool has_line;
};

const int MAX_NODES = 4500000;
Node* tree;
int node_cnt = 0;
int roots[100005];

const long long MIN_X = -1000000000LL;
const long long MAX_X = 1000000000LL;

int insert(int prev, long long l, long long r, Line new_line) {
    int cur = ++node_cnt;
    tree[cur] = (prev ? tree[prev] : Node{0, 0, {0, 0}, false});

    if (!tree[cur].has_line) {
        tree[cur].line = new_line;
        tree[cur].has_line = true;
        return cur;
    }

    long long mid = l + (r - l) / 2;
    bool better_mid = new_line.eval(mid) > tree[cur].line.eval(mid);

    if (better_mid) {
        swap(new_line, tree[cur].line);
    }

    if (l == r) return cur;

    if (new_line.eval(l) > tree[cur].line.eval(l)) {
        tree[cur].lc = insert(tree[cur].lc, l, mid, new_line);
    } else if (new_line.eval(r) > tree[cur].line.eval(r)) {
        tree[cur].rc = insert(tree[cur].rc, mid + 1, r, new_line);
    }

    return cur;
}

inline bool is_strictly_greater(int u, long long l, long long r, long long x, long long y) {
    while (u) {
        if (tree[u].has_line && tree[u].line.eval(x) > y) {
            return true;
        }
        if (l == r) break;
        long long mid = l + (r - l) / 2;
        if (x <= mid) {
            u = tree[u].lc;
            r = mid;
        } else {
            u = tree[u].rc;
            l = mid + 1;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    tree = new Node[MAX_NODES]();

    int T;
    if (!(cin >> T)) return 0;

    vector<pair<long long, long long>> tokens(T + 1);
    for (int i = 1; i <= T; i++) {
        cin >> tokens[i].first >> tokens[i].second;
    }

    int P;
    cin >> P;
    for (int i = 1; i <= P; i++) {
        long long a, b;
        cin >> a >> b;
        roots[i] = insert(roots[i - 1], MIN_X, MAX_X, {a, b});
    }

    vector<vector<int>> ans(P + 1);

    for (int j = 1; j <= T; j++) {
        long long x = tokens[j].first;
        long long y = tokens[j].second;

        // If the maximum over all P players is not > y, token is never taken
        if (!is_strictly_greater(roots[P], MIN_X, MAX_X, x, y)) {
            continue;
        }

        // Binary search for the first player index where max line value > y
        int low = 1, high = P, best = P;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_strictly_greater(roots[mid], MIN_X, MAX_X, x, y)) {
                best = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        ans[best].push_back(j);
    }

    for (int i = 1; i <= P; i++) {
        cout << ans[i].size();
        for (int tok : ans[i]) {
            cout << " " << tok;
        }
        cout << "\n";
    }

    return 0;
}
