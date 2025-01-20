#include <bits/stdc++.h>
using namespace std;

static inline double nowSec()
{
    static auto start = chrono::steady_clock::now();
    auto cur = chrono::steady_clock::now();
    return chrono::duration<double>(cur - start).count();
}

void buildParentsFromOrder(
    const vector<int> &order,
    const vector<vector<int>> &G_desc,
    int N, int H,
    vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -2);
    vector<bool> visited(N, false);

    function<void(int, int)> dfs = [&](int u, int depth)
    {
        visited[u] = true;
        if (depth == H)
            return;
        for (int nx : G_desc[u])
        {
            if (!visited[nx])
            {
                parent[nx] = u;
                dfs(nx, depth + 1);
            }
        }
    };

    for (int idx = 0; idx < N; idx++)
    {
        int r = order[idx];
        if (!visited[r])
        {
            parent[r] = -1;
            dfs(r, 0);
        }
    }
}

long long computeScoreCheck(
    const vector<int> &parent,
    const vector<int> &A,
    const vector<vector<int>> &G,
    int N, int H)
{
    vector<vector<int>> children(N);
    for (int v = 0; v < N; v++)
    {
        int p = parent[v];
        if (p >= 0)
        {
            children[p].push_back(v);
        }
    }

    vector<bool> visited(N, false);
    long long score = 0LL;

    for (int v = 0; v < N; v++)
    {
        if (parent[v] == -1)
        {
            queue<int> q;
            q.push(v);
            vector<int> depth(N, -1);
            depth[v] = 0;

            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                if (visited[u])
                    return -1;
                visited[u] = true;
                score += (long long)(depth[u] + 1) * A[u];

                for (int c : children[u])
                {
                    if (depth[c] >= 0)
                    {
                        if (!visited[c])
                            return -1;
                        continue;
                    }
                    depth[c] = depth[u] + 1;
                    if (depth[c] > H)
                        return -1;
                    q.push(c);
                }
            }
        }
    }

    for (int v = 0; v < N; v++)
    {
        if (!visited[v])
            return -1;
    }
    return score;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, H;
    cin >> N >> M >> H;
    vector<int> A(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A[i];
    }

    vector<vector<int>> G(N);
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    vector<int> X(N), Y(N);
    for (int i = 0; i < N; i++)
    {
        cin >> X[i] >> Y[i];
    }

    vector<vector<int>> G_desc(N);
    for (int u = 0; u < N; u++)
    {
        auto &adj = G[u];
        sort(adj.begin(), adj.end(), [&](int a, int b)
             { return A[a] > A[b]; });
        G_desc[u] = adj;
    }

    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b)
         { return A[a] < A[b]; });

    vector<int> parent(N, -2);
    buildParentsFromOrder(order, G_desc, N, H, parent);

    long long curScore = computeScoreCheck(parent, A, G, N, H);
    if (curScore < 0)
    {
        for (int i = 0; i < N; i++)
        {
            parent[i] = -1;
        }
        curScore = computeScoreCheck(parent, A, G, N, H);
    }

    long long bestScore = curScore;
    vector<int> bestParent = parent;
    vector<int> bestOrder = order;
    vector<vector<int>> bestG_desc = G_desc;

    double timeLimit = 1.97;
    double startTime = nowSec();
    double T0 = 3e3;
    double T1 = 10;

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> randNode(0, N - 1);
    uniform_real_distribution<double> rand01(0.0, 1.0);
    uniform_int_distribution<int> randMoveType(0, 1);

    long long iteration = 0;

    while (true)
    {
        double t = nowSec();
        if (t - startTime > timeLimit)
            break;
        iteration++;

        double progress = (t - startTime) / timeLimit;
        double T = exp(log(T0) + (log(T1) - log(T0)) * progress);

        int moveType = randMoveType(mt);

        auto oldOrder = order;
        auto oldGdesc = G_desc;

        bool revertFlag = false;

        if (moveType == 0)
        {
            int i = randNode(mt), j = randNode(mt);
            if (i != j)
            {
                swap(order[i], order[j]);
            }
            else
            {
                continue;
            }
        }
        else
        {
            int u = randNode(mt);
            auto &adjList = G_desc[u];
            if ((int)adjList.size() < 2)
            {
                continue;
            }
            uniform_int_distribution<int> randAdj(0, (int)adjList.size() - 1);
            int p = randAdj(mt), q = randAdj(mt);
            if (p == q)
                continue;
            swap(adjList[p], adjList[q]);
        }

        buildParentsFromOrder(order, G_desc, N, H, parent);
        long long newScore = computeScoreCheck(parent, A, G, N, H);

        if (newScore < 0)
        {
            revertFlag = true;
        }
        else
        {
            long long d = newScore - curScore;
            if (d >= 0)
            {
                curScore = newScore;
                if (curScore > bestScore)
                {
                    bestScore = curScore;
                    bestParent = parent;
                    bestOrder = order;
                    bestG_desc = G_desc;
                }
            }
            else
            {
                double prob = exp(double(d) / T);
                double r = rand01(mt);
                if (r > prob)
                {
                    revertFlag = true;
                }
                else
                {
                    curScore = newScore;
                }
            }
        }

        if (revertFlag)
        {
            if (moveType == 0)
            {
                order = oldOrder;
            }
            else
            {
                G_desc = oldGdesc;
            }
        }
    }

    cerr << "[INFO] iteration = " << iteration
         << ", bestScore = " << bestScore << endl;

    {
        buildParentsFromOrder(bestOrder, bestG_desc, N, H, bestParent);
        for (int v = 0; v < N; v++)
        {
            cout << bestParent[v] << (v + 1 < N ? ' ' : '\n');
        }
    }
    return 0;
}
