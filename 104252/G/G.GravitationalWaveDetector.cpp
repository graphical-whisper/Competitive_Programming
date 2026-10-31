#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef __int128_t int128;

struct Point {
    long long x, y;

    Point() : x(0), y(0) {}
    Point(long long _x, long long _y) : x(_x), y(_y) {}

    Point operator+(const Point& o) const {
        return Point(x + o.x, y + o.y);
    }

    Point operator-(const Point& o) const {
        return Point(x - o.x, y - o.y);
    }
};

inline int128 cross(const Point& a, const Point& b) {
    return (int128)a.x * b.y - (int128)a.y * b.x;
}

inline int128 dot(const Point& a, const Point& b) {
    return (int128)a.x * b.x + (int128)a.y * b.y;
}

void reorder_polygon(vector<Point>& p) {
    size_t pos = 0;
    for (size_t i = 1; i < p.size(); i++) {
        if (p[i].y < p[pos].y || (p[i].y == p[pos].y && p[i].x < p[pos].x)) {
            pos = i;
        }
    }
    rotate(p.begin(), p.begin() + pos, p.end());
}

vector<Point> clean_collinear(const vector<Point>& poly) {
    int n = poly.size();
    if (n <= 3) return poly;
    vector<Point> res;
    res.reserve(n);
    for (int i = 0; i < n; i++) {
        Point prev = poly[(i - 1 + n) % n];
        Point cur = poly[i];
        Point next = poly[(i + 1) % n];
        Point e1 = cur - prev;
        Point e2 = next - cur;
        if (cross(e1, e2) == 0 && dot(e1, e2) > 0) {
            continue;
        }
        res.push_back(cur);
    }
    return res;
}

vector<Point> minkowski(vector<Point> P, vector<Point> Q) {
    reorder_polygon(P);
    reorder_polygon(Q);
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);

    vector<Point> result;
    result.reserve(P.size() + Q.size());
    size_t i = 0, j = 0;
    while (i < P.size() - 2 || j < Q.size() - 2) {
        result.push_back(P[i] + Q[j]);
        int128 c = 0;
        bool canP = (i < P.size() - 2);
        bool canQ = (j < Q.size() - 2);
        if (canP && canQ) {
            Point eP = P[i + 1] - P[i];
            Point eQ = Q[j + 1] - Q[j];
            c = cross(eP, eQ);
            if (c >= 0) i++;
            if (c <= 0) j++;
        } else if (canP) {
            i++;
        } else {
            j++;
        }
    }
    return clean_collinear(result);
}

bool point_in_convex_polygon(const vector<Point>& poly, const Point& q) {
    int n = poly.size();
    if (n < 3) return false;

    Point p0 = poly[0];
    Point vq = q - p0;
    Point v1 = poly[1] - p0;
    Point vn = poly[n - 1] - p0;

    int128 c1 = cross(v1, vq);
    int128 cn = cross(vn, vq);

    if (c1 < 0 || cn > 0) return false;

    if (c1 == 0) {
        return dot(v1, vq) >= 0 && dot(vq, vq) <= dot(v1, v1);
    }
    if (cn == 0) {
        return dot(vn, vq) >= 0 && dot(vq, vq) <= dot(vn, vn);
    }

    int low = 1, high = n - 2;
    int idx = 1;
    while (low <= high) {
        int mid = (low + high) >> 1;
        Point vmid = poly[mid] - p0;
        if (cross(vmid, vq) >= 0) {
            idx = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    Point edge = poly[idx + 1] - poly[idx];
    Point toQ = q - poly[idx];
    return cross(edge, toQ) >= 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m1;
    if (!(cin >> m1)) return 0;
    vector<Point> poly1(m1);
    for (int i = 0; i < m1; i++) {
        cin >> poly1[i].x >> poly1[i].y;
    }

    int m2;
    if (!(cin >> m2)) return 0;
    vector<Point> poly2(m2);
    for (int i = 0; i < m2; i++) {
        cin >> poly2[i].x >> poly2[i].y;
    }

    int n;
    if (!(cin >> n)) return 0;
    vector<Point> minor_plants(n);
    for (int i = 0; i < n; i++) {
        cin >> minor_plants[i].x >> minor_plants[i].y;
    }

    // Case 1: 2 * Q in P1 + P2
    vector<Point> sum1 = minkowski(poly1, poly2);

    // Case 2: Q in 2 * P2 - P1 = 2 * P2 + (-P1)
    // Negating (x, y) -> (-x, -y) is a 180-deg rotation, so it preserves CCW orientation.
    vector<Point> negP1(m1);
    for (int i = 0; i < m1; i++) {
        negP1[i] = Point(-poly1[i].x, -poly1[i].y);
    }
    vector<Point> twoP2(m2);
    for (int i = 0; i < m2; i++) {
        twoP2[i] = Point(2 * poly2[i].x, 2 * poly2[i].y);
    }
    vector<Point> sum2 = minkowski(twoP2, negP1);

    // Case 3: Q in 2 * P1 - P2 = 2 * P1 + (-P2)
    vector<Point> negP2(m2);
    for (int i = 0; i < m2; i++) {
        negP2[i] = Point(-poly2[i].x, -poly2[i].y);
    }
    vector<Point> twoP1(m1);
    for (int i = 0; i < m1; i++) {
        twoP1[i] = Point(2 * poly1[i].x, 2 * poly1[i].y);
    }
    vector<Point> sum3 = minkowski(twoP1, negP2);

    string ans = "";
    ans.reserve(n);
    for (int i = 0; i < n; i++) {
        const Point& q = minor_plants[i];
        Point twoQ(2 * q.x, 2 * q.y);
        bool ok = point_in_convex_polygon(sum1, twoQ) ||
                  point_in_convex_polygon(sum2, q) ||
                  point_in_convex_polygon(sum3, q);
        ans.push_back(ok ? 'Y' : 'N');
    }

    cout << ans << "\n";
    return 0;
}

