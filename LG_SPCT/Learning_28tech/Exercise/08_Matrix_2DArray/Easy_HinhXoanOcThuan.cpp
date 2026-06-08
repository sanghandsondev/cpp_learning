#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận A[N][M] và nhiệm vụ của bạn là in các phần tử của ma trận theo hình xoắn ốc
// 1 2 3
// 4 5 6
// 7 8 9
// ---> 1 2 3 6 9 8 7 4 5
int n, m;
int a[500][500];

void solve(){
    // Input
    cin >> n >>  m;
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) cin >> a[i][j];
    }
    // Handle
    int h1 = 0, h2 = n - 1, c1 = 0, c2 = m - 1;
    while(h1 <= h2 && c1 <= c2){
        // Dong dau` tien
        for(int i = c1; i <= c2; i++){
            cout << a[h1][i] << " ";
        }
        h1++;
        // Cot ben phai
        for(int i = h1; i <= h2; i++){
            cout << a[i][c2] << " ";
        }
        c2--;
        // Hang cuoi cung
        if (h1 <= h2) {
            for(int i = c2; i >= c1; i--){
                cout << a[h2][i] << " ";
            }
            h2--;
        }
        if (c1 <= c2) {
            for (int i = h2; i >= h1; i--){
                cout << a[i][c1] << " ";
            }
            c1++;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}