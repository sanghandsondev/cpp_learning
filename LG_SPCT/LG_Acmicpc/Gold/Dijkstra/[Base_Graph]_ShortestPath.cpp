// https://www.acmicpc.net/problem/1753

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007
const int maxn = 300001;

int V, E, K;
vector<pair<int, int>> a[maxn];     // Next point + Weight

void input(){
    cin >> V >> E >> K;
    for(int i = 0; i < E; i++){
        int u,v,w;
        cin >> u >> v >> w;
        a[u].push_back({v, w});     // Next point + Weight
    }
}

void dijkstra(int start){
    vector<int> dist(V+1, INT_MAX);
    dist[start] = 0;
    // vector<bool> visited(V+1, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({dist[start], start});
    
    while(!pq.empty()){
        pair<int,int> tmp = pq.top(); pq.pop();
        int weight_p = tmp.first;
        int p = tmp.second;

        // ------ Check condition -----------------
        // if (visited[p]) continue;
        // visited[p] = true;
        if (weight_p > dist[p]) continue;
        
        // ------ Check next move -----------------
        for(auto it : a[p]) {
            int next_p = it.first;
            int next_weight = it.second;
            int new_weight = weight_p + next_weight;
            // ---- If current weight is less than weight of next_point -> push
            if (new_weight < dist[next_p]){
                dist[next_p] = new_weight;
                pq.push({new_weight, next_p});
            }
        }
    }
    for(int i = 1; i <= V; i++){
        if (dist[i] == INT_MAX) cout << "INF" << endl;
        else cout << dist[i] << endl;
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    input();
    dijkstra(K);
    return 0;
}