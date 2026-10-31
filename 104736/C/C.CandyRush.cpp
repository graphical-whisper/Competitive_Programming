#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>

using namespace std;

struct Entry {
    uint64_t h1, h2;
    int idx;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
        cin >> c[i];
    }

    mt19937_64 rng(1337);
    vector<uint64_t> W1(k + 1), W2(k + 1);
    uint64_t S1 = 0, S2 = 0;
    for (int i = 1; i <= k; ++i) {
        W1[i] = rng();
        W2[i] = rng();
        S1 += W1[i];
        S2 += W2[i];
    }

    vector<int> val(k + 1, 0);
    vector<int> count_at_val(n + 1, 0);
    count_at_val[0] = k;

    int base_sub = 0;
    uint64_t cur_h1 = 0, cur_h2 = 0;

    vector<Entry> entries;
    entries.reserve(n + 1);
    entries.push_back({0, 0, 0});

    for (int i = 0; i < n; ++i) {
        int brand = c[i];
        int old_v = val[brand];
        count_at_val[old_v]--;
        val[brand]++;
        count_at_val[val[brand]]++;

        cur_h1 += W1[brand];
        cur_h2 += W2[brand];

        while (count_at_val[base_sub] == 0) {
            base_sub++;
        }

        uint64_t norm_h1 = cur_h1 - (uint64_t)base_sub * S1;
        uint64_t norm_h2 = cur_h2 - (uint64_t)base_sub * S2;

        entries.push_back({norm_h1, norm_h2, i + 1});
    }

    sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        if (a.h1 != b.h1) return a.h1 < b.h1;
        if (a.h2 != b.h2) return a.h2 < b.h2;
        return a.idx < b.idx;
    });

    int max_len = 0;
    for (size_t i = 0; i < entries.size(); ) {
        size_t j = i;
        while (j < entries.size() && entries[j].h1 == entries[i].h1 && entries[j].h2 == entries[i].h2) {
            j++;
        }
        max_len = max(max_len, entries[j - 1].idx - entries[i].idx);
        i = j;
    }

    cout << max_len << "\n";

    return 0;
}

