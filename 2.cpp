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

State greedy_action(State now_state, int idx) {
    vector<int> bs;
    for(int b=-1; b<idx; b++) bs.push_back(b);
    vector<int> ds = {0, 1};
    vector<int> rs = {0, 1};
    ll min_score = numeric_limits<ll>::max();
    State min_state;
    for(int b : bs)for(int d : ds)for(int r : rs) {
        State new_state = now_state;
        new_state.submit_now(Rect(Rt[idx].h, Rt[idx].w, idx, r, d, b));
        ll score = new_state.get_score();
        // cout<<idx<<" "<<r<<" "<<d<<" "<<b<<" "<<score<<endl;
       
        // スコアが同じ場合は右下の座標の和が小さい方を選ぶ
        if(score == min_score) {
            if(rand01() > 0.5) continue;
            int last = min_state.now.size() - 1;
            ll min_state_dr = min_state.now[last].i + min_state.now[last].h + min_state.now[last].j + min_state.now[last].w;
            ll new_state_dr = new_state.now[last].i + new_state.now[last].h + new_state.now[last].j + new_state.now[last].w;

            if(min_state_dr > new_state_dr) {
                min_state = new_state;
            }
        }
        if(score < min_score) {
            min_score = score;
            min_state = new_state;
        }
    }

    return min_state;
}

const bool dbg = false;
ll avrg;
void solve() {
    State state;
    std::random_device seed;
	std::mt19937 engine(seed());
	std::normal_distribution<> dist(0, sigma);
    eps.clear();
    Rt.clear();
    for(int i=0; i<N; i++) {
        eps.emplace_back(dist(engine), dist(engine));
    }
    for(int i=0; i<N; i++) {
        Rect new_rect = R[i];
        new_rect.h = new_rect.h + eps[i].first;
        new_rect.w = new_rect.w + eps[i].second;
        Rt.push_back(new_rect);
    } 

    for(int i=0; i<N; i++) {
        if(i==0) {
            Rect zero = Rect(Rt[0].h, Rt[0].w, 0, 0, 0, -1);
            zero.i = 0;
            zero.j = 0;
            state.push_now(zero);
        }
        else {
            state = greedy_action(state, i);
        }
    }
    
    if(dbg) {
         for(auto rect : state.now) { 
             cout<<rect.p<<" "<<rect.i<<" "<<rect.j<<" "<<rect.h<<" "<<rect.w<<endl;
         }
         cout<<state.get_score()<<endl;
         printf("H=%lld, W=%lld", state.max_bottom, state.max_right);
    }
    cout<<state.now.size()<<endl;
    for(auto rect : state.now) {
        printone(rect.p, rect.r, rect.d, rect.b);
    }
    // cout<<state.get_score()<<endl;
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
    int Tp = T-1;
    ll resh, resw;
    while(Tp--) {
        cout<<0<<endl;
        cin>>resh>>resw;
    }
    solve();
    cin>>resh>>resw;
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
