#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// BÀI TOÁN: Giá trị nhỏ nhất của xâu
// Cho xâu S[] gồm các kí tự in hoa [A,B...Z]. Ta định nghĩa giá trị của xâu là tổng bình phương số lần xuất hiện mỗi kí tự trong xâu
// Ví dụ: xâu S[] = "AAABBCD" có F(S) = 3^2 + 2^2 + 1^2 + 1^2 = 15.
// Hãy tìm giá trị nhỏ nhất của  xâu S[] sau khi loại bỏ K ký tự trong xâu.
// Ví dụ: ABCC (k=1) -> ABC -> F(S) = 1 + 1 + 1 = 3
// Ví dụ: ABBCC (k=2) -> ABC -> F(S) = 1 + 1 + 1 = 3

// --- Mindset của tôi: Duyệt từ 1->K, sẽ tìm kí tự có số lần xuất hiện lớn nhất để xóa đi
//                      Bởi vì mỗi lần xóa đi một kí tự, thì sẽ phải tìm đến kí tự có lần xuất hiện lớn nhất tiếp theo -> biến thiên -> Sử dụng Priority queue
int solve(string s, int k){
    map<char,int> m;
    priority_queue<int> pq;
    // Lưu số lần xuất hiện của từng kí tự vào map 
    for(auto x : s){
        m[x]++;
    }
    // Push số lần xuất hiện của các kí tự vào Priority_queue (ko quan tâm là số lần xuất hiện đó thuộc về kí tự nào)
    for (auto it : m){
        pq.push(it.second);
    }
    while (k > 0)
    {
        int top = pq.top(); pq.pop();
        --top; --k;
        pq.push(max(top, 0));   // sau khi trừ số lần xuất hiện của max đi 1 thì push nó lại vào priority_queue
    }
    ull ans = 0;
    // tính tổng bình phương của các lần xuất hiện của các kí tự
    while (!pq.empty())
    {
        ans += 1ull * pq.top() * pq.top();
        pq.pop();
    }
    return ans;
}

// ---


int main(){
    string s1 = "ABCC";
    string s2 = "ABBCC";
    cout << solve(s1, 1) << endl;
    cout << solve(s2, 2) << endl;

    return 0;
}