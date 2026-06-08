// https://www.acmicpc.net/problem/1037
// Note: Ước nhỏ nhất * ước lớn nhất là ra số cần tìm
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int N;
int arr[51];

int solve(){
    cin >> N;
    for (int i = 0; i < N; i++){
        cin >> arr[i];
    }
    if (N == 0) return 1;
    if (N == 1) return arr[0]*arr[0];
    // Sắp xếp thành mảng tăng dần
    sort(arr, arr + N);
    return arr[0]*arr[N-1];  // Ước nhỏ nhất * Ước lớn nhất
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cout << solve();
    return 0;
}