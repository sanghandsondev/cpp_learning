// https://www.acmicpc.net/problem/7562

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, sx, sy, ex, ey;
int a[301][301];

int dx[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
int dy[8] = {-2, -1, 1, 2, 2, 1, -1, -2};

int bfs(int x, int y){
    queue<pair<int,int>> q;
    q.push({x,y});
    int step[301][301] = {0};
    memset(a, 0, sizeof(a));
    step[x][y] = 0;
    a[x][y] = 1;
    while(!q.empty()){
        pair<int,int> point = q.front();
        q.pop();
        int i = point.first;
        int j = point.second;
        for(int k = 0; k < 8; k++) {
            int i1 = i + dx[k];
            int j1 = j + dy[k];
            if (i1 >= 0 && i1 < n && j1 >= 0 && j1 < n && a[i1][j1] == 0) {
                if (i1 == ex && j1 == ey) {
                    return step[i][j] + 1;
                } else {
                    q.push({i1,j1});
                    a[i1][j1] = 1;
                    step[i1][j1] = step[i][j] + 1;
                }
            }
        }
    }
    return 0;
}

void solve(){
    cin >> n;
    cin >> sx >> sy >> ex >> ey;
    cout << bfs(sx, sy) << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int t; cin >> t;
    while(t--){
        solve();
    }
    return 0;
}