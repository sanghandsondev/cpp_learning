#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// ----- Bài toán: Cho ma trận [N][M], tìm đường đi với tổng nhỏ nhất từ ô (1,1) đến ô (x,y). 
// Bạn chỉ được di chuyển xuống dưới / sang phải

// ----- Solution(DP-Prefix Sum): 
// Với bài toán các hướng đi là cố định thì không cần xài tới BFS làm gì cho phức tạp

// Sử dụng một mảng dp[i][j] để lưu tổng nhỏ nhất từ ô (1,1) đến ô (i,j)
// dp[i][j] = matrix[i][j] + min(dp[i-1][j], dp[i][j-1]);   ----> 1 dạng kiểu Prefix Sum
// dp[i-1][j]: Tổng nhỏ nhất từ ô phía trên.
// dp[i][j-1]: Tổng nhỏ nhất từ ô bên trái.

int n, m;
int x, y;
int a[100][100];
int dp[100][100];

void solve(){
    cin >> n >> m >> x >> y;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> a[i][j];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if (i == 0 && j == 0) dp[i][j] = a[i][j];
            else if (j == 0) {
                dp[i][j] = dp[i - 1][j] + a[i][j];
            } else if (i == 0) {
                dp[i][j] = dp[i][j - 1] + a[i][j];
            } else {
                dp[i][j] = a[i][j] + min(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    cout << dp[x-1][y-1] << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}