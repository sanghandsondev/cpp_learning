#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận A[N][M] có các phần tử là "O" hoặc "X". Hãy thay thế các miền O được bao quanh bởi "X" (theo 4 hướng)
// --- Input
// 5 4
// X X X X
// X O X X
// X O O X
// X O X X
// X X O O
// ---- Output: X X X X
             // X X X X
             // X X X X
             // X X O O

// Tip: Các "O" chắc chắn không bị thay thế sẽ nằm ở BIÊN của ma trận và ngược lại, 
// => Nên những miền O nào có một phần tử thuộc biên thì chúng sẽ không bị thay thế
// ./ThayTheX-O.png

int n, m;
char a[100][100];
pair<int,int> path[4] = {{-1,0},{0,-1},{0,1},{1,0}};

void dfs(int i, int j){
    a[i][j] = '*';
    for(int k = 0; k < 4; k++){
        int i1 = i + path[k].first;
        int j1 = j + path[k].second;
        if(i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i][j] == 'O'){
            dfs(i1, j1);
        } 
    }
}

void solve(){
    cin >> n >> m;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            cin >> a[i][j];
        }
    }
    // 1. Duyệt các miền O ở biên
    //    1.1. Duyệt cột đầu tiên và cột cuối cùng
    for(int r = 1; r <= n; r++){
        if(a[r][1] == 'O') dfs(r, 1);
        if(a[r][m] == 'O') dfs(r, m);
    }
    //    1.2. Duyệt hàng đầu tiên và hàng cuối cùng
    for(int c = 1; c <= m; c++){
        if(a[1][c] == 'O') dfs(1, c);
        if(a[n][c] == 'O') dfs(n, c);
    }
    // 2. Thay thế các ký tự 'O' thành 'X' do bị bao bọc với X. Và thay thế ký tự '*' về 'O' do là miền biên.
    cout << "Ma tran sau khi thay the X - O: \n";
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            if (a[i][j] != '*') {
                cout << "X ";
            } else {
                cout << "O ";
            }
        }
        cout << "\n";
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}