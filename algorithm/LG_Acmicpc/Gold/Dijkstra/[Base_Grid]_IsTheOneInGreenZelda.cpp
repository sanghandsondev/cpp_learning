// https://www.acmicpc.net/problem/4485

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
using pii = pair<int, pair<int,int>>;
const int INF = 1e9;

int N;
int cave[126][126];
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int dijkstra(int sx, int sy, int ex, int ey){
    vector<vector<int>> dist(N, vector<int> (N, INF));
    dist[sx][sy] = cave[sx][sy];
    
    priority_queue<pii, vector<pii>, greater<>> pq;
    pq.push({dist[sx][sy], {sx, sy}});
    
    while(!pq.empty()){
        pii tmp = pq.top(); pq.pop();
        int cost = tmp.first;
        int x = tmp.second.first;
        int y = tmp.second.second;
        if (x == ex && y == ey) return cost;
        if (cost > dist[x][y]) continue;
        for(int k = 0; k < 4; k++){
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                int new_cost = cost + cave[nx][ny];
                if (new_cost < dist[nx][ny]){
                    dist[nx][ny] = new_cost;
                    pq.push({new_cost, {nx,ny}});
                }
            }
        }  
    }
    return 0;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int t = 0;
    while(1){
        cin >> N;
        if (N == 0) break;
        t++;
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                cin >> cave[i][j];
            }
        }
        int min_cost = dijkstra(0, 0, N-1, N-1);
        cout << "Problem " << t << ": " << min_cost << endl;
    }
    return 0;
}