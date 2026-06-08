// https://www.acmicpc.net/problem/20926

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
using pii = pair<int, pair<int,int>>;
const int INF = 1e9;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
int W, H, sx, sy, ex, ey;
char mapz[501][501];

int dijkstra(){
    vector<vector<int>> dist(H, vector<int> (W, INF));
    dist[sx][sy] = 0;
    
    priority_queue<pii, vector<pii>, greater<>> pq;
    pq.push({0, {sx, sy}});
    
    while(!pq.empty()){
        pii tmp = pq.top(); pq.pop();
        int cost = tmp.first;
        int x = tmp.second.first;
        int y = tmp.second.second;
        
        if (x == ex && y == ey) return cost;
        if (cost > dist[x][y]) continue;
        int x1 = x;
        int y1 = y;
        int sumx = 0;
        // up
        while(x1 > 0){
            x1--;
            if (mapz[x1][y1] == 'E') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x1][y1]) {
                    dist[x1][y1] = new_cost;
                    pq.push({new_cost, {x1,y1}});
                }
                break;
            } else if (mapz[x1][y1] == 'H') {
                break;
            } else if (mapz[x1][y1] == 'R') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x1+1][y1]) {
                    dist[x1+1][y1] = new_cost;
                    pq.push({new_cost, {x1 + 1, y1}});
                }
                break;
            } else {
                sumx += mapz[x1][y1] - '0';
            }
        }
        int x2 = x;
        int y2 = y;
        sumx = 0;
        // down
        while(x2 < H-1){
            x2++;
            if (mapz[x2][y2] == 'E') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x2][y2]) {
                    dist[x2][y2] = new_cost;
                    pq.push({new_cost, {x2,y2}});
                }
                break;
            } else if (mapz[x2][y2] == 'H') {
                break;
            } else if (mapz[x2][y2] == 'R') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x2-1][y2]) {
                    dist[x2-1][y2] = new_cost;
                    pq.push({new_cost, {x2 - 1, y2}});
                }
                break;
            } else {
                sumx += mapz[x2][y2] - '0';
            }
        }
        int x3 = x;
        int y3 = y;
        sumx = 0;
        // right
        while(y3 < W-1){
            y3++;
            if (mapz[x3][y3] == 'E') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x3][y3]) {
                    dist[x3][y3] = new_cost;
                    pq.push({new_cost, {x3,y3}});
                }
                break;
            } else if (mapz[x3][y3] == 'H') {
                break;
            } else if (mapz[x3][y3] == 'R') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x3][y3-1]) {
                    dist[x3][y3-1] = new_cost;
                    pq.push({new_cost, {x3, y3-1}});
                }
                break;
            } else {
                sumx += mapz[x3][y3] - '0';
            }
        }
        int x4 = x;
        int y4 = y;
        sumx = 0;
        // left
        while(y4 > 0){
            y4--;
            if (mapz[x4][y4] == 'E') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x4][y4]) {
                    dist[x4][y4] = new_cost;
                    pq.push({new_cost, {x4,y4}});
                }
                break;
            } else if (mapz[x4][y4] == 'H') {
                break;
            } else if (mapz[x4][y4] == 'R') {
                int new_cost = cost + sumx;
                if (new_cost < dist[x4][y4+1]) {
                    dist[x4][y4+1] = new_cost;
                    pq.push({new_cost, {x4, y4+1}});
                }
                break;
            } else {
                sumx += mapz[x4][y4] - '0';
            }
        }
    }
    return -1;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> W >> H;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cin >> mapz[i][j];
            if (mapz[i][j] == 'T') {
                sx = i; sy = j;
            } else if (mapz[i][j] == 'E') {
                ex = i; ey = j;
            }
        }
    }
    int min_cost = dijkstra();
    cout << min_cost;
    return 0;
}