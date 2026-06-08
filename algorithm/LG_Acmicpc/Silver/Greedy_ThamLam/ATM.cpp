// https://www.acmicpc.net/problem/11399

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// THAM LAM: Ưu tiên thằng waste ít time nhất đứng trước thì tổng thời gian đợi của N thằng sẽ là nhỏ nhất

int n;
multiset<int> se;   

void solve(){
    cin >> n;
    for(int i = 1; i <= n; i++){
        int x; cin >> x;
        se.insert(x);
    }
    ull sum = 0;
    int wait = 0;
    for (auto x : se) {
        sum += x + wait;
        wait += x;
    }
    cout << sum;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}