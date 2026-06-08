#include <bits/stdc++.h>
using namespace std;

// Bài toán 2: Kiểm tra có tồn tại đường đi giữa 2 điểm trên lưới hay không( o là có đường đi, x là không có đường đi )  -> DFS / BFS
// ---- Input -----
// 6 6
// Aooxoo
// oxoooo
// oxoooo
// ooooxx
// Boooxo
// oooxxx
// ------- Output: 

int n, m, x1, t1;      // n hàng, m cột, điểm bắt đầu A(x1,y1), điểm kết thúc B(x2,y2);
char a[1001][1001];
bool res;

int dx[4] = {-1,0,0,1};
int dy[4] = {0,-1,1,0};

void inp(){
    cin >> n >> m;
    for (int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++) {
            cin >> a[i][j];
            if (a[i][j] == 'A') {
                x1 = i; 
                t1 = j;
            }
        }
    }
}

// ------------------------------------------------ DFS -----------------------------------------------
// void dfs(int i, int j){
//     a[i][j] = 'x';
//     for(int k = 0; k < 4; k++){
//         int i1 = i + dx[k];
//         int j1 = j + dy[k];
//         if (i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i1][j1] != 'x'){
//             if(a[i1][j1] == 'B'){
//                 res = true;
//                 return;
//             }
//             dfs(i1,j1);
//         }
//     };
// }
// ------------------------------------------------ BFS -----------------------------------------------
void bfs(int i, int j){
    queue<pair<int, int>> q;
    q.push({i,j});
    a[i][j] = 'x';
    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        for(int k = 0; k < 4; k++){
            int i1 = top.first + dx[k];
            int j1 = top.second + dy[k];
            if (i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i1][j1] != 'x'){
                if(a[i1][j1] == 'B'){
                    res = true;
                    return;
                }
                q.push({i1, j1});
                a[i1][j1] = 'x';
            }
        }
    }
}

void solve(){
    // dfs(x1,t1);     // Xuất phát từ A
    bfs(x1,t1);     // Xuất phát từ A
    if (res) {
        cout << "Ton tai duong di tu A den B\n";
    } else{
        cout << "Khong ton tai duong di tu A den B\n";
    }
}

int main(){
    inp();
    solve();
    return 0;
}