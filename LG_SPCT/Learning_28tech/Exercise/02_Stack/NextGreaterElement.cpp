#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long
// Next Greater Element: Phần tử đầu tiên lớn hơn bên phải
// Ví dụ: 3 4 2 5 1  6  2
// -----> 4 5 5 6 6 -1 -1
// Ví dụ: 4  9 2 1 4 5  9 2  8  7
// -----> 9 -1 4 4 5 9 -1 8 -1 -1
// ----------------------- Cách 1: for trâu
// void solve(vector<int>& a){
//     vector<int> temp;
//     for (int i = 0; i < a.size(); i++){
//         int res = -1;
//         for (int j = i + 1; j < a.size() ; j++){
//             if (a[j] > a[i]) {
//                 res = a[j];
//                 break;
//             }
//         }
//         temp.push_back(res);
//     }
//     for (auto x : temp) {
//         cout << x << ' ';
//     }
//     cout << endl;
// }
// -------------------------- Cách 2: STACK (Monotonic Stack)
void solve(vector<int>& a){
    stack <int> st;
    vector<int> b(a.size());
    for (int i = 0; i < a.size(); i++){
        if (st.empty()){
            st.push(i);     // push chỉ số trong trường hợp xét phần tử đầu tiên của mảng
        } else {
            while(!st.empty() && a[st.top()] < a[i]) {      // Khi stack không rỗng thì kiểm tra chỉ số
                b[st.top()] = a[i]; // cập nhật giá trị phần tử lớn hơn bên phải cho chỉ số hiện tại
                st.pop();
            }
            st.push(i);     // push chỉ số hiện tại trong trường hợp không thỏa mãn điều kiện trên / sau khi đã pop hết những thằng thỏa mãn
        }
    }
    while(!st.empty()){
        b[st.top()] = -1;   // những chỉ số của phần tử không tìm thấy thằng nào lớn hơn bên phải vẫn còn lưu trong stack
        st.pop();
    }

    for (auto x : b) {
        cout << x << ' ';
    }
    cout << endl;
}

int main(){
    vector<int> a = {3, 4, 2, 5, 1, 6, 2};
    vector<int> b = {4, 9, 2, 1, 4, 5, 9, 2, 8, 7};
    solve(a);       // 4 5 5 6 6 -1 -1
    solve(b);       // 9 -1 4 4 5 9 -1 8 -1 -1
    return 0;
}