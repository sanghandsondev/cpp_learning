#include <bits/stdc++.h>
#include <algorithm>    // sort()
using namespace std;

int tongchuso(int n){
    int ans = 0;
    while(n){
        ans+= n % 10;
        n /= 10;
    }
    return ans;
}
bool cmp(int a, int b){                 // a đứng trước, b đứng sau
    return tongchuso(a) < tongchuso(b); // Tăng dần
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    // -------------- Array ------------------
    int n = 5;
    int a[n] = {3, 5, 2, 1, 2};
    sort(a, a + n);                     // output: 1 2 2 3 5    =>  a[0, n-1]
    sort(a, a + n, greater<int>());     // output: 5 3 2 2 1

    // -------------- Vector -----------------
    vector<int> v = {6, 8, 5, 5, 3, 2, 5, 1};
    sort(v.begin(), v.end());                      // output: 1 2 3 5 5 5 6 8

    sort(v.begin(), v.end(), greater<int>());      // sort giảm dần, output: 8 6 5 5 5 3 2 1
    sort(v.rbegin(), v.rend());                    // sort giảm dần, output: 8 6 5 5 5 3 2 1

    // sort(v.begin() + 1, v.begin() + 3 + 1);      // => v[1, 3]

    // --------------Comparison function custom ---------------
    vector<int> v1 = {12, 42, 111, 56, 123, 456, 11111};
    sort(v1.begin(), v1.end(), cmp);                // output: 12 111 11111 42 123 56 456
    // stable_sort(v1.begin(), v1.end(), cmp);      // output: 12 111 11111 42 123 56 456 (giữ nguyên thứ tự phần tử)

    sort(v1.begin(), v1.end(), [](int &a, int &b){      // -> Lamda function
        return tongchuso(a) < tongchuso(b);
    });

    return 0;
}