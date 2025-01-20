#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static inline double nowSec(){
    static chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
    auto cur = chrono::steady_clock::now();
    return chrono::duration<double>(cur - start).count();
}
ll computeScoreCheck(
    const vector<int>& parent,
    const vector<vector<int>>& children,
    const vector<int>& A,
    int N, int H
) {
    vector<bool> visited(N, false);
    ll scoreSum = 0LL;

    for(int v = 0; v < N; v++){
        if(parent[v] == -1){
            queue<int>q;
            q.push(v);
            vector<int> depth(N, -1);
            depth[v] = 0;

            while(!q.empty()){
                int u = q.front(); q.pop();
                if(visited[u]) return -1;
                visited[u] = true;

                scoreSum += (ll)(depth[u] + 1) * A[u];

                for(int c : children[u]){
                    if(depth[c] != -1){
                        if(!visited[c]) return -1;
                        continue;
                    }
                    depth[c] = depth[u] + 1;
                    if(depth[c] > H) return -1;
                    q.push(c);
                }
            }
        }
    }
    for(int v = 0; v < N; v++) if(!visited[v]) return -1;

    return scoreSum;
}

vector<vector<int>> buildChildren(const vector<int>& parent, int N){
    vector<vector<int>> children(N);
    for(int v = 0; v < N; v++){
        int p = parent[v];
        if(p >= 0){
            children[p].push_back(v);
        }
    }
    return children;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, H;
    cin>>N>>M>>H;

    vector<int> A(N);
    for(int i=0; i<N; i++) cin>>A[i];

    vector<vector<int>> G(N);
    for(int i=0; i<M; i++){
        int u, v;
        cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    vector<int> X(N), Y(N);
    for(int i=0; i<N; i++) cin>>X[i]>>Y[i];

    vector<vector<int>> G_desc(N);
    for(int u=0; u<N; u++){
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

    function<void(int,int)> dfs = [&](int u, int depth) {
        visited[u] = true;
        if(depth == H) return;
        for(int nx : G_desc[u]){
            if(!visited[nx]){
                parent[nx] = u;
                dfs(nx, depth+1);
            }
        }
    };

    for(int i=0;i<N;i++){
        int r = order[i];
        if(!visited[r]){
            parent[r] = -1;
            dfs(r, 0);
        }
    }

    auto children = buildChildren(parent, N);
    ll curScore = computeScoreCheck(parent, children, A, N, H);
    if(curScore < 0){
        for(int i=0;i<N;i++) parent[i] = -1;
        children = buildChildren(parent, N);
        curScore = computeScoreCheck(parent, children, A, N, H);
    }

    ll bestScore = curScore;
    vector<int> bestParent = parent;

    double T0 = 3e2;
    double T1 = 1e-10;
    double timeLimit = 1.9;
    double startTime = nowSec();
    int iterCount = 0;

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> randNode(0, N-1);

    while(true){
        double t = nowSec();
        if(t - startTime > timeLimit) break;
        iterCount++;
        double progress = (t - startTime) / timeLimit;
        double T = exp(log(T0) + (log(T1) - log(T0))*progress);

        int v = randNode(mt);
        if(parent[v] == -2) continue;
        if(parent[v] == -1) continue;

        int p0 = parent[v];
        auto &adj = G[v];
        if(adj.empty()) continue;
        int w = adj[ mt() % adj.size() ];

        if(w == p0) continue;
        parent[v] = w;

        auto newChildren = buildChildren(parent, N);
        ll newScore = computeScoreCheck(parent, newChildren, A, N, H);

        if(newScore < 0){
            parent[v] = p0;
        } else {
            ll d = newScore - curScore;
            if(d >= 0){
                curScore = newScore;
                children = move(newChildren);
                if(curScore > bestScore){
                    bestScore = curScore;
                    bestParent = parent;
                }
            } else {
                double prob = exp(double(d)/T);
                double r = uniform_real_distribution<>(0.0,1.0)(mt);
                if(r < prob){
                    curScore = newScore;
                    children = move(newChildren);
                } else {
                    parent[v] = p0;
                }
            }
        }
        if(iterCount%100==0) for(int v=0; v<N; v++) cout<<bestParent[v]<<(v+1<N?" ":"\n");
    }
    for(int v=0; v<N; v++) cout<<bestParent[v]<<(v+1<N?" ":"\n");
}
