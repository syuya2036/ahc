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
    long h, w;
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
    long sn = (long)sqrt(N);
    long ha=0, wa=0;
    for(int i=0; i<N; i++) {
        ha += R[i].h;
        wa += R[i].w;
    }
    ha /= N;
    wa /= N;
    long lng = (ha + wa) / 2;

    while(Tp--) {
        long W = lng * sn;
        if(rand01() > 0.5) W += randxor() % lng;
        else  W -= randxor() % lng;
        int cnt = 0;
        cout<<N<<endl;
        while(true) {
            long now_w = 0;
            int i = 0;
            while(now_w < W) {
                int r;
                if(i==0) r = rand01() >= 0.5 ? 0 : 1;
                else{
                    int bh = R[cnt-1].h;
                    if(abs(bh - R[cnt].h) > abs(bh - R[cnt].w)) r = 0;
                    else r = 1;
                    if(rand01() < 0.1) r = 1 - r;
                }
                if(i==0) printone(cnt, r, 0, -1);
                else printone(cnt, r, 0, cnt-1);
                cnt++;
                i++;
                now_w += R[cnt].w;
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
