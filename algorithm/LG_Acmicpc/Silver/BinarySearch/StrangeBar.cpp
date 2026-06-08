// https://www.acmicpc.net/problem/13702

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, k;
vector<int> pitchers;

bool check(int amount) {
    int tmp = 0;
    for (auto it : pitchers){
        tmp += it / amount;
    }
    if (tmp >= k) return true;
    return false;
}

// Binary search type 1.2: Tìm số lớn nhất trong dãy thỏa mãn điều kiện
int binarySearch (int l, int r){
    while(l <= r) {
        int m = l + (r-l)/2; 
        if (check(m)) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return r;
}

void solve(){
    cin >> n >> k;
    for(int i = 0; i < n; i++){
        int x; cin >> x;
        pitchers.push_back(x);
    }
    int maxPitcher = *max_element(pitchers.begin(), pitchers.end());
    cout << binarySearch(1, maxPitcher);
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}