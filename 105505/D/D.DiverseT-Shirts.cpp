#include <iostream>
#include <string>
#include <vector>


using namespace std;

const int MAXN = 1005;
const int MAXP = 2005; // Máximo número de propiedades (2 * MAXN)
const int INF = 1e9;

string A[MAXN];
int edge_color[MAXN][MAXN];
int prop_u[MAXN], prop_v[MAXN];

// Variables para el grafo bipartito
vector<int> adj[MAXP];
int side[MAXP];
int match_pair[MAXP], dist_hk[MAXP];
int total_properties;

// Funciones para Hopcroft-Karp
bool bfs_hk() {
  vector<int> q;
  q.reserve(total_properties);
  for (int i = 1; i <= total_properties; ++i) {
    if (side[i] == 0) { // Nodos del lado izquierdo
      if (match_pair[i] == 0) {
        dist_hk[i] = 0;
        q.push_back(i);
      } else {
        dist_hk[i] = INF;
      }
    }
  }
  dist_hk[0] = INF;
  int head = 0;
  while (head < (int)q.size()) {
    int u = q[head++];
    if (dist_hk[u] < dist_hk[0]) {
      for (int v : adj[u]) {
        if (dist_hk[match_pair[v]] == INF) {
          dist_hk[match_pair[v]] = dist_hk[u] + 1;
          q.push_back(match_pair[v]);
        }
      }
    }
  }
  return dist_hk[0] != INF;
}

bool dfs_hk(int u) {
  if (u != 0) {
    for (int v : adj[u]) {
      if (dist_hk[match_pair[v]] == dist_hk[u] + 1) {
        if (dfs_hk(match_pair[v])) {
          match_pair[v] = u;
          match_pair[u] = v;
          return true;
        }
      }
    }
    dist_hk[u] = INF;
    return false;
  }
  return true;
}

int hopcroft_karp() {
  for (int i = 0; i <= total_properties; ++i)
    match_pair[i] = 0;
  int matching = 0;
  while (bfs_hk()) {
    for (int i = 1; i <= total_properties; ++i) {
      if (side[i] == 0 && match_pair[i] == 0) {
        if (dfs_hk(i)) {
          matching++;
        }
      }
    }
  }
  return matching;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n;
  if (!(cin >> n))
    return 0;

  for (int i = 0; i < n; ++i) {
    cin >> A[i];
  }

  int curr_id = 0;
  vector<int> clique;
  clique.reserve(MAXN);

  // Identificación de propiedades (cliqués)
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (A[i][j] == '1' && edge_color[i][j] == 0) {
        curr_id++;
        clique.clear();
        clique.push_back(i);
        clique.push_back(j);

        for (int k = 0; k < n; ++k) {
          if (k != i && k != j && A[i][k] == '1' && A[j][k] == '1') {
            clique.push_back(k);
          }
        }

        int c_size = clique.size();
        for (int u = 0; u < c_size; ++u) {
          for (int v = u + 1; v < c_size; ++v) {
            edge_color[clique[u]][clique[v]] = curr_id;
            edge_color[clique[v]][clique[u]] = curr_id;
          }
        }
      }
    }
  }

  // Extracción de colores por nodo
  for (int i = 0; i < n; ++i) {
    int p1 = 0, p2 = 0;
    for (int j = 0; j < n; ++j) {
      if (A[i][j] == '1') {
        int col = edge_color[i][j];
        if (p1 == 0)
          p1 = col;
        else if (p1 != col && p2 == 0)
          p2 = col;
      }
    }
    if (p1 == 0)
      p1 = ++curr_id;
    if (p2 == 0)
      p2 = ++curr_id;

    prop_u[i] = p1;
    prop_v[i] = p2;
  }

  total_properties = curr_id;

  // Conectar grafo de propiedades y asignar bipartición (2-coloring)
  for (int i = 1; i <= total_properties; ++i)
    side[i] = -1;

  for (int i = 0; i < n; ++i) {
    adj[prop_u[i]].push_back(prop_v[i]);
    adj[prop_v[i]].push_back(prop_u[i]);
  }

  vector<int> q;
  q.reserve(total_properties);
  for (int i = 1; i <= total_properties; ++i) {
    if (side[i] == -1) {
      side[i] = 0;
      q.push_back(i);
      int head = 0;
      while (head < (int)q.size()) {
        int u = q[head++];
        for (int v : adj[u]) {
          if (side[v] == -1) {
            side[v] = 1 - side[u];
            q.push_back(v);
          }
        }
      }
    }
  }

  // Reconstruir lista de adyacencia dirigida (Lado Izquierdo a Lado Derecho)
  for (int i = 1; i <= total_properties; ++i)
    adj[i].clear();
  for (int i = 0; i < n; ++i) {
    int u = prop_u[i];
    int v = prop_v[i];
    if (side[u] == 1) {
      int temp = u;
      u = v;
      v = temp;
    }
    adj[u].push_back(v);
  }

  cout << hopcroft_karp() << "\n";
  return 0;
}