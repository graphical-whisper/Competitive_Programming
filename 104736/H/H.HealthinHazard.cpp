#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const double PI = acos(-1.0);
const double TWO_PI = 2.0 * PI;

struct Arc {
    double l, r;
};

struct Prediction {
    bool valid;
    vector<Arc> intervals;
};

Prediction make_prediction(long long x1, long long y1, long long x2, long long y2, double D) {
    long long A = y2 - y1;
    long long B = x1 - x2;
    long long C = -(A * x1 + B * y1);
    if (C < 0) {
        A = -A;
        B = -B;
        C = -C;
    }

    double len = sqrt((double)A * A + (double)B * B);
    double d = (double)C / len;

    if (d >= D) {
        return {false, {}};
    }

    double phi = atan2(-(double)B, -(double)A);
    if (phi < 0) phi += TWO_PI;

    double ratio = d / D;
    if (ratio > 1.0) ratio = 1.0;
    if (ratio < -1.0) ratio = -1.0;

    double alpha = acos(ratio);
    double L = phi - alpha;
    double R = phi + alpha;

    vector<Arc> intervals;
    if (L < 0) {
        intervals.push_back({L + TWO_PI, TWO_PI});
        intervals.push_back({0.0, R});
    } else if (R >= TWO_PI) {
        intervals.push_back({L, TWO_PI});
        intervals.push_back({0.0, R - TWO_PI});
    } else {
        intervals.push_back({L, R});
    }

    return {true, intervals};
}

bool check_coverage(int mid, const vector<Prediction>& preds) {
    vector<Arc> all_intervals;
    all_intervals.reserve(2 * mid);
    for (int i = 0; i < mid; ++i) {
        if (preds[i].valid) {
            for (const auto& arc : preds[i].intervals) {
                all_intervals.push_back(arc);
            }
        }
    }

    if (all_intervals.empty()) return false;

    sort(all_intervals.begin(), all_intervals.end(), [](const Arc& a, const Arc& b) {
        return a.l < b.l;
    });

    if (all_intervals[0].l > 1e-9) return false;

    double max_r = all_intervals[0].r;
    for (size_t i = 1; i < all_intervals.size(); ++i) {
        if (all_intervals[i].l > max_r + 1e-9) {
            return false;
        }
        max_r = max(max_r, all_intervals[i].r);
    }

    return max_r >= TWO_PI - 1e-9;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    double d;
    if (!(cin >> n >> d)) return 0;

    vector<Prediction> preds;
    preds.reserve(n);
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        preds.push_back(make_prediction(x1, y1, x2, y2, d));
    }

    int low = 1, high = n, ans = -1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (check_coverage(mid, preds)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (ans == -1) {
        cout << "*\n";
    } else {
        cout << ans << "\n";
    }

    return 0;
}

