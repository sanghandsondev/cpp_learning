// BÀI TOÁN: Tìm đường đi có tổng chi phí nhỏ nhất từ điểm start -> end trên ma trận (di chuyển 4 hướng)
// 3 3
// 0 0 2 2
// 1 3 1
// 1 5 1
// 4 2 1

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

using pii = pair<int, pair<int,int>>;  // {current_cost, {point_x, point_y}}
int dx[4] = {-1, 1, 0, 0}; 
int dy[4] = {0, 0, -1, 1}; 

int n, m, sx, sy, ex, ey;
int cost[10][10];
int dist[10][10];


int dijkstra(){
    memset(dist, INT_MAX, sizeof(dist));
    priority_queue<pii, vector<pii>, greater<>> pq;     // Ưu tiên current_cost nhỏ nhất

    dist[sx][sy] = cost[sx][sy];
    pq.push({dist[sx][sy], {sx, sy}});

    while (!pq.empty())
    {
        pii tp = pq.top();
        pq.pop();
        int cur_cost = tp.first;
        int x = tp.second.first;
        int y = tp.second.second;
        
        if (x == ex && y == ey) return cur_cost;

        if (cur_cost > dist[x][y]) continue;        // important

        for(int i = 0; i < 4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && ny >= 0 && nx < n && ny < m) {
                int new_cost = cur_cost + cost[nx][ny];
                // --- Nếu cost mới nhỏ hơn cost hiện tại của {nx,ny} thì sẽ push ---
                if (new_cost < dist[nx][ny]) {
                    dist[nx][ny] = new_cost;
                    pq.push({new_cost,{nx,ny}});
                }
            }
        }
    }
    return -1;
    
}

int main(){
    cin >> n >> m >> sx >> sy >> ex >> ey;
    for(int i = 0 ; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> cost[i][j];
        }
    }
    int min_cost = dijkstra();
    cout << "Min Cost: " << min_cost << endl;
    return 0;
}