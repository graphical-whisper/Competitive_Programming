#include <iostream>
#include <string>
#include <vector>

using namespace std;

string get_feedback(const string& guess, const string& secret) {
    string fb = "";
    for (int i = 0; i < 5; i++) {
        if (guess[i] == secret[i]) {
            fb += '*';
        } else if (secret.find(guess[i]) != string::npos) {
            fb += '!';
        } else {
            fb += 'X';
        }
    }
    return fb;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> dict(n);
    for (int i = 0; i < n; i++) {
        cin >> dict[i];
    }

    const string& secret = dict[0];

    int g;
    cin >> g;
    vector<string> feedbacks(g);
    for (int i = 0; i < g; i++) {
        cin >> feedbacks[i];
    }

    // Precalculate feedback for all words against secret
    vector<string> word_feedbacks(n);
    for (int i = 0; i < n; i++) {
        word_feedbacks[i] = get_feedback(dict[i], secret);
    }

    for (int i = 0; i < g; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (word_feedbacks[j] == feedbacks[i]) {
                count++;
            }
        }
        cout << count << "\n";
    }

    return 0;
}

