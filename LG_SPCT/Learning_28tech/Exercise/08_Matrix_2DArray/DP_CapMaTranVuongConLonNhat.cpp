#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận A[N][M] chỉ bao gồm 0 vs 1. Tìm cấp MA TRẬN VUÔNG CON lớn nhất có các phần tử đều bằng 1
// --- Input
// 1 6 5
// 0 1 1 0 1
// 1 1 0 1 0
// 0 1 1 1 0
// 1 1 1 1 0
// 1 1 1 1 1
// 0 0 0 0 0
// ---> Output: 3
// --- dp tương ứng ---
// 0 1 1 0 1
// 1 1 0 1 0
// 0 1 1 1 0
// 1 1 2 2 0
// 1 2 2 3 1
// 0 0 0 0 0

int tc, n, m;
int a[100][100];
int dp[100][100];  // Lưu cấp ma trận vuông con lớn nhất mà mỗi phần tử chính là BOTTOM RIGHT point của ma trận vuông con đó.
                   // Lại một dạng nữa của DP (Dynamic Programming) hay cũng có thể xem là một dạng kiểu kiểu Prefix sum
int res;

void solve(){
    cin >> n >> m;
    for (int r = 0; r < n; r++){
        for (int  c = 0; c < m; c++){
            cin >> a[r][c];
        }
    }
    // Cột và hàng đầu tiên thì thằng nào = 1 thì dp tương ứng sẽ = 1, = 0 thì dp tương ứng chắc chắn = 0 rồi
    for(int c = 0; c < m; c++){
        dp[0][c] = a[0][c];
    }
    for(int r = 0; r < n; r++){
        dp[r][0] = a[r][0];
    }
    // Handle các phần tử còn lại cho mảng dp
    for (int r = 1; r < n; r++){
        for (int  c = 1; c < m; c++){
            if (a[r][c] == 1){
                // -- Để xác định công thức DP này thì cần vẽ ma trận ra 
                // (vẽ các ma trận con liên quan trước đó để tìm ra mối quan hệ --> nâng cao hơn prefix sum thông thường)
                dp[r][c] = min(dp[r-1][c-1], min(dp[r-1][c], dp[r][c-1])) + 1;
            } else {
                dp[r][c] = 0;
            }
        }
    }
    int res = 0;
    for (int r = 0; r < n; r++){
        for (int  c = 0; c < m; c++){
            res = max(res, dp[r][c]);
        }
    }
    cout << "Cap ma tran vuong con lon nhat: " << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}