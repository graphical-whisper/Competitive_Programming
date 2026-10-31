#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string T;
    int D;
    if (!(cin >> T >> D)) return 0;

    int n = T.size();
    int instructions = 0;
    int start = 0;

    vector<int> pi;
    pi.reserve(n);

    while (start < n) {
        instructions++;
        pi.clear();
        pi.push_back(0);

        int j = start + 1;
        while (j < n) {
            int k = pi.back();
            while (k > 0 && T[j] != T[start + k]) {
                k = pi[k - 1];
            }
            if (T[j] == T[start + k]) {
                k++;
            }
            int len = (j - start + 1);
            if (len - k > D) {
                break;
            }
            pi.push_back(k);
            j++;
        }
        start = j;
    }

    cout << instructions << "\n";
    return 0;
}

