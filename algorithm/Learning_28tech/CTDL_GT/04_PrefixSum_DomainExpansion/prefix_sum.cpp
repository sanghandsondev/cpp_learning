#include <iostream>
using namespace std;

#define ull long long
#define mod 1000000007

// NOTE 1: Prefix sum rất hay thường được ứng dụng để giải bài toán liên quan đến Đệ quy / Quay lui (Backtracking)
// NOTE 1: Liên quan đến lưu status hiện tại / trước đó cho việc quay lui và thỏa mãn điều kiện

// NOTE 2: Áp dụng sử dụng trong các bài toán mảng 2 chiều với tìm kiếm ma trận con hoặc là đường đi MIN/MAX thỏa mãn nhiều điều kiện nào đó
// NOTE 2: Những bài toán này với hướng đi cố định như đường chéo / lên xuống thì xác định prefix sum sẽ rất dễ dàng

// NOTE 3: Luôn phải init giá trị đầu cho mảng Prefix

// ------------------- Pre-sum: Mảng cộng dồn trên mảng một chiều -----------------------------
// Mảng pre[] là mảng cộng dồn của A[], khi pre[i] = A[0] + A[1] + ... A[i]
// Init:            pre[0] = A[0];
// for (1,n-1) :    pre[i] = pre[i-1] + A[i];

// Example: arr[1, 3, 5, 7, 8]
// -> pre[1, 4, 9, 16, 24]  với pre[i] = arr[i] + pre[i-1];
// Ứng dụng tính tổng đoạn từ i = 1 -> 3 
// Sum[2->4]= pre[4] - pre[1] = 24 - 4 = 20 = [5 + 7 + 8]

// ------------------- Prefix sum: Cộng dồn trên mảng 2 chiều -----------------------------
// Mảng 2 chiều B[n][m] cho trước
// prefix[i][j] = prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1] + B[i][j];



int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int A[5] = {1, 3, 5, 7, 8};
    int pre[5];
    for (int i = 0; i < 5; i++){
        if (i == 0) pre[i] = A[i];      // Init state
        else pre[i] = pre[i-1] + A[i];
    }
    for (int i = 0; i < 5; i++){
        cout << pre[i] << " ";
    }
    cout << endl;
    return 0;
}