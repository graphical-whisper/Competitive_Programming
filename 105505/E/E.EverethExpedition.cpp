#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> a(n);
    vector<int> pos(n + 1, -1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (a[i] != 0) {
            pos[a[i]] = i;
        }
    }

    vector<int> next_orig(n, -1);
    vector<int> prev_orig(n, -1);

    int last = -1;
    for (int i = 0; i < n; i++) {
        prev_orig[i] = last;
        if (a[i] != 0) {
            last = i;
        }
    }

    last = -1;
    for (int i = n - 1; i >= 0; i--) {
        next_orig[i] = last;
        if (a[i] != 0) {
            last = i;
        }
    }

    int l = 0, r = n - 1;
    for (int v = 1; v <= n; v++) {
        if (pos[v] != -1) {
            // v was already present in the input
            if (pos[v] == l) {
                l++;
            } else if (pos[v] == r) {
                r--;
            } else {
                cout << "*\n";
                return 0;
            }
        } else {
            // v is missing and must be placed at an empty endpoint
            if (a[l] != 0 && a[r] != 0) {
                cout << "*\n";
                return 0;
            } else if (a[l] == 0 && a[r] != 0) {
                a[l] = v;
                l++;
            } else if (a[r] == 0 && a[l] != 0) {
                a[r] = v;
                r--;
            } else {
                // Both endpoints a[l] and a[r] are currently 0
                int dx = next_orig[l];
                int dy = prev_orig[r];

                if (dx == -1 || dx > r || dy == -1 || dy < l) {
                    // No original non-zero elements remaining in [l, r]
                    a[r] = v;
                    r--;
                } else {
                    int x = a[dx];
                    int y = a[dy];
                    if (x < y) {
                        a[l] = v;
                        l++;
                    } else if (y < x) {
                        a[r] = v;
                        r--;
                    } else {
                        // x == y: exactly one original non-zero element in [l, r]
                        if ((dx - l) < (r - dy)) {
                            a[l] = v;
                            l++;
                        } else {
                            a[r] = v;
                            r--;
                        }
                    }
                }
            }
        }
    }

    // Validate that 'a' is a strictly increasing then strictly decreasing permutation
    int peak_pos = -1;
    for (int i = 0; i < n; i++) {
        if (a[i] == n) {
            peak_pos = i;
            break;
        }
    }

    if (peak_pos == -1) {
        cout << "*\n";
        return 0;
    }

    for (int i = 0; i < peak_pos; i++) {
        if (a[i] >= a[i + 1]) {
            cout << "*\n";
            return 0;
        }
    }

    for (int i = peak_pos; i < n - 1; i++) {
        if (a[i] <= a[i + 1]) {
            cout << "*\n";
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        cout << a[i] << (i + 1 == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
