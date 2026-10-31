#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

bool can_solve_orientation(int R, int C, const string& S, const vector<string>& maze) {
    vector<vector<bool>> valid(R, vector<bool>(C, true));
    for (int r = 0; r < R; r++) {
        for (int rot = 0; rot < C; rot++) {
            bool ok = true;
            for (int j = 0; j < C; j++) {
                if (S[j] == '1' && maze[r][(j + rot) % C] == '1') {
                    ok = false;
                    break;
                }
            }
            valid[r][rot] = ok;
        }
    }

    vector<vector<bool>> visited(R, vector<bool>(C, false));
    queue<pair<int, int>> q;

    for (int rot = 0; rot < C; rot++) {
        if (valid[0][rot]) {
            visited[0][rot] = true;
            q.push({0, rot});
        }
    }

    while (!q.empty()) {
        auto [r, rot] = q.front();
        q.pop();

        if (r + 1 == R) {
            return true;
        } else if (r + 1 < R && valid[r + 1][rot] && !visited[r + 1][rot]) {
            visited[r + 1][rot] = true;
            q.push({r + 1, rot});
        }

        if (r - 1 >= 0 && valid[r - 1][rot] && !visited[r - 1][rot]) {
            visited[r - 1][rot] = true;
            q.push({r - 1, rot});
        }

        int rot_l = (rot - 1 + C) % C;
        if (valid[r][rot_l] && !visited[r][rot_l]) {
            visited[r][rot_l] = true;
            q.push({r, rot_l});
        }

        int rot_r = (rot + 1) % C;
        if (valid[r][rot_r] && !visited[r][rot_r]) {
            visited[r][rot_r] = true;
            q.push({r, rot_r});
        }
    }

    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C;
    if (!(cin >> R >> C)) return 0;

    string S;
    cin >> S;

    vector<string> maze(R);
    for (int i = 0; i < R; i++) {
        cin >> maze[i];
    }

    bool ok = can_solve_orientation(R, C, S, maze);
    if (!ok) {
        reverse(S.begin(), S.end());
        ok = can_solve_orientation(R, C, S, maze);
    }

    cout << (ok ? "Y\n" : "N\n");
    return 0;
}

