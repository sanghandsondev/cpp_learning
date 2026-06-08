#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// FIFO: First in first out ---> O(1)
// push(), pop()
// front(), empty(), size()

// NOTE: giải các bài toán liên quan đến BFS (tìm đường đi ngắn nhất blabla)
// Hoặc là các dạng bài từ số A biến thành số B thông qua các phép tính (cách)

int main(){
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    cout << q.front() << endl;      // 1
    cout << q.size() << endl;       // 4
    q.pop();    // pop front element ra khỏi queue
    cout << q.front() << endl;      // 2

    // -------------------
    queue<pair<int,int>> qe;
    qe.push({1,2});
    qe.push({2,3});
    pair<int,int> tmp = qe.front(); qe.pop();
    int fir = tmp.first;
    int sec = tmp.second;
    cout << fir << " " << sec << endl;

    return 0;
}