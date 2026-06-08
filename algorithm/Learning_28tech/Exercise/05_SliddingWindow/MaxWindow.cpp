#include <bits/stdc++.h>
using namespace std;

// Tìm dãy con liên tiếp của mảng có độ dài K sao cho tổng các phần tử trong dãy con là lớn nhất và liệt kê dãy đó.
// Nếu có nhiều dãy con có cùng tổng lớn nhất thì in ra dãy con đầu tiên

// Sử dụng SlidingWindow (một kĩ thuật có thể coi là áp dụng 2 pointer)
// 10 3
// 1 2 8 7 9 2 7 8 9 1

int n, k;
int a[1001];

// Cách 1: Duyệt trâu
// void solve(){
//     long long maxi = 0;
//     int indx = 0;
//     for (int i = 0; i <= n -3; i++){
//         long long sum = 0;
//         for (int j = i; j < i + k; j++){
//             sum += a[j];
//         }
//         if (sum > maxi) {
//             maxi = sum;
//             indx = i;
//         }
//     }
//    cout << maxi << endl;
//     for (int i = indx; i < indx + k; i++){
//         cout << a[i] << ' ';
//     }
// }

// Cách 2: Sliding Window
void solve(){
    long long sum = 0;
    for (int i = 0; i < k; i++){    // Cửa sổ đầu tiên
        sum += a[i];
    }
    long long res = sum, idx = 0;
    for (int i = k; i < n; i++){    // Chỉ số kết thúc của cửa sổ (right_pointer) -> left_pointer = a[i - k + 1] 
        sum = sum - a[i-k] + a[i];
        if (sum > res) {
            res = sum;
            idx = i - k + 1;
        }
    }
    cout << res << endl;
    for (int j = 0; j < k; j++){
        cout << a[idx + j] << " ";
    }
}

int main(){
    cin >> n >> k;
    for (int i = 0; i < n; i++){
        cin >> a[i];
    }
    solve();
    return 0;
}