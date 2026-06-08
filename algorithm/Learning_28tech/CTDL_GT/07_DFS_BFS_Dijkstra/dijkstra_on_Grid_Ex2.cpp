// BÀI TOÁN: Tìm đường đi có số step là nhỏ nhất từ điểm start -> end trên ma trận (di chuyển 4 hướng)
// TRƯỜNG HỢP nhiều case có step nhỏ nhất = nhau thì output ra đường đi có Chi phí (Power) là lớn nhất
// 3 3
// 0 0 2 2
// 1 3 1
// 1 5 1
// 4 2 1
#include <bits/stdc++.h>
using namespace std;

struct State {
    int x, y, dist, power;
    bool operator<(const State& other) const {
        if (dist == other.dist) return power < other.power; // max power if same steps
        return dist > other.dist; // min steps
    }
};

int N, M;
int sx, sy, ex, ey;
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

int dijkstra(){
    vector<vector<int>> grid(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> grid[i][j];
        }
    }
    // Case Start point or End point is not exist
    if (grid[sx][sy] == -1 || grid[ex][ey] == -1) {
        return -1;
    }

    vector<vector<int>> min_dist(N, vector<int>(M, INT_MAX));
    vector<vector<int>> max_power(N, vector<int>(M, 0));

    priority_queue<State> pq;
    pq.push({sx, sy, 0, grid[sx][sy]}); // Push Start point with 0 step and its own power
    min_dist[sx][sy] = 0;
    max_power[sx][sy] = grid[sx][sy];

    while (!pq.empty()) {
        State cur = pq.top(); pq.pop();
        // End point
        if (cur.x == ex && cur.y == ey) {
            return cur.power;
        }

        // Move 4 direc
        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];
            if (nx >= 0 && ny >= 0 && nx < N && ny < M && grid[nx][ny] != -1) {
                int new_dist = cur.dist + 1;
                int new_power = cur.power + grid[nx][ny];
                // Condition to push, same to [visited == false] in easy problem
                if (new_dist < min_dist[nx][ny] || (new_dist == min_dist[nx][ny] && new_power > max_power[nx][ny])) {
                    min_dist[nx][ny] = new_dist;
                    max_power[nx][ny] = new_power;
                    pq.push({nx, ny, new_dist, new_power});
                }
            }
        }
    }

    return -1;
}

int main() {
    cin >> N >> M;
    cin >> sx >> sy >> ex >> ey;
    cout << "Max power with min steps: " << dijkstra();
    return 0;
}
