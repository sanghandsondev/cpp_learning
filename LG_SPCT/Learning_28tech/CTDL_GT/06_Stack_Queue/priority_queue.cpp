#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long
using pii = pair<int, pair<int,int>>;

// =================== Priority_Queue  ===================================
// top() -> truy cập phần tử ở đầu, mặc định là lớn nhất
// pop() -> xóa phần tử ở đầu (phần tử lớn nhất hiện tại trong queue)
// push()
// size(), empty()

// Khi nào sử dụng: Khi mà luôn muốn tìm đến thằng nhỏ nhất / lớn nhất trong mảng mà vị trí của nó biến thiên liên tục
// NOTE: Có thể custom điều kiện để get thằng top() ra theo yêu cầu bài toán (giả sử thằng top() là số chẵn lớn nhất / số nguyên tố bé nhất / ...)

int main(){
    // ------ Mặc định ưu tiên thằng lớn nhất ---------------------
    priority_queue<int> pq;
    pq.push(1);
    pq.push(4);
    pq.push(3);
    pq.push(5);
    cout << pq.top() << endl;  // 5

    pq.push(100); 
    pq.push(30);
    cout << pq.top() << endl;  // 100

    pq.pop();  // xóa 100
    cout << pq.top() << endl;  // 30

    // ------ Chuyển sang ưu tiên get ra thằng nhỏ nhất ------------
    priority_queue<int, vector<int>, greater<int>> pq2;
    priority_queue<pii, vector<pii>, greater<pii>> pq3;
    pq2.push(1);
    pq2.push(4);
    pq2.push(3);
    pq2.push(5);
    cout << pq2.top() << endl;  // 1

    pq2.push(0); 
    pq2.push(30);
    cout << pq2.top() << endl;  // 0

    pq2.pop();  // xóa 0
    cout << pq2.top() << endl;  // 1

    return 0;
}