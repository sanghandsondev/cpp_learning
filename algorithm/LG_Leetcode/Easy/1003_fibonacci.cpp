// https://www.acmicpc.net/problem/1003

// -----NOTE: Quy hoạch động + Mảng Cộng dồn có nhớ (Prefix sum) --------
// [0,        1,        2,        3,         4,      ....     40]
// [<x0, y0>, <x1, y1>, <x2, y2>, <x3, y3>, <x4, y4>          <x40, y40>]
// trong đó: x2 = x0 + x1; y2 = y0 + y1; 
//           x3 = x1 + x2; y3 = y1 + y2;
//      xi là số lần chữ số 0 xuất hiện của số "i"
//      yi là số lần chữ số 1 xuất hiện của số "i"

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int N;
vector<pair<int, int>> v(41, {0,0});   // {0,0} {0,0} ... 41 lần

void init(){
    v[0].first = 1;
    v[0].second = 0;
    v[1].first = 0;
    v[1].second = 1;
    for (int i = 2; i <= 40; i++){
        v[i].first = v[i-2].first + v[i-1].first;
        v[i].second = v[i-2].second + v[i-1].second;
    }
}

void solve(){
    cin >> N;
    cout << v[N].first << ' ' << v[N].second << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    init();
    int t; cin >> t;
    while(t--) solve();
    return 0;
}