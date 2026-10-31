#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;

struct segment {
    bool asc;
    int b, e, h;
    segment(bool a = 0, int b = 0, int e = 0, int h = 0)
        : asc(a), b(b), e(e), h(h) {}

    bool isempty() const { return b == e; }
    bool inbetween(int p) const { return b <= p && p < e; }

    segment intersect(const segment& s) const {
        if (isempty() || s.isempty()) return segment();

        if (asc == s.asc) {
            if (h != s.h) return segment();
            int nb = max(b, s.b);
            int ne = min(e, s.e);
            ne = max(ne, nb);
            return segment(asc, nb, ne, h);
        } else {
            if (inbetween(s.h) && s.inbetween(h)) {
                return segment(asc, s.h, s.h + 1, h);
            } else {
                return segment();
            }
        }
    }

    vector<pii> traverse() const {
        if (isempty()) return {};
        vector<pii> ret;
        for (int p = b; p < e; p++) {
            int x2 = p - h;
            int y2 = p + h;
            if (!asc) x2 = -x2;
            if ((x2 % 2 == 0) && (y2 % 2 == 0)) {
                ret.push_back({x2 / 2, y2 / 2});
            }
        }
        return ret;
    }
};

vector<segment> interpret(int x, int y, int d) {
    int cx = x + y;
    int cy = y - x;

    if (d == 0) {
        return {
            segment(1, cx, cx + 1, cy)
        };
    }

    return {
        segment(1, cx - d, cx + d + 1, cy - d),
        segment(1, cx - d, cx + d + 1, cy + d),
        segment(0, cy - d + 1, cy + d, cx - d),
        segment(0, cy - d + 1, cy + d, cx + d)
    };
}

vector<segment> intersect(const vector<segment>& a, const vector<segment>& b) {
    vector<segment> result;
    for (const auto& sa : a) {
        for (const auto& sb : b) {
            segment c = sa.intersect(sb);
            if (!c.isempty()) result.push_back(c);
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    int x, y, d;
    cin >> x >> y >> d;
    vector<segment> cur = interpret(x, y, d);

    for (int i = 1; i < n; ++i) {
        cin >> x >> y >> d;
        cur = intersect(cur, interpret(x, y, d));
    }

    vector<pii> pts;
    for (const auto& s : cur) {
        auto t = s.traverse();
        pts.insert(pts.end(), t.begin(), t.end());
    }

    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    for (const auto& p : pts) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}

