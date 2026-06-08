// https://www.acmicpc.net/problem/1916

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int N, M, A, B;
vector<pair<int,int>> adj[100001];  // next city + bus fare

int dijkstra(int start, int end){
    vector<int> cost(N+1, INT_MAX);
    cost[start] = 0;
    
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;    // {cost, point}
    pq.push({cost[start], start});
    
    while(!pq.empty()){
        pair<int,int> tmp = pq.top(); pq.pop();
        int cur_cost = tmp.first;
        int cur_point = tmp.second;
        // cout << cur_point << endl;
        if (cur_point == end) return cur_cost;
        
        if (cur_cost > cost[cur_point]) continue;
        
        for(auto it : adj[cur_point]){
            int next_point = it.first;
            int next_fare = it.second;
            int new_cost = next_fare + cur_cost;
            
            if (cost[next_point] > new_cost) {
                cost[next_point] = new_cost;
                pq.push({new_cost, next_point});
            }
        }
    }
    return 0;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> N >> M;
    for(int i = 0; i < M; i++){
        int x,y,f;
        cin >> x >> y >> f;
        adj[x].push_back({y,f});    // next city + bus fare
    }
    cin >> A >> B;
    int min_cost = dijkstra(A, B);
    cout << min_cost;
    return 0;
}