#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    //--
    // pair<int, int> p;
    // p.first = 10; p.second = 20;
    //--
    // pair<int, int> p = {10, 20};
    pair<int, int> p = make_pair(10, 20);
    cout << p.first << ' ' << p.second << endl;     // 10 20

    pair<int, int> p1 = p;
    cout << p1.first << ' ' << p1.second << endl;   // 10 20

    // pair<int, int> a[10];
    vector<pair<int, int>> v = {3, {4,5}}; // {4,5},{4,5},{4,5};
    for (auto x : v) {
        cout << x.first << ' ' << x.second << endl;
    }

    return 0;
}