#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (!(cin >> K)) return 0;

    if (K == 1) {
        cout << 0 << "\n";
        return 0;
    }

    int T = 0;
    while ((1 << T) < K) T++;

    // Connected backbone cells in 100x100 grid (1-based)
    vector<pair<int, int>> backbone;
    // Column 1 connects all rows
    for (int r = 1; r <= 100; r++) {
        backbone.push_back({r, 1});
    }
    // Odd rows r = 1, 3, 5, ..., 99 for c = 2..100
    for (int r = 1; r <= 100; r += 2) {
        for (int c = 2; c <= 100; c++) {
            backbone.push_back({r, c});
        }
    }

    // Available non-backbone cells: even rows r = 2, 4, ..., 100, c in 2..100
    vector<pair<int, int>> available;
    for (int r = 2; r <= 100; r += 2) {
        for (int c = 2; c <= 100; c++) {
            available.push_back({r, c});
        }
    }

    vector<vector<pair<int, int>>> toppings(T);
    for (int t = 0; t < T; t++) {
        toppings[t] = backbone;
    }

    // Assign K - 2 distinct masks to odd-row adjacent cells
    for (int i = 0; i < K - 2; i++) {
        int mask = i + 1;
        pair<int, int> cell = available[i];
        for (int t = 0; t < T; t++) {
            if ((mask >> t) & 1) {
                toppings[t].push_back(cell);
            }
        }
    }

    cout << T << "\n";
    for (int t = 0; t < T; t++) {
        cout << toppings[t].size();
        for (const auto& p : toppings[t]) {
            cout << " " << p.first << " " << p.second;
        }
        cout << "\n";
    }

    return 0;
}

