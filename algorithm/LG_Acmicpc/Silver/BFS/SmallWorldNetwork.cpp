// https://www.acmicpc.net/problem/18243

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, k;  
vector<int> adj[1001];
bool visited[1001];

void input(){
    cin >> n >> k;
    for(int i = 1; i <= k; i++){
        int x,y; cin >> x >>  y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
}

// Xét số step từ tất cả các people khác đến people i 
bool bfs(int i){
    int step[101] = {0};                  
    memset(visited, false, sizeof(visited));    
    queue<int> q;
    q.push(i);
    visited[i] = true;
    step[i] = 0;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int x : adj[v]){
            if(!visited[x]){
                q.push(x);
                visited[x] = true;
                step[x] = step[v] + 1;  // Lưu step của people mới mỗi khi push
            }
        }
    }
    for(int k = 1; k <= n; k++){    
        if (k != i) {
            if (step[k] == 0 || step[k] > 6) return false;      // Xét step từ people k đến people i
        } 
    }
    return true;
}

bool solve(){
    for(int i = 1; i <= n; i++){
        if (!bfs(i)) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    input();
    if (solve()) {
        cout << "Small World!";
    } else {
        cout << "Big World!";
    }
    return 0;
}