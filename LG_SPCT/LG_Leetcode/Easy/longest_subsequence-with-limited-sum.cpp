// https://leetcode.com/problems/longest-subsequence-with-limited-sum/

// Cách 1: Sắp xếp mảng tăng dần -> Duyệt từ index 0 -> cho đến khi sum > queries[] thì break và get số index => số phần tử tối đa thỏa mãn
// Cách 2 (Binary-search) Sắp xếp mảng tăng dần -> Prefix sum -> Sử dụng Binary search để tìm vị trí lớn nhất thỏa mãn <= queries[]

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n,m;
ull nums[1001], queries[1001];

// Type 1.2: Tìm index phần tử lớn nhất thỏa mãn điều kiện biểu thức EX
int binarySearch2(ull arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] <= x) {      // giả sử điều kiện biểu thức EX: thỏa mãn <= giá trị x
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return r;
}

void solve(){
    cin >> n;
    cin >> m;
    for (int i = 0; i < n; i++){
        cin >> nums[i];
    }
    for (int i = 0; i < m; i++){
        cin >> queries[i];
    }
    // Sort
    sort(nums, nums + n);

    // Prefix sum
    for (int i = 1; i < n; i++){
        nums[i] += nums[i-1];
    }

    // Binary search, bên trái TRUE (thỏa mãn <= queries[i])
    for (int i = 0; i < m; i++){
        cout << binarySearch2(nums, n, queries[i]) + 1 << ' ';
    }
}
// -------------------------- Solution for Leetcode -----------------------------------
// vector<int> answerQueries(vector<int>& nums, vector<int>& queries) {
//     // Sort
//     sort(nums.begin(), nums.end());

//     // Prefix sum
//     for (int i = 1; i < nums.size(); i++) {
//         nums[i] += nums[i-1];
//     }

//     // Binary search
//     for (auto x : queries){
//         res.push_back(binarySearch2(nums, nums.size(), x) + 1);
//     }
//     return res;
// }
//--------------------------------------------------------------------------------------

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    // int t; cin >> t;
    // while(t--) solve();
    solve();
    return 0;
}