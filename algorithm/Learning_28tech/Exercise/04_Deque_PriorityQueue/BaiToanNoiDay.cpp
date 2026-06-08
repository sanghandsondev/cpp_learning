#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// BÀI TOÁN: Nối dây
// Cho N sợi dây với độ dài khác nhau được lưu trong mảng A[]. Nhiệm vụ của bạn là nối N sợi dây thành một sợi sao cho tổng chi phí nối dây là nhỏ nhất
// Biết chi phí nối sợi dây thứ i và sợi dây thứ j là tổng độ dài sợi dây A[i] và A[j]

// Mindset: Cố gắng tạo ra một đoạn dây mới có chiều dài là nhỏ nhất thì chi phí mỗi lần nối sẽ là nhỏ nhất
// Ví dụ: 3, 2, 10 : nếu chọn (2+10)+3 thì chi phí sẽ là (2+10) + (12 + 3) = 27 
//                   nếu chọn (2+3)+10 thì chi phí sẽ là (2+3) + (5 + 10) = 20
//                và nếu chọn (3+10)+2 thì chi phí sẽ là (3+10) + (13 + 2) = 28

// Cách 1: (Có thể sử dụng multiset hoặc sort vector bình thường)
// Cách 2: sử dụng priority queue với top() là nhỏ nhất --> nhanh hơn

int solve1(){
    vector<int> v = {4, 2, 7, 6, 9};
    sort(v.begin(), v.end(), greater<int>()); 
    int ans = 0;
    while(v.size() > 1) {
        sort(v.begin(), v.end(), greater<int>());
        ull temp = v[v.size() - 1] + v[v.size() - 2];
        // for(auto x : v) cout << x << ' ';
        // cout << temp << endl;
        v.pop_back();
        v.pop_back();
        v.push_back(temp);
        ans += temp;
    }
    return ans;
}
// --------------- Sử dụng Priorirty Queue ---------------------
int solve2(){
    vector<int> v = {4, 2, 7, 6, 9};
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto x : v) pq.push(x);
    ull ans = 0;
    while (pq.size() > 1) {
        // lấy ra 2 sợi dây ngắn nhất
        ull top1 = pq.top(); pq.pop();
        ull top2 = pq.top(); pq.pop();
        ans += top1 + top2;
        pq.push(top1 + top2);
    }
    return ans;
}

int main(){
    cout << solve1() << endl;
    cout << solve2() << endl;

    return 0;
}