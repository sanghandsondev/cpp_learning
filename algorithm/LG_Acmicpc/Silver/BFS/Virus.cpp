// https://www.acmicpc.net/problem/2606

// Bài toán này là FloodFill nên hoàn toàn có thể sử dụng DFS

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007
 
int n, k;
vector<int> adj[1001];
bool visited[101];

void input(){
    cin >> n;
    cin >> k;
    for(int i = 0; i < k; i++){
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
}

void bfs(int i){
    queue<int> q;
    q.push(i);
    memset(visited, false, sizeof(visited)); 
    visited[i] = true;
    while(!q.empty()){
        int v = q.front();
        q.pop();
        for (int x : adj[v]){
            if (!visited[x]) {
                q.push(x);
                visited[x] = true;
            }
        }
        
    }
    int res = 0;
    for (int k = 2; k <= n; k++){   // Khong xet computer 1
        if (visited[k]) res++;
    }
    cout << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    input();
    bfs(1);
    return 0;
}