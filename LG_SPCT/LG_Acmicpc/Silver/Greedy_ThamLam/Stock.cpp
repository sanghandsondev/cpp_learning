// https://www.acmicpc.net/problem/11501

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// -- THAM LAM: Duyệt từ ngược về, ưu tiên thằng giá cao nhất hiện tại để xác định thơi điểm bán
// 6  5 (11) 3  2  4  5 (10) 7  4
// B  B  S   B  B  B  B   S  0  0

void solve(){
    int n;
    cin >> n;
    vector<int> prices(n);
    for(int i = 0; i < n; i++){
        cin >> prices[i];
    }
    
    ull profit = 0;
    int max_price = 0;
    
    // Duyệt ngược 
    for(int i = n - 1; i >= 0; i--){
        // -- Nếu giá hiện tại lớn hơn giá max trước đó (-> stock xu hướng giảm)
        if(prices[i] > max_price) {
            max_price = prices[i];
        // -- Nếu giá hiện tại bé hơn giá max trước đó (-> stock xu hướng tăng)
        } else {
            profit += (max_price - prices[i]);
        }
    }
    cout << profit << endl;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int t;
    cin >> t;
    while(t--){
        solve();
    }
    return 0;
}