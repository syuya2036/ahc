#include <bits/stdc++.h>
// #include <chrono>
using namespace std;
using ll = long long;
double rand01();
unsigned int randxor();
void printone(int p, int r, int d, int b) { cout<<p<<" "<<r<<" "<<(d==0?'U':'L')<<" "<<b<<endl; }

// d=0:U,  d=1:L
struct Rect {
    ll h, w;
    ll i, j;
    int p, r, d, b;
    Rect(ll h, ll w, int p, int r, int d, int b) : h(h), w(w), p(p), r(r), d(d), b(b) {}
    Rect(ll h, ll w) : h(h), w(w) {}
    Rect() : h(-1), w(-1) {}
};
int N, T, sigma;
// 入力されたままのデータを保持
vector<Rect> R;
vector<Rect> Rt;
vector<pair<ll, ll>> eps;
struct State {
    ll max_right, max_bottom;
    vector<Rect> now;
    
    State() : max_right(0), max_bottom(0) {}

    ll get_score() {
        return min((ll)1e9, max(max_right, 1ll)) + min((ll)1e9, max(max_bottom, 1ll)); 
    }
    void push_now(Rect put_rect) {
        now.push_back(put_rect);
        max_bottom = max(max_bottom, put_rect.i + put_rect.h);
        max_right = max(max_right, put_rect.j + put_rect.w);
    }
    // now_rectはi,jだけ空にしておく
    void submit_now(Rect now_rect) {
        // 置く基準となるものを探す．
        int b = now_rect.b;
        Rect put_rect = now_rect;
        if(put_rect.r == 1) swap(put_rect.w, put_rect.h);
        Rect base;
        if(b == -1) {
            base = Rect(0, 0);
            base.i = 0;
            base.j = 0;
        }
        else base = now[b];
        if(put_rect.d == 1) { // d='L'
            ll top = base.i + base.h;
            ll bottom = top + put_rect.h;
            put_rect.i = top;
            put_rect.j = 0;
            for(int i=0; i<now.size(); i++) {
                if(now[i].i >= bottom || now[i].i + now[i].h <= top) continue;
                put_rect.j = max(put_rect.j, now[i].j + now[i].w)           ; 
                
            }
        } else if(put_rect.d == 0) { // d='U'
            ll left = base.j + base.w;
            ll right = left + put_rect.w;
            put_rect.j = left;
            put_rect.i = 0;
            for(int i=0; i<now.size(); i++) {
                if(now[i].j >= right || now[i].j + now[i].w <= left) continue;
                put_rect.i = max(put_rect.i, now[i].i + now[i].h);
            }
        }
        else assert(false);
        push_now(put_rect);
    }
};
ll sn;
ll lng;
random_device seed;
mt19937 engine(seed());
normal_distribution<> dist(0, sigma);
void solve() {
    State state;
    State test;
    for(int i=0; i<N; i++) {
        State next_state;
        State min_state;
        ll min_score = numeric_limits<ll>::max();
        for(int r=0; r<=1; r++)for(int cnt : {-1, i-1}) {
            next_state = state;
            next_state.submit_now(Rect(R[i].h, R[i].w, i, r, 0, cnt));
            int tr = 20;
            ll score = 0;
            while(tr--) {

                ll now_w = next_state.now.back().j + next_state.now.back().w;
                ll W = lng * sn + lng;
                if(rand01() > 0.5) W += randxor() % lng;
                else W -= randxor() % lng;

                State try_state = next_state;
                Rt.clear();
                for(int j=0; j<N; j++) {
                    Rect new_rect = R[j];
                    new_rect.h = new_rect.h + dist(engine);
                    new_rect.w = new_rect.w + dist(engine);
                    Rt.push_back(new_rect);
                }
                for(int k=i+1; k<N; k++) {
                    now_w += Rt[k].w;
                    if(now_w > W) { 
                        now_w = Rt[k].w;
                        try_state.submit_now(Rect(Rt[k].h, Rt[k].w, k, 0, 0, -1));
                        continue;
                    }
                    try_state.submit_now(Rect(Rt[k].h, Rt[k].w, k, 0, 0, k-1));
                }
                score += try_state.get_score() / 1000;
            }

            if(score < min_score) {
                min_score = score;
                // cout<<min_score<<endl;
                min_state = next_state;
            }
        }
        state = min_state;
    }
    
    cout<<state.now.size()<<endl;
    for(Rect rect : state.now) printone(rect.p, rect.r, rect.d, rect.b);
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cin>>N>>T>>sigma;
    ll h, w;

    for(int i=0; i<N; ++i) {
        cin>>w>>h;
        R.push_back(Rect(h, w));
    }
    sn = (long)sqrt(N);
    ll ha=0, wa=0;
    for(int i=0; i<N; i++) {
        ha += R[i].h;
        wa += R[i].w;
    }
    ha /= N;
    wa /= N;
    lng = (ha + wa) / 2;
        
    int Tp = T;
    ll resh, resw;

    auto start = chrono::system_clock::now();
    while(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count() < 2600) { 
        solve();
        cin>>resh>>resw;
        Tp--;
        if(Tp == 0) break;
    }
    for(int i=0; i<Tp; i++) {
        cout<<0<<endl;
        cin>>resh>>resw;
    }
}

unsigned int randxor() {
    static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned int t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}

double rand01() {
    return (double)randxor() / UINT_MAX;
}
double normal_dist(double x, double mu, double sigma){
  return 1 / pow(2 * M_PI * pow(sigma,2), 0.5) * exp(-1/(2 * pow(sigma,2)) * pow(x - mu,2));
}
