#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// Bài toán: Liệt kê các số nhị phân từ 1 -> N
// front ----> back
// 1
// 10 11                // 1 -> 10 và 11
// 11 100 101           // 10 -> 100 và 101
// 100 101 110 111      // 11 -> 110 và 111
// 101 110 111 1000 1001    // 100 -> 1000 và 1001

vector<string> res;   // {1, 10, 11, 100, 101, ...}

void init(){
    queue<string> q;
    q.push("1");
    int cnt = 1;
    res.push_back("1");
    while (res.size() < 10000)
    {
        // Lấy phần tử ở đầu hàng đợi
        string top = q.front();
        q.pop();

        // Push kết quả 
        res.push_back(top + "0");
        res.push_back(top + "1");
        q.push(top + "0");
        q.push(top + "1");
    }
}

int main(){
    init();
    cout << res[3];     // số nhị phân thứ 4 -> 4 = 100
    return 0;
}