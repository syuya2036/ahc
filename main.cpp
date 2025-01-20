#include <bits/stdc++.h>
#include <chrono>
using namespace std;
const int INF = 1e5;

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int N; cin>>N;
    int x, y;
    for(int i=0; i<2*N; ++i) {
        cin>>x>>y;
    }


    // time keeping
    auto start = chrono::system_clock::now();
    while (chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count() < 1900)
    {
        continue;
    }

    cout<<4<<endl;
    cout<<"0 0"<<endl;
    cout<<"100 0"<<endl;
    cout<<"100 100"<<endl;
    cout<<"0 100"<<endl;
}
