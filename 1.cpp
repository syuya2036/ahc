#include <bits/stdc++.h>
#include<atcoder/dsu>
using namespace std;

int N, M, H;
int A[1010], X[1010], Y[1010];
vector<int> G[1010];
int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cin>>N>>M>>H;
    for(int i=0; i<N; i++) cin>>A[i];
    for(int i=0; i<M; i++) {
        int u, v; cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for(int i=0; i<N; i++) cin>>X[i]>>Y[i];
    for(int i=0; i<N; i++) cout<<"-1"<<" ";
    cout<<endl;
}
