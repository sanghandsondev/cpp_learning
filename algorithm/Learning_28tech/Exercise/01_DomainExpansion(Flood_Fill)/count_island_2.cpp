#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận A[N][M] bao gồm các số 0 và 1. Ta gọi mỗi miền của ma trận là nhóm các số 1 được bao quanh bởi các số 0. 
// Hãy tìm số miền của ma trận đó.
// (( Giống bài toán count_island_1 thôi cơ mà đây sẽ xét theo 8 hướng chứ không phải 4 hướng))
// ----- Input ------
// 1 5 5
// 1 1 0 0 0
// 0 1 0 0 1
// 1 0 0 1 1
// 0 0 0 0 0
// 1 0 1 1 0
// ----> Output : 4

int tc, n, m;
int a[100][100];
int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int cnt;

void dfs(int r, int c){
    a[r][c] = 0;        // STATUS
    for(int k = 0; k < 8; k++){
        int r1 = r + dx[k];
        int c1 = c + dy[k];
        if (r1 >= 0 && r1 < n && c1 >= 0 && c1 <= m && a[r1][c1] == 1) {
            dfs(r1,c1);
        }
    }
}

void bfs(int r, int c){
    queue<pair<int, int>> q;
    q.push({r,c});
    a[r][c] = 0;        // STATUS INIT
    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        for(int k = 0; k < 8; k++){
            int r1 = top.first + dx[k];
            int c1 = top.second + dy[k];
            if (r1 >= 0 && r1 < n && c1 >= 0 && c1 < m && a[r1][c1] == 1){
                q.push({r1, c1});
                a[r1][c1] = 0;      // STATUS QUEUE
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> tc;
    while(tc--){
        cin >> n >> m;
        for(int r = 0; r < n; r++){
            for(int c = 0; c < m; c++){
                cin >> a[r][c];
            }
        }
        cnt = 0;
        // Duyệt hết các phần tử trong mảng
        for(int r = 0; r < n; r++){
            for(int c = 0; c < m; c++){
                // Bắt gặp 1 miền mới
                if(a[r][c] == 1) {
                    cnt++;
                    // dfs(r,c);
                    bfs(r,c);
                }
            }
        }
        cout << "So mien cua ma tran: " << cnt;
    }
    
    return 0;
}