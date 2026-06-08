#include <bits/stdc++.h>
using namespace std;

// Hãy sắp đặt lại các phần tử của mảng sao cho các số nhỏ hơn hoặc bằng K đứng cạnh nhau.
// ví dụ: A[] = {2, 1, 5, 6, 3} ----> A[] = {2,1, 3, 6, 5} với K = 3
// {8 1 2 3 4 3 5 1 2} , K = 3 ------> { 8, 1 2 3 3 1 2, 4 5}
// Output: Số phép dịch chuyển ít nhất

// NOTE: Yêu cầu bài toán tương đương với tìm cửa sổ có số phần tử thỏa mãn là nhiều nhất (ans)
// NOTE: Kết quả sẽ = lengthWindow - ans;

int main(){
    int n = 7;
    int k = 5;
    int a[7] = {2, 7, 9, 5, 8, 7, 4};
    int d = 0;
    for (int i = 0; i < n; i++){    // Xác định chiều dài window sau khi dịch chuyển (thỏa mãn các phần tử <= k)
        if (a[i] <= k) ++d;
    }
    int cnt = 0;
    for (int i = 0; i < d; i++){    // Xác định số phần tử <= k ở window đầu tiên
        if (a[i] <= k) ++cnt;
    }
    int ans = cnt;
    for (int i = d; i < n; i++){    // Right_pointer
        if (a[i] <= k) cnt++;
        if (a[i-d] <= k) cnt--;
        ans = max(ans, cnt);        // --> Tìm ra được cửa sổ có nhiều phần tử <= k nhất để làm gốc
    }
    // Output: Số phép dịch chuyển ít nhất = d - ans
    cout << d - ans;

    return 0;
}