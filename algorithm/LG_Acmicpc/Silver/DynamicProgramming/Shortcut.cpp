// https://www.acmicpc.net/problem/1446

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int N, D;
vector<pair<pair<int, int>, int>> shortcuts;


void solve(){
    cin >> N >> D;
    for (int i = 0; i < N; i++){
        int from, to, cost;
        cin >> from >> to >> cost;
        if (to > D) continue;       // Khong duoc di qua' D
        if (to - from <= cost) continue;    // Shortcut khong toi uu hon duong` thuong`
        shortcuts.push_back({{from,to},cost});
    }
    vector<int> dp(D+1, INT_MAX);
    dp[0] = 0;
    
    for(int i = 0; i <= D; i++){
        if (i > 0) {
            dp[i] = min(dp[i], dp[i-1] + 1);    // Di thuong` tu` i-1 den i
        }
        for (auto& shortcut : shortcuts) {
            int from = shortcut.first.first;
            int to = shortcut.first.second;
            int cost = shortcut.second;

            if (from == i && to <= D) {
                dp[to] = min(dp[to], dp[i] + cost);
            }
        }
    }
    cout << dp[D] << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}