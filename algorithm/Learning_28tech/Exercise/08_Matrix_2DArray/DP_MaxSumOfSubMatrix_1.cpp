#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// BÀI TOÁN: Giả sử giá trị của một ma trận là hiệu giữa tổng các số trên đường chéo chính và tổng các số trên đường chéo phụ
// Cho ma trận A [N][N], hãy tìm ma trận con của A sao cho ma trận con đó có giá trị lớn nhất (tất nhiên ma trận con là vuông)
// --- Input 
// 4
// 9 -2 -8 0
// 6 -2 0 -9
// 4 -5 6 1
// 1 3 4 9
// --- Output: giá trị của ma trận con cần tìm

// SOLUTION: Khả năng sẽ vẫn xài PREFIX SUM mà thôi
// Tạo 2 ma trận lưu prefix sum cho đường chéo chính và đường chéo phụ
// Ví dụ ma trận 1 với ô (i,j) thì nó sẽ xác định được tổng các phần tử trên đường chéo chính từ hàng / cột đầu tiên đến ô (i,j) đó

int n;
int a[100][100];

void solve(){
    cin >> n;
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }
    ull dcchinh[100][100] = {0};
    ull dcphu[100][100] = {0};
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) {
            if (i == 0 || j == 0) {
                dcchinh[i][j] = a[i][j];
            } else {
                dcchinh[i][j] = a[i][j] + dcchinh[i-1][j-1];    // Prefix sum - DP
            }
        }
    }
    for (int i = 0; i < n; i++){
        for(int j = n-1; j >= 0; j--) {
            if (i == 0 || j == n-1) {
                dcphu[i][j] = a[i][j];
            } else {
                dcphu[i][j] = a[i][j] + dcphu[i-1][j+1];    // Prefix sum - DP
            }
        }
    }

    // Handle sau khi tạo thành công 2 ma trận prefix sum
    ull ans = -1e18;
    for(int i = n-1; i >= 0; i--){
        for(int j = n-1; j >= 0; j--){
            int temp = min(j,i);    // Kích thước tối đa của ma trận con được tạo ra từ ô (i,j)
            int hang = i - temp, cot = j - temp;
            for (int k = 0; k < temp; k++){
                ans = max(ans, 0ll + dcchinh[i][j] - dcchinh[hang + k][cot + k] - 
                    (dcphu[i][cot + k + 1] - dcphu[hang + k][j + 1]));   
            }
        }
    }
    cout << ans;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}