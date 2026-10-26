#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int theta, n;
    if (!(cin >> theta >> n)) return 0;

    const double PI = acos(-1.0);
    double rad = theta * PI / 180.0;
    double tan_val = tan(rad);

    vector<pair<int, int>> flatlanders(n);
    for (int i = 0; i < n; i++) {
        cin >> flatlanders[i].first >> flatlanders[i].second;
    }

    sort(flatlanders.begin(), flatlanders.end());

    double total_shadow = 0.0;
    double cur_left = flatlanders[0].first;
    double cur_right = flatlanders[0].first + flatlanders[0].second / tan_val;

    for (int i = 1; i < n; i++) {
        double r_i = flatlanders[i].first + flatlanders[i].second / tan_val;
        if (flatlanders[i].first <= cur_right) {
            cur_right = max(cur_right, r_i);
        } else {
            total_shadow += cur_right - cur_left;
            cur_left = flatlanders[i].first;
            cur_right = r_i;
        }
    }
    total_shadow += cur_right - cur_left;

    cout << fixed << setprecision(10) << total_shadow << "\n";

    return 0;
}
