#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long

// NGUYÊN TẮC khi sử dụng ĐỆ QUY: Hàm đệ quy phải có điểm dừng

// BÀI TOÁN: Dãy Fibonacci: 0 1 1 2 3 5 8 13 21 ....
// Fn = n            (n <= 1)  --> F0 = 0; F1 = 1;
// Fn = Fn-1 + Fn-2  (n >= 2)  --> F2 = F1 + F0; F3 = F2 + F1; ...

ull fibo(int n){
    if(n <= 1){
        return n;
    } else {
        return fibo(n-1) + fibo(n-2);
    }
}

int main(){
    cout << fibo(92) << endl;
    return 0;
}