#include <bits/stdc++.h>
using namespace std;

// Giả sử cấu hình cần xây dựng có dạng (X1, X2, ... Xn) ----> Kết quả in ra là một dãy thỏa mãn (Ví dụ: đường đi ma trận,...)
// ĐỘ PHỨC TẠP của backtracking gần như là bằng duyệt trâu
//
// Mô hình thuật toán quay lui:
// Try(int i){
//     for (j = <Khả năng 1>; j <= <Khả năng M>; j++){
//         if (<chấp nhận khả năng j>) {
//             X[i] = <khả năng j>;
//             Status = 0 -> 1
//             if (i == n) 
//                 < Thông báo cấu hình tìm được > ---> Chốt kết quả
//             else 
//                 Try (i + 1);
//             --backtrack--
//             Status = 0 -> 1
//         }
//     }
// }

int main(){
    cout << "BackTracking\n";
    return 0;
}