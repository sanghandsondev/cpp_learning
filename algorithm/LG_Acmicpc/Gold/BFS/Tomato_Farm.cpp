
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// https://www.acmicpc.net/problem/7576
// Một số trái cà chua bị ripe (cà chua chín)
// Cứ sau 1 ngày (tương đương 1 step), những trái chưa ripe sẽ bị ripe bởi các trái đã ripe (lây nhiễm)
// Cà chua ko tự ripe
// Tìm số ngày ít nhất để tất cả cà chua đều ripe
// Note: Một số box không có cà chua (wall)

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};
int m, n;
vector<vector<int>> farm;

int main() {
    cin >> m >> n;
    farm = vector<vector<int>> (n, vector<int>(m));
    int area = n * m;   // Tổng số cà chua phải chín
    int count = 0;      // Số cà chua đã chín
    vector<pair<int,int>> ripeTomato;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> farm[i][j];
            if (farm[i][j] == -1) area--;
            else if(farm[i][j] == 1) {
                count++;
                ripeTomato.push_back({i, j});
            }
        }
    }
    if(count == area) {
        cout << 0 << endl;
        return 0;
    }
    // BFS
    int result = -1;
    queue<pair<int,int>> q;
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));    // Sử dụng dist vừa để lưu trữ step, vừa để thay thế visited[i][j] để kiểm tra xem ô (i,j) đã được duyệt hay chưa
    for(auto [x,y] : ripeTomato){
        dist[x][y] = 0;
        q.push({x,y});
    }
    while(!q.empty()){
        auto [x,y] = q.front(); q.pop();
        // no stop point (no target)
        result = max(dist[x][y], result);       // Cập nhật số ngày đã đi qua (số step tối đa)
        for(int k = 0; k < 4; k++){
            int nx = x + dx[k];
            int ny = y + dy[k]; 
            int new_dist = dist[x][y] + 1;
            if(nx >= 0 && nx < n && ny >= 0 && ny < m && farm[nx][ny] == 0){    // Chỉ xét ô có cà chua chưa ripe
                if (dist[nx][ny] == INT_MAX){       // Case 1: Ô chưa được duyệt
                    dist[nx][ny] = new_dist;
                    q.push({nx,ny});
                    count++;                        // Tăng số cà chua đã chín
                } else if (dist[nx][ny] > new_dist){        // Case 2: Ô đã được duyệt nhưng có thể đi qua đường ngắn hơn
                    dist[nx][ny] = new_dist;
                    q.push({nx,ny});
                }
            }
        }
    }
    if (count != area) {
        cout << -1 << endl;
    } else {
        cout << result << endl;
    }  
    return 0;
}