// https://www.acmicpc.net/problem/14889

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n;
int a[21][21];
bool visited[21];
int res;
// int X[21];

// Calculate the abilities of each team
int ab(vector<int> &team){
    int sumx = 0;
    int len = team.size();
    if (len == 0 || len == 1) return 0;
    for(int i = 0; i < len - 1; i++){
        for(int j = i + 1; j < len; j++){
            sumx = sumx + a[team[i]][team[j]] + a[team[j]][team[i]];
        }
    }
    return sumx;
}

// Calculate difference between the abilities of 2 teams
int distanceX(int a, int b){
    if (a >= b) return a - b;
    return b - a;
}

// BACKTRACK KHÔNG TRUY VẾT    (tức không cần truy vết nghiệm X[i])
// -> Bài này chỉ quan tâm số phần tử của nghiệm Count == n/2 là được
// -> Ngoài ra cũng cần quan tâm về chỉ số trước đó bằng đối số "Here" để duyệt tối ưu hơn
void backtrack(int here, int Count){
    if (Count == n/2){
        vector<int> link, start;
        for(int k = 1; k <= n; k++){
            if(visited[k] == false) {
                start.push_back(k);
            } else {
                link.push_back(k);
            }
        }
        int abi1 = ab(link);
        int abi2 = ab(start);
        int dis = distanceX(abi1, abi2);
        res = min(res, dis);
        return;
    }
    for (int i = here + 1; i <= n; i++){
        visited[i] = true;
        backtrack(i, Count + 1);
        visited[i] = false;
    }
}

void solve(){
    cin >> n;
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            cin >> a[i][j];
        }
    }
    res = INT_MAX;
    memset(visited, false, sizeof(visited));
    visited[1] = true;
    // X[1] = 1;
    // Try(2); 
    backtrack(1,1);
    cout << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}