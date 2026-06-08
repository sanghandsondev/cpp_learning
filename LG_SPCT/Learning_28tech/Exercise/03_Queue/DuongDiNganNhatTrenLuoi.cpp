#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// Bài toán: Cho ma trận A[n][m] chỉ bao gồm các số 0 và 1. Hãy tìm đường đi ngắn nhất từ một phần tử bắt đầu đến phần tử đích.
//           Biết mỗi bước đi ta chỉ được phép dịch chuyển từ phần tử có giá trị 1 đến phần tư có giá trị là 1
//           Cho phép đi 4 hướng (trên, dưới, trái, phải)

// Note: Nhìn là biết dùng BFS rồi -> sử dụng queue để duyệt
// 9 10 0 0 3 4
// 1 0 1 1 1 1 0 1 1 1
// 1 0 1 0 1 1 1 0 1 1
// 1 1 1 0 1 1 0 1 0 1
// 0 0 0 0 1 0 0 0 0 1
// 1 1 1 0 1 1 1 0 1 0
// 1 0 1 1 1 1 0 1 0 0
// 1 0 0 0 0 0 0 0 0 1
// 1 0 1 1 1 1 0 1 1 1 
// 1 1 0 0 0 0 1 0 0 1

int a[1000][1000];
int d[1000][1000] = {0};  // d[i][j]: Lưu số bước đi tối thiểu từ ô start đến ô hiện tại
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};
pair<int, int> s, e;    // start(i,j), end(i,j)
int n, m;

void input(){
    cin >> n;
    cin >> m;
    cin >> s.first >> s.second;     // Điểm start(i, j)
    cin >> e.first >> e.second;     // Điểm end(i, j)
    for (int r = 0; r < n; r++){
        for (int  c = 0; c < m; c++){
            cin >> a[r][c];
        }
    }
}

int solve(){
    queue<pair<int, int>> q;
    // handle cho ô start
    q.push({s.first, s.second});
    d[s.first][s.second] = 0;
    a[s.first][s.second] = 0;

    while (!q.empty()){
        pair<int, int> top = q.front();
        q.pop();
        
        int i = top.first;
        int j = top.second;
        for (int k = 0; k < 4; k++){
            int i1 = i + dx[k];
            int j1 = j + dy[k];
            if (a[i1][j1] == 1 && i1 >= 0 && i1 < n && j1 >= 0 && j1 < m) {
                // Nếu ô tiếp theo đó chính là điểm "end" thì return ra số bước đi hiện tại + 1
                if (i1 == e.first && j1 == e.second) {
                    return d[i][j] + 1;
                }
                // Push điểm tiếp theo vào queue
                q.push({i1, j1});
                d[i1][j1] = d[i][j] + 1;
                a[i1][j1] = 0;
            }
        }
    }
    return -1;

}

int main(){
    input();
    if (a[s.first][s.second] && a[e.first][e.second]){
        cout << "Shortest step: " << solve() << endl;
    } else {
        cout << "Shortest step: -1" << endl;
    }
    return 0;
}