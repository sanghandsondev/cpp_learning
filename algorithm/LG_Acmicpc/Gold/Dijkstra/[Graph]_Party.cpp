// https://www.acmicpc.net/problem/1238

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Min cost from: 1-N --> X
// +
// Min cost from: X --> 1-N
// (Có thể tối ưu hơn bằng cách lượt đi thì cho dijkstra một lượt từ X -> 1-N)

int N, M, X;
vector<pair<int,int>> adj[10001];
vector<int> fulltime(1001, 0);

int dijkstra(int start, int end){
    vector<int> timers(N+1, INT_MAX);
    timers[start] = 0;
    
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({timers[start], start});
    
    while(!pq.empty()){
        pair<int,int> tmp = pq.top(); pq.pop();
        int cur_p = tmp.second;
        int cur_time = tmp.first;
        
        if (cur_p == end) return cur_time;
        if (cur_time > timers[cur_p]) continue;
        for(auto it : adj[cur_p]) {
            int next_p = it.first;
            int next_time = it.second;
            int new_time = next_time + cur_time;
            if (new_time < timers[next_p]) {
                timers[next_p] = new_time;
                pq.push({new_time, next_p});
            }
        }
    }
    return 0;
}

int solve(){
    for(int i = 1; i <= N; i++){
        if (i != X) {
            fulltime[i] += dijkstra(i, X) + dijkstra(X, i);
        }
    }
    sort(fulltime.begin(), fulltime.end());
    return *fulltime.rbegin();      // last element -> max element
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> N >> M >> X;
    for(int i = 0; i < M; i++){
        int x,y,t;
        cin >> x >> y >> t;
        adj[x].push_back({y,t});    // next city + time travel
    }
    int res = solve();
    cout << res;
    return 0;
}