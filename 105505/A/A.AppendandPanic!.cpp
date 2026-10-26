#include <iostream>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (cin >> s) {
        int n = s.size();
        int len_t = n - 1;
        while (len_t > 0 && s[len_t - 1] < s[len_t]) {
            len_t--;
        }
        cout << len_t << "\n";
    }

    return 0;
}
