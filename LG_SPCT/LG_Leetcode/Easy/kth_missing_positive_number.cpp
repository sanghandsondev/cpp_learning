// https://leetcode.com/problems/kth-missing-positive-number/description/

// Cách 1: Tạo mảng b[3000], xét b[i] = 1 khi i tồn tại trong mảng đã cho. For từ 1 -> 2999 để tìm ra phần tử b[i] != 1 và tại thời điểm đó count == k. Return i
//     int b[3000];
//     int count = 0;
//     int findKthPositive(vector<int>& arr, int k) {
//         for (auto x : arr){
//             b[x] = 1;
//         }
//         for (int i = 1; i < 3000; i++){
//             if (b[i] != 1) count++;
//             if (count == k) return i;
//         }
//         return 1;
//     }

// Cách 2: Binary search - Khoai vãi
// class Solution {
//     public:
//         vector<int> miss;
//         int findKthPositive(vector<int>& arr, int k) {
//             int number = 0;
//             for (int i = 0; i < arr.size(); i++){
//                 if (i == 0) {
//                     number = arr[0] - 1;
//                     miss.push_back(number);
//                 }
//                 else {
//                     number += arr[i] - arr[i-1] - 1;
//                     miss.push_back(number);
//                 }
//             }
//             int index = binarySearch(0,miss.size()-1,k);
//             if (index == -1){
//                 return k;
//             }
//             if (miss[index] == k){
//                 return arr[index]-1;
//             }
//             else {
//                 return arr[index] + k - miss[index];
//             }
//             return 0;
//         }
     
//         int binarySearch(int l, int r, int x)
//         {
//             while (l <= r) {
//                 int m = l + (r - l) / 2;      
//                 if (miss[m] < x)                // Lớn nhất 
//                     l = m + 1;
//                 else
//                     r = m - 1;
//             }
//             return r;
//         }
//};

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

void solve(){
    
}


int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    // int t; cin >> t;
    // while(t--) solve();
    solve();
    return 0;
}