// https://www.acmicpc.net/problem/1946

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// --- Sắp xếp lại theo chiều tăng dần của document
// document:   1  2 3 4 5 6 7 8 9 10
// interview:  10 4 2 7 1 5 3 8 6 9
int t, n;
map <int, int> mp;

void solve(){
    cin >> n;
    for(int i = 1; i <= n; i++){
        int x,y ; cin >> x >> y;
        mp[x] = y;
    }
    vector<int> v;
    for(auto x : mp) {
        v.push_back(x.second);
    }
    // THAM LAM: Tìm kiếm giảm dần theo interview sau khi đã sắp xếp tăng dần cho document (Ưu tiên thằng có document nhỏ nhất luôn)
    int count = 1;
    int best_interview = v[0];
    for (int i = 1; i < n; ++i) {
        if (v[i] < best_interview) {
            count++;
            best_interview = v[i];
        }
    }
    cout << count << '\n';
    // ------------------------------- LIS ----------------------------------
    // vector<bool> check(n, true);
    // int maxCnt = 0;
    // for (int i = 0; i < n; i++) {
        // Each mono
        // if (check[i] == true) {
        //     int tmp = v[i];
        //     int cnt = 1;
        //     check[i] = false;
        //     for (int j = i + 1; j < n; j++) {
        //         if (v[j] < tmp) {
        //             tmp = v[j];
        //             cnt++;
        //             check[j] = false;
        //         }
        //     }
        //     maxCnt = max(maxCnt, cnt);
        // }
    // }
    // cout << maxCnt << endl;;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> t;
    while(t--){
        solve();
    }
    return 0;
}