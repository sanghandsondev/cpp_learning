#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// In ra ma trận thay thế với mỗi phần tử trong mảng sẽ được thay thế bằng tổng của các phần tử xung quanh nó
// 1 2 3                11 19 13      (11 = 2 + 4 + 5)
// 4 5 6       -->      23 40 27        
// 7 8 9                17 31 19

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int n;
int a[500][500];
ull res[500][500];

void solve(){
    // -- Input
    cin >> n ;
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) cin >> a[i][j];
    }
    
    // Duyệt qua toàn bộ phần tử để thay thế
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            // (i,j)
            int sum = 0;
            for(int k = 0; k < 8; k++){
                int i1 = i + dx[k];
                int j1 = j + dy[k];
                if (i1 >= 0 && i1 < n && j1 >= 0 && j1 < n) sum+= a[i1][j1];
            }
            res[i][j] = sum;
        }
    }

    // -- Output
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            cout << res[i][j] << " ";
        }
        cout << "\n";
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}