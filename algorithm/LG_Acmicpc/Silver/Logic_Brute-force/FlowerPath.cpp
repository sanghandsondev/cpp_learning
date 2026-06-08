// https://www.acmicpc.net/problem/14620

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int dx[5] = {-1, 0, 0, 0, 1};
int dy[5] = {0, -1, 0, 1, 0};

int n;
int a[11][11];
int dp[11][11];
bool check[11][11];

void solve(){
    cin >> n;
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            cin >> a[i][j];
        }
    }
    memset(check, true, sizeof(check));
    memset(dp, 0, sizeof(dp));
    for(int i = 2; i <= n-1; i++) {
        for (int j = 2; j <= n-1; j++) {
            dp[i][j] =  a[i][j] + a[i-1][j] + a[i][j-1] + a[i][j+1] + a[i+1][j];  
        }
    }
    int res = INT_MAX;
    // point 1
    for(int i1 = 2; i1 <= n-1; i1++) {
        for (int j1 = 2; j1 <= n-1; j1++) {
            // point 2
            for(int i2 = 2; i2 <= n-1; i2++) {
                for (int j2 = 2; j2 <= n-1; j2++) {
                    // point 3
                    for(int i3 = 2; i3 <= n-1; i3++) {
                        for (int j3 = 2; j3 <= n-1; j3++) {
                            // check 3 point can plant or not
                            bool check[11][11] = {false};
                            for(int k = 0; k < 5; k++){
                                check[i1+dx[k]][j1+dy[k]] = true;
                                check[i2+dx[k]][j2+dy[k]] = true;
                                check[i3+dx[k]][j3+dy[k]] = true;
                            }
                            int cnt = 0;
                            for(int r = 1; r <= n; r++){
                                for(int c = 1; c <= n; c++){
                                    if (check[r][c]) cnt++;
                                }
                            }
                            // if can plant, calculate sum
                            if(cnt == 15) {
                                int sum = dp[i1][j1] + dp[i2][j2] + dp[i3][j3];
                                res = min(sum, res);
                                // cout << i1 << " " << j1 << " " << i2 << " " << j2 << " " << i3 << " " << j3 << " " << sum << " " << res << endl;
                            }
                        }
                    }  
                }    
            }
        }
    }
    cout << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}