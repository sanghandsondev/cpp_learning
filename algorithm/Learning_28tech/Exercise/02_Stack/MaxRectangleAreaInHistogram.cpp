#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long
// ------------------------------- Bài toán ---------------------------------
// Tìm diện tích hình chữ nhật lớn nhất được tạo nên bởi biểu đồ cột
// 6 2 5 4 5 1 6
// ====== 6
// ==     2
// ****=  5
// ****   4
// ****=  5
// =      1
// ====== 6
// phần diện tích chứa kí tự '*' là diện tích lớn nhất (12)
//--------------------------------- Hướng giải -------------------------------
void solve(vector<int>& a){
    stack<int> st;
    int res = INT_MIN;
    int i = 0;
    int size = a.size();
    while (i < size)
    {
        if (st.empty() || a[i] >= a[st.top()]){     // Thằng bên phải lớn hơn thằng bên trái thì thằng bên trái vẫn là "cận trái để xét diện tích"
            st.push(i);
            i++;
        } else {
            int idx = st.top();
            st.pop();
            // Tính toán HCN tạo bởi cột hiện tại, trong đó cột hiện tại được coi là cột ngắn nhất của HCN đó
            if (st.empty()) {
                res = max(res, i * a[idx]);
            } else {
                res = max(res, a[idx] * (i - st.top() - 1));
            }
        }
    }
    while (!st.empty())
    {
        int idx = st.top();
        st.pop();
        // tính toán HCN tạo bởi cột hiện tại, trong đó cột hiện tại được coi là cột ngắn nhất của HCN đó
        if (st.empty()) {
            res = max(res, i * a[idx]);
        } else {
            res = max(res, a[idx] * (i - st.top() - 1));
        }
    }
    cout << res << endl;
}

int main(){
    vector<int> a = {6, 2, 5, 4, 5, 1, 6};
    solve(a);       // 12
    return 0;
}