#include <bits/stdc++.h>
#include <chrono>
using namespace std;
const int INF = 1e5;

int N, M, H;
int A[1010], X[1010], Y[1010];
vector<int> G[1010];

int main(){
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    cin >>N>>M>>H;
    vector<pair<int, int>> perm(N);
    for(int i=0; i<N; i++){
        cin>>A[i];
        perm[i] = make_pair(A[i], i);
    }
    for(int i=0; i<M; i++){
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for(int i=0; i<N; i++){
        cin>>X[i]>>Y[i];
    }

    vector<int> parent(N, -1);
    vector<bool> used(N, false);
    sort(perm.begin(), perm.end());
    for(int node=0; node<N; node++) {
        int start = perm[node].second;
        if(!used[start]){
            parent[start] = -1;
            used[start] = true;
            queue<pair<int,int>> que;
            que.push({start, 0});
            while(!que.empty()){
                auto [u, depth] = que.front();
                que.pop();
                if(depth == H) continue;
                for(auto &nx : G[u]){
                    if(!used[nx]){
                        used[nx] = true;
                        parent[nx] = u;
                        que.push({nx, depth + 1});
                    }
                }
            }
        }
    }

    for(int i = 0; i < N; i++){
        cout<<parent[i]<<(i+1<N?" ":"\n");
    }
}
