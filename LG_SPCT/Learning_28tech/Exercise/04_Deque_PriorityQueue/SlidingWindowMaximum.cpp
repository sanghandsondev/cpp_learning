#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// Sliding Window Maximum: Tìm số lớn nhất của mọi cửa sổ kích thước K của mảng
// 7 3 1 2 8 6 4 1 0 (k = 3)
// {7, 3, 1} -> 7
// {3, 1, 2} -> 3
// {1, 2, 8} -> 8
// {2, 8, 6} -> 8
// {8, 6, 4} -> 8
// {6, 4, 1} -> 6
// {4, 1, 0} -> 4

int n = 9;
int a[9] = {7, 3, 1, 2, 8, 6, 4, 1, 0};
int k = 3;
vector<int> res;
// Cách 1: duyệt trâu ==> O(n*k)
void solve1(){
    for(int i = 0; i <= n - k; i++){
        int temp = INT_MIN;
        for(int j = 0; j < k; j++){
            temp = max (temp, a[i + j]);
        }
        cout << temp << " ";
    }
}
// Cách 2: sử dụng Deque ==> O(2n)
void solve2(){
    deque<int> q;    // Lưu chỉ số của phần tử mảng
    // ------------ handle cửa sổ đầu tiên 
    for(int i = 0; i < k; i++){
        // push đảm bảo thằng đứng ở front phải lớn hơn thằng đứng ở back ---> deque: front (7 3 1) back
        while (!q.empty() && a[i] > a[q.back()]) 
        {
            q.pop_back();
        }
        q.push_back(i);
    }
    res.push_back(a[q.front()]);    // front luôn là thằng lớn nhất trong cửa sổ đang xét
    // ------------ handle các cửa sổ tiếp theo
    for(int i = k; i < n; i++){
        if (q.front() <= i - k){    // xét thằng lớn nhất (front) có đang thuộc window đang xét hay không
            q.pop_front();
        }
        // push đảm bảo thằng đứng ở front phải lớn hơn thằng đứng ở back ---> nên deque tiếp theo sẽ là (3, 2) chứ ko phải (3 1 2) ---> thằng 1 đã bị pop ra do 1 <= 2 
        while (!q.empty() && a[i] >= a[q.back()])
        {
            q.pop_back();
        }
        q.push_back(i);
        res.push_back(a[q.front()]);
    }
    //----
    for (auto x : res){
        cout << x << " ";
    }
}

int main(){
    solve1();
    cout << endl;
    solve2();
    return 0;
}