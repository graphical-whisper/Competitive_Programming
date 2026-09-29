#include <iostream>
#include<algorithm>
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin>>t){
        while(t--){
            // x empleados, y proyectos, k meses
            long long x,y,k;
            cin>>x>>y>>k;

            long long D = y - x;
            long long total_projects = 0;

            // Calcular el módulo para los meses donde el número de empleados es menor a D
            long long limit = min(x + k - 1, D);
            for (long long E=x; E<=limit; E++){
                total_projects += D % E;
            }

            // Para los meses restantes donde el numero de empleados mayor D, el módulo es siempre D
            long long rem_terms = max(0LL, x + k - 1 - max(x - 1LL, D));
            total_projects += rem_terms * D;

            cout<<total_projects<<"\n";
        }
    }

    return 0;
}