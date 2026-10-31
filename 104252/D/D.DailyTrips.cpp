#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, at_home, at_work;
    if (!(cin >> n >> at_home >> at_work)) return 0;

    for (int i = 0; i < n; i++) {
        char morning_rain, night_rain;
        cin >> morning_rain >> night_rain;

        char bring_work = 'N', bring_home = 'N';

        if (morning_rain == 'Y' || at_work == 0) {
            bring_work = 'Y';
            at_home--;
            at_work++;
        }

        if (night_rain == 'Y' || at_home == 0) {
            bring_home = 'Y';
            at_work--;
            at_home++;
        }

        cout << bring_work << " " << bring_home << "\n";
    }

    return 0;
}

