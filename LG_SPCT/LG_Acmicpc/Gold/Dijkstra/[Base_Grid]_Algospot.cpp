// https://www.acmicpc.net/problem/1261

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
const int INF = 1e9;
using pii = pair<int, pair<int,int>>;   // {cost, {x,y}}

// Bai toan co trong so dac biet:
// Di qua o 0 : khong ton gi
// Di qua o 1: phai pha wall -> ton 1 don vi chi phi

int N, M;   // height M, width M 
int maze[101][101];
// int dist[101][101];      // method 1
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int dijkstra(int sx, int sy, int ex, int ey){
    vector<vector<int>> dist(N, vector<int>(M, INF));   // method 2
    
    priority_queue<pii, vector<pii>, greater<pii>> pq;   // uu tien chi phi nho nhat
    dist[sx][sy] = 0;
    pq.push({dist[sx][sy], {sx, sy}});    // {cost, {x,y}}
    
    while(!pq.empty()){
        pii tmp = pq.top(); pq.pop();
        int cur_cost = tmp.first;
        // cout << cur_cost << endl;
        int x = tmp.second.first;
        int y = tmp.second.second;
        
        if (x == ex && y == ey) return cur_cost;
    
        if (cur_cost > dist[x][y]) continue;
        
        for(int k = 0; k < 4; k++){
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && ny >= 0 && nx < N && ny < M) {
                int new_cost = cur_cost + maze[nx][ny];
                if (new_cost < dist[nx][ny]){
                    dist[nx][ny] = new_cost;
                    pq.push({new_cost, {nx, ny}});
                }
            }
        }
    }
    return dist[ex][ey];
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> M >> N;
    for(int i = 0; i < N; i++){
        string row; cin >> row;
        for(int j = 0; j < M; j++){
            maze[i][j] = row[j] - '0';
        }
    }
    // memset(dist, INF, sizeof(dist));     // method 1
    int min_cost = dijkstra(0, 0, N-1, M-1);
    cout << min_cost;
    return 0;
}