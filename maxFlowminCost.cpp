#include <iostream>
#include <vector>
#include <iomanip>
#include <set>

using namespace std;

struct Edge {
    int v, c, f, p;
    size_t backEd, id;
};

const int INF = 1e9;
vector<vector<Edge>> gr;
vector<int> dist;
vector<pair<int, int>> ancestor;

void addEd(int u, int v, int c, int p, size_t id) {
    Edge ed1 = {u, c, 0, p, gr[u].size(), id};
    Edge ed2 = {v, 0, 0, -p, gr[v].size(), id};
    gr[v].push_back(ed1);
    gr[u].push_back(ed2);
    
    ed1 = {u, 0, 0, -p, gr[u].size(), id};
    ed2 = {v, c, 0, p, gr[v].size(), id};
    gr[v].push_back(ed1);
    gr[u].push_back(ed2);
}

void pathDecomp(int s, int t, int n, int k) {
    int ans = 0;
    vector<vector<pair<int, int>>> solve;
    vector<bool> used(n, false);
    for (int j = 0; j < k; ++j) {
        vector<pair<int, int>> path;
        int v = s;
        used.assign(n, false);
        while (v != t) {
            used[v] = true;
            for (size_t i = 0; i < gr[v].size(); ++i) {
                if (gr[v][i].f <= 0)
                    continue;
                
                if (used[gr[v][i].v]) {
                    while (gr[path.back().first][path.back().second].v != gr[v][i].v)
                        path.pop_back();
                } else {
                    path.push_back({v, i});
                }
                v = gr[v][i].v;
                break;
            }
        }
        for (size_t i = 0; i < path.size(); ++i) {
            ans += gr[path[i].first][path[i].second].p;
            --gr[path[i].first][path[i].second].f;
        }
        solve.push_back(path);
    }
    cout << fixed << setprecision(10) << static_cast<long double>(ans)/k << "\n";
    for (size_t i = 0; i < solve.size(); ++i) {
        cout << solve[i].size() << " ";
        for (size_t j = 0; j < solve[i].size(); ++j)
            cout << gr[solve[i][j].first][solve[i][j].second].id << " ";
        cout << "\n";
    }
}

void dijkstra(int n, int s, int t) {
    vector<int> newDist(n, INF);
    ancestor.assign(n, {0, 0});
    newDist[s] = 0;
    set<pair<int, int>> q;
    q.insert({newDist[s], s});
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        for (int j = 0; j < gr[v].size(); ++j) {
            if (gr[v][j].c == 0)
                continue;
            int u = gr[v][j].v;
            if (newDist[v] + gr[v][j].p + dist[u] - dist[v] < newDist[u]) {
                q.erase({newDist[u], u});
                newDist[u] = newDist[v] + gr[v][j].p + dist[u] - dist[v];
                ancestor[u] = {v, j};
                q.insert({newDist[u], u});
            }
        }
    }
    dist = newDist;
}

int findMinPath(int s, int t, int n) {
    dijkstra(n, s, t);
    if (dist[t] >= INF)
        return 0;
    int v = t, minC = INF;
    while (v != s) {
        minC = min(minC, gr[ancestor[v].first][ancestor[v].second].c);
        v = ancestor[v].first;
    }
    v = t;
    while (v != s) {
        int u = ancestor[v].first, i = ancestor[v].second;
        gr[u][i].c -= minC;
        gr[u][i].f += minC;
        
        gr[v][gr[u][i].backEd].c += minC;
        gr[v][gr[u][i].backEd].f -= minC;
        v = u;
    }
    return minC;
}

int maxFlowMinCost(int n, int s, int t, int k) {
    int totalFlow = 0;
    while (true) {
        int flow = findMinPath(s, t, n);
        totalFlow += flow;
        if (totalFlow >= k || flow == 0)
            break;
    }
    return totalFlow;
}

int main() {
    int n, m, k, s, t;
    cin >> n >> m >> k;
    dist.resize(n, INF);
    ancestor.resize(n);
    gr.resize(n);
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        int v, u, cost;
        cin >> v >> u >> cost;
        addEd(u - 1, v - 1, 1, cost, i + 1);
    }
    if (maxFlowMinCost(n, s, t, k) >= k)
        pathDecomp(s, t, n, k);
    else
        cout << -1 << "\n";
}

