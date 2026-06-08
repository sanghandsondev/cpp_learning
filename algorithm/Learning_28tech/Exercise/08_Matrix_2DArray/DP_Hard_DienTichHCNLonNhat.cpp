#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận A[N][M], gồm số 0 vs 1. Hãy tìm hình chữ nhật lớn nhất có các phần tử đều bằng 1
// --- Input
// 4 4
// 0 1 1 0
// 1 1 1 1
// 1 1 1 1
// 1 1 0 0
// --> Output: 1 1 1 1 
//             1 1 1 1

// SOLUTION:  ./assets/DP_DienTichHCNLonNhat.png
// Xây dựng 1 mảng DP khác là Histogram

int n, m;
int a[100][100];
int hgram[100][100];

// Thực sự bài này thì chỉ quan tâm mindset xây dựng DP histogram như thế nào thôi
// Chứ thực sự không thể tự code được các tính max area từng hàng, khó quá. Sau đây là code mẫu thôi sau này cg chẳng cần đọc đâu:
int max_area(int x[]){
    int res = 0;
    int i = 0;
    stack<int> st;
    while(i < m) {
        if (st.empty() || x[i] >= x[st.top()]) st.push(i++);
        else {
            int top = st.top();
            st.pop();
            if(st.empty()){
                res = max(res, i * x[top]);
            } else {
                res = max(res, x[top]*(i-st.top()-1));
            }
        }
    }
    while (!st.empty())
    {
        int top = st.top();
        st.pop();
        if(st.empty()){
            res = max(res, i * x[top]);
        } else {
            res = max(res, x[top]*(i-st.top()-1));
        }
    }
    return res;
}

void solve(){
    cin >> n >> m;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> a[i][j];
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if (i == 0) hgram[i][j] = a[i][j];      // Hàng đầu tiên
            else if (a[i][j] == 1){
                hgram[i][j] = hgram[i-1][j] + 1;    // -- Công thức DP, prefix sum theo từng cột thôi (xem hình ảnh)
            } else {
                hgram[i][j] = 0;
            }
        }
    }
    int ans = 0;
    // Xét area qua từng hàng (xem hình ảnh)
    for(int r = 0; r < n; r++){
        ans = max(ans, max_area(hgram[r]));
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}