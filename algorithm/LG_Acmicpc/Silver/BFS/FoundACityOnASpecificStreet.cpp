// https://www.acmicpc.net/problem/18352

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, m, k, x;
vector<int> adj[300001];
vector<bool> visited(300001, false);
set<int> city;
int step[300001];

void bfs(int i){
    queue<int> q;
    q.push(i);
    visited[i] = true;
    step[i] = 0;
    while(!q.empty()){
        int v = q.front();
        q.pop();
        for(auto it : adj[v]){
            if(!visited[it]) {
                q.push(it);
                visited[it] = true;
                step[it] = step[v] + 1;
                if (step[it] == k) {
                    city.insert(it);
                }
            }
        }
    }
    if (city.empty()){
        cout << -1;
    } else {
        for (auto it : city) {
            cout << it << endl;
        }
    }
}

void solve(){
    cin >> n >> m >> k >> x;
    for(int i = 0; i < m; i++){
        int x, y; cin >> x >> y;
        adj[x].push_back(y);
    }
    // --- Start from X
    bfs(x);
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);    
    solve();
    return 0;
}