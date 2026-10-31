#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    unordered_map<int, int> freq;
    for (int i = 0; i < 3 * n; ++i) {
        int x;
        cin >> x;
        freq[x]++;
    }

    bool all_divisible_by_3 = true;
    for (const auto& kv : freq) {
        if (kv.second % 3 != 0) {
            all_divisible_by_3 = false;
            break;
        }
    }

    if (all_divisible_by_3) {
        cout << "N\n";
    } else {
        cout << "Y\n";
    }

    return 0;
}

