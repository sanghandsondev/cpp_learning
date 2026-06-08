// https://www.acmicpc.net/problem/1032
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int N;
string arr[51];

void solve(){
    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> arr[i];
    }

    string res = arr[0];
    // Duyệt qua từng kí tự
    for (int i = 0; i < res.length(); i++){
        // Duyệt qua từng string của mảng, kiểm tra xem liệu có kí tự nào là khác biệt
        for (int j = 0; j < N; j++){
            if (res[i] != arr[j][i]) res[i] = '?';
        }
    }
    cout << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}