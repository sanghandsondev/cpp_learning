// https://www.acmicpc.net/problem/1504

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007
#define INF 1e9

// Check 2 case : 1 -> v1 -> v2 -> N and 1 -> v2 -> v1 -> N
// Point out the case with less distance
// =>
// route1 = dist_from_1[v1] + dist_from_v1[v2] + dist_from_v2[N]
// route2 = dist_from_1[v2] + dist_from_v2[v1] + dist_from_v1[N]

int N, E, v1, v2;
vector<pair<int,int>> adj[200001];

vector<int> dijkstra(int start){
    vector<int> dist(N+1, INF);
    dist[start] = 0;
    
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({dist[start], start});
    
    while(!pq.empty()){
        pair<int,int> tmp = pq.top(); pq.pop();
        int cur_p = tmp.second;
        int cur_dist = tmp.first;
        if (cur_dist > dist[cur_p]) continue;
        for(auto it : adj[cur_p]) {
            int next_p = it.first;
            int next_dist = it.second;
            int new_dist = next_dist + cur_dist;
            if (new_dist < dist[next_p]) {
                dist[next_p] = new_dist;
                pq.push({new_dist, next_p});
            }
        }
    }
    return dist;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> N >> E;
    for(int i = 0; i < E; i++){
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});      // two-way road
    }
    cin >> v1 >> v2;
    
    vector<int> d1 = dijkstra(1);
    vector<int> d_v1 = dijkstra(v1);
    vector<int> d_v2 = dijkstra(v2);
    
    ull route1 = 1LL * d1[v1] + d_v1[v2] + d_v2[N];
    ull route2 = 1LL * d1[v2] + d_v2[v1] + d_v1[N];
    
    ull res = min(route1, route2);
    if (res >= INF || res < 0) cout << -1 << endl;
    else cout << res << endl;
    
    return 0;
}