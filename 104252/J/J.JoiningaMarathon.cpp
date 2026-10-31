#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// Fast I/O for large inputs
struct FastInput {
    static const int BUF_SIZE = 1 << 20;
    char buf[BUF_SIZE];
    int pos = 0, len = 0;

    char next_char() {
        if (pos >= len) {
            pos = 0;
            len = (int)fread(buf, 1, BUF_SIZE, stdin);
            if (len <= 0) return EOF;
        }
        return buf[pos++];
    }

    bool read_long(long long &x) {
        char c = next_char();
        while (c != EOF && c <= ' ') c = next_char();
        if (c == EOF) return false;
        long long sign = 1;
        if (c == '-') {
            sign = -1;
            c = next_char();
        }
        x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = next_char();
        }
        x *= sign;
        return true;
    }

    bool read_int(int &x) {
        long long tmp = 0;
        if (read_long(tmp)) {
            x = (int)tmp;
            return true;
        }
        return false;
    }
} in;

struct TimeFraction {
    long long num, den; // den > 0

    bool operator<(const TimeFraction& o) const {
        return (__int128)num * o.den < (__int128)o.num * den;
    }
    bool operator>(const TimeFraction& o) const {
        return (__int128)num * o.den > (__int128)o.num * den;
    }
    bool operator==(const TimeFraction& o) const {
        return (__int128)num * o.den == (__int128)o.num * den;
    }
    bool operator<=(const TimeFraction& o) const {
        return !(o < *this);
    }
};

struct Runner {
    long long T = 0, S = 0;
    int id = 0;
};

struct Photo {
    long long U = 0, A = 0, B = 0;
    int id = 0;
};

bool get_cross_time(const Runner& u, const Runner& v, TimeFraction& out_t) {
    if (u.S == v.S) return false;
    long long num = u.T * u.S - v.T * v.S;
    long long den = u.S - v.S;
    if (den < 0) {
        num = -num;
        den = -den;
    }
    long long max_T = max(u.T, v.T);
    if ((__int128)num >= (__int128)max_T * den) {
        out_t = {num, den};
        return true;
    }
    return false;
}

struct Fenwick {
    int n;
    vector<int> tree;
    Fenwick(int n = 0) : n(n), tree((size_t)(n + 2), 0) {}

    void add(int i, int delta) {
        for (; i <= n; i += i & -i) tree[(size_t)i] += delta;
    }

    int query(int i) const {
        int sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[(size_t)i];
        return sum;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R = 0;
    if (!in.read_int(R)) return 0;

    vector<Runner> runners((size_t)R);
    for (int i = 0; i < R; i++) {
        in.read_long(runners[(size_t)i].T);
        in.read_long(runners[(size_t)i].S);
        runners[(size_t)i].id = i;
    }

    int P = 0;
    in.read_int(P);
    vector<Photo> photos((size_t)P);
    for (int i = 0; i < P; i++) {
        in.read_long(photos[(size_t)i].U);
        in.read_long(photos[(size_t)i].A);
        in.read_long(photos[(size_t)i].B);
        photos[(size_t)i].id = i;
    }

    int Q = 0;
    in.read_int(Q);
    vector<Runner> johnnies((size_t)Q);
    for (int i = 0; i < Q; i++) {
        in.read_long(johnnies[(size_t)i].T);
        in.read_long(johnnies[(size_t)i].S);
        johnnies[(size_t)i].id = i;
    }

    // Sort photos chronologically by time U
    sort(photos.begin(), photos.end(), [](const Photo& a, const Photo& b) {
        return a.U < b.U;
    });

    // Sweep 1: Find all trash photos
    struct Event1 {
        TimeFraction t;
        int type; // 0: START, 1: CROSS
        int u, v;

        bool operator<(const Event1& o) const {
            if (!(t == o.t)) return t > o.t;
            return type > o.type;
        }
    };

    priority_queue<Event1> pq1;
    for (int i = 0; i < R; i++) {
        pq1.push({{runners[(size_t)i].T, 1}, 0, i, -1});
    }

    vector<int> L;
    L.reserve((size_t)R);
    vector<int> pos_in_L((size_t)R, -1);

    auto check_cross1 = [&](int idx1, int idx2, TimeFraction cur_t) {
        if (idx1 < 0 || idx2 >= (int)L.size() || idx1 >= idx2) return;
        int u = L[(size_t)idx1];
        int v = L[(size_t)idx2];
        TimeFraction ct;
        if (get_cross_time(runners[(size_t)u], runners[(size_t)v], ct)) {
            if (runners[(size_t)u].S > runners[(size_t)v].S && (ct > cur_t || ct == cur_t)) {
                pq1.push({ct, 1, u, v});
            }
        }
    };

    vector<bool> is_trash((size_t)P, false);
    int photo_ptr = 0;

    while (!pq1.empty() || photo_ptr < P) {
        if (photo_ptr < P && (pq1.empty() || TimeFraction{photos[(size_t)photo_ptr].U, 1} < pq1.top().t)) {
            long long U = photos[(size_t)photo_ptr].U;
            long long A = photos[(size_t)photo_ptr].A;
            long long B = photos[(size_t)photo_ptr].B;

            int low = 0, high = (int)L.size() - 1, first_ge = (int)L.size();
            while (low <= high) {
                int mid = (low + high) / 2;
                int r = L[(size_t)mid];
                long long pos = (U - runners[(size_t)r].T) * runners[(size_t)r].S;
                if (pos >= A) {
                    first_ge = mid;
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }

            bool has_runner = false;
            if (first_ge < (int)L.size()) {
                int r = L[(size_t)first_ge];
                long long pos = (U - runners[(size_t)r].T) * runners[(size_t)r].S;
                if (pos <= B) has_runner = true;
            }
            if (!has_runner) {
                is_trash[(size_t)photo_ptr] = true;
            }
            photo_ptr++;
            continue;
        }

        Event1 e = pq1.top();
        pq1.pop();

        if (e.type == 0) { // START
            int u = e.u;
            int insert_pos = 0;
            while (insert_pos < (int)L.size()) {
                int other = L[(size_t)insert_pos];
                if (runners[(size_t)other].T == runners[(size_t)u].T) {
                    if (runners[(size_t)other].S < runners[(size_t)u].S) insert_pos++;
                    else break;
                } else break;
            }
            L.insert(L.begin() + insert_pos, u);
            for (int i = insert_pos; i < (int)L.size(); i++) {
                pos_in_L[(size_t)L[(size_t)i]] = i;
            }
            check_cross1(insert_pos - 1, insert_pos, e.t);
            check_cross1(insert_pos, insert_pos + 1, e.t);
        } else if (e.type == 1) { // CROSS
            int u = e.u, v = e.v;
            int pu = pos_in_L[(size_t)u], pv = pos_in_L[(size_t)v];
            if (pu != -1 && pv != -1 && pv == pu + 1) {
                swap(L[(size_t)pu], L[(size_t)pv]);
                pos_in_L[(size_t)u] = pv;
                pos_in_L[(size_t)v] = pu;
                check_cross1(pu - 1, pu, e.t);
                check_cross1(pv, pv + 1, e.t);
            }
        }
    }

    int total_trash = 0;
    vector<Photo> trash_photos;
    trash_photos.reserve((size_t)P);
    for (int i = 0; i < P; i++) {
        if (is_trash[(size_t)i]) {
            total_trash++;
            trash_photos.push_back(photos[(size_t)i]);
        }
    }

    if (total_trash == 0) {
        for (int i = 0; i < Q; i++) {
            cout << 0 << "\n";
        }
        return 0;
    }

    // Sweep 2: Process queries against trash photos
    struct Event2 {
        TimeFraction t;
        int type; // 0: START, 1: CROSS
        int u, v;

        bool operator<(const Event2& o) const {
            if (!(t == o.t)) return t > o.t;
            return type > o.type;
        }
    };

    priority_queue<Event2> pq2;
    for (int i = 0; i < Q; i++) {
        pq2.push({{johnnies[(size_t)i].T, 1}, 0, i, -1});
    }

    vector<int> L2;
    L2.reserve((size_t)Q);
    vector<int> pos_in_L2((size_t)Q, -1);
    Fenwick bit(Q + 5);

    auto check_cross2 = [&](int idx1, int idx2, TimeFraction cur_t) {
        if (idx1 < 0 || idx2 >= (int)L2.size() || idx1 >= idx2) return;
        int u = L2[(size_t)idx1];
        int v = L2[(size_t)idx2];
        TimeFraction ct;
        if (get_cross_time(johnnies[(size_t)u], johnnies[(size_t)v], ct)) {
            if (johnnies[(size_t)u].S > johnnies[(size_t)v].S && (ct > cur_t || ct == cur_t)) {
                pq2.push({ct, 1, u, v});
            }
        }
    };

    int trash_ptr = 0;
    int num_trash = (int)trash_photos.size();

    while (!pq2.empty() || trash_ptr < num_trash) {
        if (trash_ptr < num_trash && (pq2.empty() || TimeFraction{trash_photos[(size_t)trash_ptr].U, 1} < pq2.top().t)) {
            long long U = trash_photos[(size_t)trash_ptr].U;
            long long A = trash_photos[(size_t)trash_ptr].A;
            long long B = trash_photos[(size_t)trash_ptr].B;

            int low = 0, high = (int)L2.size() - 1, first_ge = (int)L2.size();
            while (low <= high) {
                int mid = (low + high) / 2;
                int jid = L2[(size_t)mid];
                long long pos = (U - johnnies[(size_t)jid].T) * johnnies[(size_t)jid].S;
                if (pos >= A) {
                    first_ge = mid;
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }

            low = 0; high = (int)L2.size() - 1; int last_le = -1;
            while (low <= high) {
                int mid = (low + high) / 2;
                int jid = L2[(size_t)mid];
                long long pos = (U - johnnies[(size_t)jid].T) * johnnies[(size_t)jid].S;
                if (pos <= B) {
                    last_le = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }

            if (first_ge <= last_le) {
                bit.add(first_ge + 1, 1);
                bit.add(last_le + 2, -1);
            }
            trash_ptr++;
            continue;
        }

        Event2 e = pq2.top();
        pq2.pop();

        if (e.type == 0) { // START
            int u = e.u;
            int sz = (int)L2.size();
            vector<int> vals((size_t)sz);
            for (int i = 0; i < sz; i++) {
                vals[(size_t)i] = bit.query(i + 1);
            }

            int insert_pos = 0;
            while (insert_pos < sz) {
                int other = L2[(size_t)insert_pos];
                if (johnnies[(size_t)other].T == johnnies[(size_t)u].T) {
                    if (johnnies[(size_t)other].S < johnnies[(size_t)u].S) insert_pos++;
                    else break;
                } else break;
            }

            L2.insert(L2.begin() + insert_pos, u);
            vals.insert(vals.begin() + insert_pos, 0);
            for (int i = insert_pos; i < (int)L2.size(); i++) {
                pos_in_L2[(size_t)L2[(size_t)i]] = i;
            }

            bit = Fenwick(Q + 5);
            for (int i = 0; i < (int)L2.size(); i++) {
                int diff = vals[(size_t)i] - (i > 0 ? vals[(size_t)(i - 1)] : 0);
                bit.add(i + 1, diff);
            }

            check_cross2(insert_pos - 1, insert_pos, e.t);
            check_cross2(insert_pos, insert_pos + 1, e.t);
        } else if (e.type == 1) { // CROSS
            int u = e.u, v = e.v;
            int pu = pos_in_L2[(size_t)u], pv = pos_in_L2[(size_t)v];
            if (pu != -1 && pv != -1 && pv == pu + 1) {
                int val_u = bit.query(pu + 1);
                int val_v = bit.query(pv + 1);
                int delta = val_v - val_u;

                bit.add(pu + 1, delta);
                bit.add(pv + 1, -2 * delta);
                if (pv + 2 <= Q) bit.add(pv + 2, delta);

                swap(L2[(size_t)pu], L2[(size_t)pv]);
                pos_in_L2[(size_t)u] = pv;
                pos_in_L2[(size_t)v] = pu;

                check_cross2(pu - 1, pu, e.t);
                check_cross2(pv, pv + 1, e.t);
            }
        }
    }

    for (int i = 0; i < Q; i++) {
        int saved = 0;
        if (pos_in_L2[(size_t)i] != -1) {
            saved = bit.query(pos_in_L2[(size_t)i] + 1);
        }
        cout << (total_trash - saved) << "\n";
    }

    return 0;
}
