#include <bits/stdc++.h>
using namespace std;

void dfs(int u, int depth, int H,
         const vector<vector<int>>& G_desc,
         vector<int>& parent,
         vector<bool>& visited) {
    visited[u] = true;
    if(depth == H) return;
    for (auto &v : G_desc[u]) {
        if(!visited[v]) {
            parent[v] = u;
            dfs(v, depth + 1, H, G_desc, parent, visited);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, H;
    cin>>N>>M>>H;
    vector<int> A(N);
    for(int i = 0; i < N; i++){
        cin >> A[i];
    }

    vector<vector<int>> G(N), G_desc(N);
    for(int i = 0; i < M; i++){
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    vector<int> X(N), Y(N);
    for(int i = 0; i < N; i++){
        cin >> X[i] >> Y[i];
    }

    for(int u = 0; u < N; u++){
        auto &adj = G[u];
        sort(adj.begin(), adj.end(), [&](int a, int b){
            return A[a] > A[b];
        });
        G_desc[u] = adj;
    }

    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b){
        return A[a] < A[b];
    });

    vector<int> parent(N, -2);
    vector<bool> visited(N, false);

    for(int i = 0; i < N; i++){
        int r = order[i];
        if(!visited[r]){
            parent[r] = -1;
            dfs(r, 0, H, G_desc, parent, visited);
        }
    }

    for(int v = 0; v < N; v++) cout<<parent[v]<<(v+1<N?" ":"\n");

    return 0;
}

