#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int MAXN = 500005;
const int LOGN = 20;

int pref_R[MAXN];
int pref_B[MAXN];
int lg2[MAXN];

// Mapping (cm + 1, -cR, cB):
// cm in {-1, 0, 1} -> cm + 1 in {0, 1, 2}
// cR in {-1, 0}    -> -cR in {1, 0}
// cB in {0, 1}     -> cB in {0, 1}
// Total 3 * 2 * 2 = 12 tables
int st[3][2][2][LOGN][MAXN];

void build_st(int n) {
    lg2[1] = 0;
    for (int i = 2; i <= n + 1; i++) {
        lg2[i] = lg2[i / 2] + 1;
    }

    for (int cm_idx = 0; cm_idx < 3; cm_idx++) {
        int cm = cm_idx - 1;
        for (int cr_idx = 0; cr_idx < 2; cr_idx++) {
            int cr = (cr_idx == 1 ? -1 : 0);
            for (int cb = 0; cb < 2; cb++) {
                if (cm == 0 && cr == 0 && cb == 0) continue;
                for (int m = 0; m <= n; m++) {
                    st[cm_idx][cr_idx][cb][0][m] = cm * m + cr * pref_R[m] + cb * pref_B[m];
                }
                for (int k = 1; k < LOGN; k++) {
                    int len = 1 << (k - 1);
                    for (int m = 0; m + (1 << k) <= n + 1; m++) {
                        st[cm_idx][cr_idx][cb][k][m] = min(st[cm_idx][cr_idx][cb][k - 1][m],
                                                           st[cm_idx][cr_idx][cb][k - 1][m + len]);
                    }
                }
            }
        }
    }
}

int query_st(int cm, int cr, int cb, int L, int R) {
    if (cm == 0 && cr == 0 && cb == 0) return 0;
    int cm_idx = cm + 1;
    int cr_idx = (cr == -1 ? 1 : 0);
    int len = R - L + 1;
    int k = lg2[len];
    return min(st[cm_idx][cr_idx][cb][k][L], st[cm_idx][cr_idx][cb][k][R - (1 << k) + 1]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R_cnt, B_cnt;
    if (!(cin >> R_cnt >> B_cnt)) return 0;

    string s;
    cin >> s;
    int n = s.size();

    vector<int> pos_R;
    vector<int> pos_B;
    pos_R.push_back(0); // 1-indexed
    pos_B.push_back(0);

    for (int i = 1; i <= n; i++) {
        pref_R[i] = pref_R[i - 1] + (s[i - 1] == 'R');
        pref_B[i] = pref_B[i - 1] + (s[i - 1] == 'B');
        if (s[i - 1] == 'R') pos_R.push_back(i);
        if (s[i - 1] == 'B') pos_B.push_back(i);
    }

    build_st(n);

    int total_R = (int)pos_R.size() - 1;
    int total_B = (int)pos_B.size() - 1;

    int q;
    cin >> q;
    while (q--) {
        int L, U;
        cin >> L >> U;

        // Cut points where m <= C vs m > C:
        int C1 = L + R_cnt - 1;

        int target_R = pref_R[L - 1] + R_cnt;
        int idx_R = (target_R <= total_R ? pos_R[target_R] : n + 1);
        int C2 = idx_R - 1;

        int C3 = U - B_cnt - 1;

        int target_B = pref_B[U] - B_cnt;
        int idx_B;
        if (target_B < 0) {
            idx_B = -1;
        } else if (target_B >= total_B) {
            idx_B = n;
        } else {
            idx_B = pos_B[target_B + 1] - 1;
        }
        int C4 = idx_B;

        vector<int> r_ends;
        auto add_cut = [&](int c) {
            if (c >= L - 1 && c < U) {
                r_ends.push_back(c);
            }
        };
        add_cut(C1);
        add_cut(C2);
        add_cut(C3);
        add_cut(C4);
        r_ends.push_back(U);

        sort(r_ends.begin(), r_ends.end());
        r_ends.erase(unique(r_ends.begin(), r_ends.end()), r_ends.end());

        int ans = 2e9;
        int cur_left = L - 1;

        for (int r : r_ends) {
            int a = cur_left;
            int b = r;
            cur_left = r + 1;
            if (a > b) continue;

            int m = a;
            int cm = 0, cr = 0, cb = 0, C = 0;

            // Left part
            if (m <= C1) {
                cm += 0;
                cr += -1;
                C += R_cnt + pref_R[L - 1];
            } else if (m <= C2) {
                cm += 1;
                cr += -1;
                C += -L + 1 + pref_R[L - 1];
            } else {
                cm += 1;
                cr += 0;
                C += -L + 1 - R_cnt;
            }

            // Right part
            if (m > C3) {
                cm += 0;
                cb += 1;
                C += B_cnt - pref_B[U];
            } else if (m > C4) {
                cm += -1;
                cb += 1;
                C += U - pref_B[U];
            } else {
                cm += -1;
                cb += 0;
                C += U - B_cnt;
            }

            int min_val = query_st(cm, cr, cb, a, b);
            ans = min(ans, C + min_val);
        }

        cout << ans << "\n";
    }

    return 0;
}
