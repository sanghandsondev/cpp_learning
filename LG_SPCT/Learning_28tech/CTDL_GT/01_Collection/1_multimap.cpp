#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
using ll = long long;

// multimap <key, value> ---
// --> 1 key có thể lưu nhiều value khác nhau
// --> các key vẫn sắp xếp theo thứ tự tăng dần (khá giống multiset)
// WARN: KHÔNG CHO PHÉP sử dụng gán mp[i] = j trực tiếp như map<>

// insert({key, value})
// size()
// begin(), end(), rbegin(), rend()
// count(key)
// find(key)  ---> FIRST TO COME

// erase(k) ---> xóa hết phần tử có key = k
// erase(it) ---> chỉ xóa phần tử ở vị trí iterator = it

int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    multimap <int, int> mp;
    mp.insert({1, 100});
    mp.insert({2, 200});
    mp.insert({1, 200});
    mp.insert({2, 300});

    for (auto x : mp){
        cout << x.first << " " << x.second << endl;
    }

    cout << mp.size() << endl;      // output:  4
    cout << mp.count(1) << endl;    // output: 2
    auto it = mp.find(1);
    cout << (*it).first << " " << (*it).second << endl;    // output: {1, 100} FIRST TO COME

    mp.erase(1);    // xóa hết phần tử có key = 1
    cout << mp.count(1) << endl;    // output: 0

    auto it2 = mp.find(2);  // --> FIRST TO COME
    mp.erase(it2);  // xóa phần tử có key = 2 (First to come => xóa {2, 200})
    cout << mp.count(2) << endl;    // output: 1

    return 0;
}