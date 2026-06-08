// https://leetcode.com/problems/number-of-sub-arrays-with-odd-sum/description/

//  ========== Phân biệt sub-arrays và sub-sequences: ===========================
// Ví dụ có mảng arr = [1,3,5,6,7];
// sub-array: [1,3] [1,3,5] [3,5] [5,6,7] ---> các phần tử phải đứng cạnh nhau theo thứ tự của mảng ban đầu
// sub-sequence: [1,3] [1,3,6] [1,6] -=--> các phần tử có thể không cần đứng cạnh nhau, nhưng vẫn theo thứ tự của mảng ban đầu
    // ** Sub-sequence trong trường hợp cụ thể không quan tâm thứ tự phần tử thì các mảng con sub-sequence cũng được xem như "chọn các phần tử trong mảng"
// =============================================================================

// Duyệt từ trái sang phải, for i từ 0 -> n.
// Giả sử hiện tại đang xét cho điểm index i là k = array[i]
// -> Cần lưu lại: number of subarray có tổng lẻ (odd) & number of subarray có tổng chẵn (even) (tính đến index 0 -> k-1)
// Khi xét thêm index i (add thêm 1 phần tử k) -> xảy ra 2 TH: 
// 1. k lẻ
    // -> tất cả các subarray chẵn từ trước sẽ biến thành lẻ (do + thêm k)
    // -> số subarray chẵn mới = odd
    // -> tất cả các subarray lẻ từ trước sẽ thành chẵn (+k)
    // -> số subarray lẻ mới = even + 1 (riêng phần tử k chính là 1 subarray lẻ) 
    // -> Đáp án + thêm subarray lẻ mới
// 2. k chẵn
    // -> số subarray chẵn mới = số subarray chẵn cũ + 1 (do + thêm k)
    // -> số subarray lẻ mới = subarray lẻ cũ
    // -> Đáp án + thêm sub array lẻ mới

//                     [1]     [3]     [5]     [6]
// Sub-array chẵn: 0    0       1       1       2
// Sub-array lẻ:   0    1       1       2       2
// Result:         0  + 1   +   1   +   2   +   2  = 6

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n;
int arr[100001];
ull sumOdd = 0;     // number of Sub-array có tổng lẻ hiện xét
ull sumEven = 0;    // number of Sub-array có tổng chẵn hiện xét
ull res;

void solve(){
    cin >> n;
    for (int i = 0; i < n; i++){
        cin >> arr[i];
    }
    for (int i = 0; i < n; i++){
        if (arr[i] % 2 != 0) {
            // Nếu phần tử hiện xét là lẻ
            int temp = sumEven;
            sumEven = sumOdd;
            sumOdd = temp + 1;
            res += sumOdd;
        } else {
            // Nếu phần tử hiện xét là chẵn
            sumEven+= 1;
            res += sumOdd;
        }
    }    
    cout << res % mod;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    // int t; cin >> t;
    // while(t--) solve();
    solve();
    return 0;
}