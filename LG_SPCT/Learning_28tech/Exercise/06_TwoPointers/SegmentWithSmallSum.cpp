#include <bits/stdc++.h>
using namespace std;

// BÀI TOÁN: Tìm dãy con liên tiếp (subarray) có chiều dài lớn nhất mà thỏa mãn tổng các phần tử <= S

// SOLVE: Xài 2 pointer, kéo dãn right_pointer khi mà tổng phần tử Window hiện tại <= S,
//        Thu hẹp left_pointer khi tổng phần tử > S

int main(){
    int n = 7;
    long long s = 20;
    int a[7] = {2, 6, 4, 3, 6, 8, 9};
    long long sum = 0;
    int l = 0, r;       // two pointers
    int ans = 0;
    // Dịch phải (kéo dãn) right pointer sang phải đến cuối mảng
    for (r = 0; r < n; r++){
        sum+= a[r];
        while (sum > s)
        {
            sum -= a[l];
            ++l;            // thu hẹp (dịch phải) right pointer
        }
        ans = max (ans, r - l + 1);    // so sánh với chiều dài window hiện tại [l...r] khi mà sum thỏa mãn <= 20
    }

    return 0;
}