// https://www.acmicpc.net/problem/1149

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n;
int cost[1001][3];

void solve(){
    cin >> n;
    for(int i = 1; i <= n; i++){
        for(int j = 0; j < 3; j++) {
            cin >> cost[i][j];
        }
    }
    vector<vector<int>> dp(n+1, vector<int>(3, INT_MAX));
    // Init first house
    for (int j = 0; j < 3; j++) {
        dp[1][j] = cost[1][j];
    }
    // From house 2 -> N
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j < 3; j++){
            for(int prev = 0; prev < 3; prev++) {
                if (j != prev) {
                    dp[i][j] = min(dp[i][j], dp[i-1][prev] + cost[i][j]);
                }
            }
        }
    }
    int result = min({dp[n][0], dp[n][1], dp[n][2]});
    cout << result;          
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}