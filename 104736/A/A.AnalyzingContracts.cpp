#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Supplier {
    long long s, p;
};

struct Client {
    long long e, r;
    long long eval(const Supplier& sup) const {
        if (sup.s > e || sup.p >= r) return 0;
        return (r - sup.p) * (e - sup.s + 1);
    }
};

vector<Supplier> suppliers;

struct Node {
    Client client = {0, 0};
};

vector<Node> tree;

void insert_line(int node, int l, int r, Client g) {
    int mid = (l + r) / 2;
    bool b_mid = g.eval(suppliers[mid]) > tree[node].client.eval(suppliers[mid]);
    bool b_l = g.eval(suppliers[l]) > tree[node].client.eval(suppliers[l]);
    if (b_mid) {
        swap(tree[node].client, g);
    }
    if (l == r) return;
    if (b_l != b_mid) {
        insert_line(2 * node, l, mid, g);
    } else {
        insert_line(2 * node + 1, mid + 1, r, g);
    }
}

void insert_segment(int node, int l, int r, int ql, int qr, Client g) {
    if (ql <= l && r <= qr) {
        insert_line(node, l, r, g);
        return;
    }
    int mid = (l + r) / 2;
    if (ql <= mid) insert_segment(2 * node, l, mid, ql, qr, g);
    if (qr > mid) insert_segment(2 * node + 1, mid + 1, r, ql, qr, g);
}

long long query(int node, int l, int r, int idx) {
    long long res = tree[node].client.eval(suppliers[idx]);
    if (l == r) return res;
    int mid = (l + r) / 2;
    if (idx <= mid) {
        res = max(res, query(2 * node, l, mid, idx));
    } else {
        res = max(res, query(2 * node + 1, mid + 1, r, idx));
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    suppliers.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> suppliers[i].s >> suppliers[i].p;
    }

    tree.resize(4 * (n + 1));

    int q;
    if (!(cin >> q)) return 0;

    while (q--) {
        char type;
        cin >> type;
        if (type == 'c') {
            long long e, r;
            cin >> e >> r;

            int low = 1, high = n, ql = n + 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                if (suppliers[mid].p < r) {
                    ql = mid;
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }

            low = 1, high = n;
            int qr = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                if (suppliers[mid].s <= e) {
                    qr = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }

            if (ql <= qr) {
                insert_segment(1, 1, n, ql, qr, {e, r});
            }
        } else {
            int idx;
            cin >> idx;
            cout << query(1, 1, n, idx) << "\n";
        }
    }

    return 0;
}

