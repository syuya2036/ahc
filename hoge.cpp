#include <bits/stdc++.h>
// #include <chrono>
using namespace std;
double rand01();
unsigned int randxor();
struct Rect {
    long h, w;
    Rect(long h, long w) : h(h), w(w) {}
};
vector<Rect> R;
int N, T, sigma;

void solve() {
    cout<<N<<endl;
    for(int i=0; i<N; i++) {
        char d = 'L';
        int r = -1;
        cout<<i<<" "<<0<<" "<<d<<" "<<r<<endl;
    }
    // time keeping
    //  auto start = chrono::system_clock::now();
    //  while (chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count() < 100)
    //  {
    //      break;
    //  }
}
int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cin>>N>>T>>sigma;
    int h, w;
    for(int i=0; i<N; ++i) {
        cin>>h>>w;
        R.push_back(Rect(h, w));
    }
    const char* local = getenv("LOCAL");
    if(local) {
        long dh, dw;
        for(int i=0; i<N; i++) cin>>dh>>dw;
    }

    int Tp = T;
    long resh, resw;
    while(Tp--) {
        solve();
        cin>>resh>>resw;
    }
}

unsigned int randxor()
{
    static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned int t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}

double rand01() {
    return (double)randxor() / UINT_MAX;
}

