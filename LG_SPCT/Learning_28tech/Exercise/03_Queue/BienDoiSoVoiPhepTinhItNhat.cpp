#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// ----------------- Bài toán: Cho 2 số nguyên dương S, T và 2 thao tác (a) (b): -------------------
// (a) S = S - 1
// (b) S = S * 2
// Hãy dịch chuyển S thành T sao cho số lần thực hiện các thao tác (a) (b) là ít nhất.

// TƯỞNG TƯỢNG: nó sẽ giống như bài toán có thể chia ra 2 nhánh tree nhị phân + tìm đường đi ngắn nhất
//              Vậy tại mỗi nhánh có thể của đường đi thứ N sẽ xét điều kiện thỏa mãn hay không, nếu không thì sẽ sinh ra nhánh mới vào QUEUE

vector<string> res;

int solve(int s, int t){
    queue<pair<int, int>> q;    // pair: first là số đang xét, second là nhánh phép tính thứ mấy
    set<int> se;
    se.insert(s);   // cần xét trong set để tránh xét lặp lại số đã xét
    q.push({s, 0});
    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        // case thỏa mãn tại vị trí xét hiện tại / vị trí của phép tính tiếp theo
        if(top.first == t) return top.second;
        if(top.first * 2 == t || top.first - 1 == t) return top.second + 1;
        
        // Nếu không thì thực hiện 2 phép tính để ném vào queue
        // case được phép thực hiện phép tính (b)
        if(se.find(top.first * 2) == se.end() && top.first < t){
            q.push({top.first * 2, top.second + 1});
            se.insert(top.first * 2);
        }
        // case được phép thực hiện phép tính (a)
        if(se.find(top.first - 1) == se.end() && top.first > 1){
            q.push({top.first - 1, top.second + 1});
            se.insert(top.first - 1);
        }
    }
}

int main(){
    cout << solve(2, 5);       // output: 4
    // (2, 0) ---- (4, 1) ---- (8, 2) ...
    //       |            ---- (3, 2) ...-- (6, 3) ...-- (5, 4) ===> 4 phép tính
    //       |
    //        ---- (1, 1) --x-- (2, 2) (số 2 đã xét rồi)
    //                    --x-- (1, 2) (ko lớn hơn 1)


    return 0;
}