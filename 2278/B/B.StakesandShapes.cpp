#include <iostream>
#include <string>

using namespace std;

void solve() {
    int x, y, z;
    if (!(cin >> x >> y >> z)) return;

    int n = x + y + z;
    char choice;
    // Alice wins:
    // vs all R: Alice wins z, Bob wins y => Bob >= Alice if y >= z
    // vs all S: Alice wins x, Bob wins z => Bob >= Alice if z >= x
    // vs all P: Alice wins y, Bob wins x => Bob >= Alice if x >= y
    if (y >= z) {
        choice = 'R';
    } else if (z >= x) {
        choice = 'S';
    } else {
        choice = 'P';
    }

    string s(static_cast<size_t>(n), choice);
    cout << s << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 0;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}

