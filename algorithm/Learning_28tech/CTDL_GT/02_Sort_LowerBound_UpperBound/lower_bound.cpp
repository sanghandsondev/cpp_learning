#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// lower_bound(): Trả về con trỏ / iterator trỏ đến phần tử đầu tiên có giá trị >= X
// Array : auto p = lower_bound(a, a + n, X)            --> int value = *p;     int index = p - a;
// Vector: auto it = lower_bound(v.begin(), v.end(), X) --> int value = *it;    int index = it - v.begin();

// ChatGPT: Cho một dãy tăng (không giảm) a[n]. Ví dụ 1 2 3 4 4 5 6
// Bài toán 1: Tìm vị trí phần tử đầu tiên ≥ 4 ----> auto p = lower_bound(a, a+7, 4);
// Bài toán 2: Tìm vị trí phần tử đầu tiên > 4 ----> auto p = upper_bound(a, a+7, 4);
// Bài toán 3: Tìm vị trí phần tử cuối cùng <= 4 --> auto p = upper_bound(a, a+7, 4) - 1;   // Lưu ý kiểm tra có ngoài bộ nhớ không
// Bài toán 4: Tìm vị trí phần tử cuối cùng < 4 ---> auto p = lower_bound(a, a+7, 4) - 1;   // Lưu ý kiểm tra có ngoài bộ nhớ không

int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    // -------------- Vector: đã được sắp xếp tăng dần  -----------------
    vector<int> v = {1, 2, 3, 3, 3, 4, 5, 9};

    auto it = lower_bound(v.begin(), v.end(), 5);
    cout << (it - v.begin()) << endl;           // it = 6  => v[6]

    it = lower_bound(v.begin(), v.end(), 3);
    cout << (it - v.begin()) << endl;           // it = 2  => v[2]

    it = lower_bound(v.begin(), v.end(), 10);   
    cout << (it - v.begin()) << endl;           // it = v.end() = 8 --> giá trị rác, đứng sau phần tử cuối cùng, index = n

    return 0;
}