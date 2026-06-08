#include <bits/stdc++.h>
using namespace std;

// BÀI TOÁN: Merge 2 mảng ko giảm thành 1 mảng ko giảm

// SOLVE: Sử dụng 2 pointer nhưng không phải trên 1 mảng mà xài trên 2 mảng, mỗi mảng 1 pointer

int n = 6, m = 7;
int a[6] = {1, 6, 9, 13, 18, 18};
int b[7] = {2, 3, 8, 13, 15, 21 , 25};

int main(){
    // multiset<int> ms;
    // for (int i = 0; i < n; i++){
    //     ms.insert(a[i]);
    // }
    // for (int i = 0; i < m; i++){
    //     ms.insert(b[i]);
    // }
    // for (auto x : ms) {
    //     cout << x << ' ';
    // }
    int i = 0, j = 0;        // two pointers
    vector<int> v;
    while (i < n && j < m)
    {
        if (a[i] <= b[j]) {
            v.push_back(a[i]);
            i++;
        } else {
            v.push_back(b[j]);
            j++;
        }
    }
    // Kiểm tra lại 1 trong 2 pointer sẽ vẫn chưa chạy hết mảng -> in ra nốt
    while (i < n)
    {
        v.push_back(a[i]);
        i++;
    }
    while (j < m)
    {
        v.push_back(b[j]);
        j++;
    }
    // output
    for (auto x : v){
        cout << x << ' ';
    }
    
    return 0;
}