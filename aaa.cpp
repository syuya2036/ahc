#include <bits/stdc++.h>
// #include <chrono>
using namespace std;
double rand01();
unsigned int randxor();
void printone(int p, int r, int d, int b) { cout<<p<<" "<<r<<" "<<(d==0?'U':'L')<<" "<<b<<endl; }
struct Rect {
    long h, w;
    Rect(long h, long w) : h(h), w(w) {}
};
vector<Rect> R;
int N, T, sigma;
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
    int sn = (int)sqrt(N);
    while(Tp--) {
        int W = sn - 2 + (Tp % 7);
        if(W <= 0) W = 1;
        int cnt = 0;
        cout<<N<<endl;
        while(true) {
            for(int i=0; i<W; i++) {
                int r = randxor() % 2 == 0 ? 0 : 1;
                if(i==0) printone(cnt, r, 1, -1);
                else printone(cnt, r, 1, cnt-1);
                cnt++;
                if(cnt == N) break;
            }
            if(cnt == N) break;
        }
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
