#include <bits/stdc++.h>
using namespace std;

// Bài toán 1: Đếm số thành phần liên thông trên lưới (Flood_Fill) -> DFS / BFS
// ---- Input -----
// 6 6
// xxxooo
// ooxooo
// oxxxxo
// ooooox
// xxoxox
// xoxxox
// ------- Output: 4

int n, m;       // n hàng, m cột
char a[1001][1001];
bool visited[1001][1001];

int dx[4] = {-1,0,0,1};
int dy[4] = {0,-1,1,0};

void inp(){
    cin >> n >> m;
    for (int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++) {
            cin >> a[i][j];
        }
    }
    memset(visited, false, sizeof(visited));
}
// ------------------------------------------------ DFS -----------------------------------------------
void dfs(int i, int j){
    visited[i][j] = true;
    for(int k = 0; k < 4; k++){
        int i1 = i + dx[k];
        int j1 = j + dy[k];
        if (i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i1][j1] == 'x' && !visited[i1][j1]){
            dfs(i1,j1);
        }
    }
}

void solveByDFS(){
    int cnt = 0;
    for (int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++) {
            if (a[i][j]=='x' && !visited[i][j]){
                dfs(i,j);
                cnt++;
            }
        }
    }
    cout << "So thanh phan lien thong tren luoi: " << cnt << endl;
}
// ------------------------------------------------ BFS -----------------------------------------------
void bfs(int i, int j){
    queue<pair<int, int>> q;
    q.push({i,j});
    a[i][j] = 'o';      // Duyệt qua cái nào thì gán điểm đó = 'o' luôn, đỡ phải tạo ra một mảng visited[][]
    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        for(int k = 0; k < 4; k++){
            int i1 = top.first + dx[k];
            int j1 = top.second + dy[k];
            if (i1 >= 1 && i1 <= n && j1 >= 1 && j1 <= m && a[i1][j1] == 'x'){
                q.push({i1, j1});
                a[i1][j1] = 'o';
            }
        }
    }
}

void solveByBFS(){
    int cnt = 0;
    for (int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++) {
            if (a[i][j]=='x'){
                bfs(i,j);
                cnt++;
            }
        }
    }
    cout << "So thanh phan lien thong tren luoi: " << cnt << endl;
}

int main(){
    inp();
    solveByDFS();
    solveByBFS();
    return 0;
}