// https://www.acmicpc.net/problem/13423

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int t, n;
int a[1001];

// Type 1.1: Tìm chính xác giá trị x trong mảng
int binarySearch(int l, int r, int x){
    while(l <= r) {
        int m = l + (r-l)/2;
        if (a[m] == x) {
            return m;
        } else if (a[m] < x) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return -1;
}

void solve(){
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    sort(a, a + n);
    // --
    int cnt = 0;
    for(int i = 0; i < n - 2; i++){     // Point 1
        for(int j = i + 1; j < n - 1; j++){     // Point 2
            int tmp = a[j] + (a[j] - a[i]);
            if (binarySearch(j+1, n-1, tmp) > 0) {   // BinarySearch for finding point 3
                cnt++;
            }
        }
    }
    cout << cnt << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> t; 
    while(t--){
        solve();
    }
    return 0;
}