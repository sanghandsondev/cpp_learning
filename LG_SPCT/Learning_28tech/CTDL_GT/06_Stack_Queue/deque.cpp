#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define ull long long

// DEQUE: Double End QUEue
// size(), empty(), front(), back()
// push_front() pop_front()
// push_back() pop_back()

int main(){
    deque<int> dq;
    dq.push_front(2);
    dq.push_back(3);
    dq.push_back(4);
    dq.push_back(5);
    dq.push_front(1);       // 1 2 3 4 5 
    cout << dq.front() << endl;     // 1
    cout << dq.back() << endl;      // 5
    cout << dq.size() << endl;      // 5
    //----------------------------------------
    dq.pop_back();          // 1 2 3 4
    dq.pop_front();         //   2 3 4
    cout << dq.front() << endl;         // 2
    cout << dq.back() << endl;          // 4
    cout << dq.size() << endl;          // 3

    return 0;
}