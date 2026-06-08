#include <bits/stdc++.h>
using namespace std;

// Bài toán 3 : Tìm số bước đi tối thiểu giữa 2 điểm trên lưới A -> B( o là có đường đi, x là không có đường đi )  ->  BFS
// ---- Input -----
// 6 6
// Aooxoo
// oxoooo
// oxoooo
// ooooxx
// Boooxo
// oooxxx
// ------- Output: 4

int n, m, x1, t1, u, v;      // n hàng, m cột, điểm bắt đầu A(x1,y1), điểm kết thúc B(x2,y2);
char a[1001][1001];
char d[1001][1001];     // d[i][j] lưu số bước đi từ điểm A tới ô (i,j)

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
            } else if(a[i][j] == 'B') {
                u = i;
                v = j;
            }
        }
    }
}
// ------------------------------------------------ BFS -----------------------------------------------
int bfs(){
    queue<pair<int, int>> q;
    q.push({x1,t1});
    a[x1][t1] = 'x';
    d[x1][t1] = 0;
    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        for(int k = 0; k < 4; k++){
            int i1 = top.first + dx[k];
            int j1 = top.second + dy[k];
            if (i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i1][j1] != 'x'){
                if(a[i1][j1] == 'B'){
                    return d[top.first][top.second] + 1;
                }
                q.push({i1, j1});
                a[i1][j1] = 'x';
                d[i1][j1] = d[top.first][top.second] + 1;    // bước đi từ ô (i1,j1) tới điểm A = bước đi từ ô (i,j) + 1
            }
        }
    }
    return -1;
}

void solve(){
    cout << "Shortest step: " << bfs() << endl;
}

int main(){
    inp();
    solve();
    return 0;
}