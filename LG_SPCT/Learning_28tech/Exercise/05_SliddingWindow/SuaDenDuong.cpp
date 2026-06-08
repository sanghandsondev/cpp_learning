#include <bits/stdc++.h>
using namespace std;

// Tuyến đường có N chiếc đèn, sau bão đã làm hỏng mất B chiếc
// Để khắc phục sự cố tạm thời ---> có ít nhất một khu vực có K chiếc đèn liên tiếp hoạt động
// Xác định xem số đèn đường cần phải sửa chữa ít nhất là bao nhiêu?

// NOTE: Yêu cầu bài toán tương đương với tìm cửa sổ K phần tử có số đèn hỏng là nhỏ nhất (ans)
// NOTE: Kết quả sẽ = ans;

int main(){
    int n = 10, k = 6, b = 5;
    int dead[5] = {2,10,1,5,9};     // vị trí các chiếc đèn bị hỏng
    int a[1001] = {0};
    for (int i = 0; i < b; i++){
        a[dead[i]] = 1;      // đánh dấu chỉ số đèn hỏng trong mảng a
    }
    int cnt = 0;
    for (int i = 1; i <= k; i++){    // Xét window đầu tiên
        if (a[i] == 1) cnt++;
    }
    int ans = cnt;
    for (int i = k + 1; i <= n; i++){   // Xét các window tiếp theo với Right_pointer
        cnt = cnt - a[i - k] + a[i];    // Xác định số đèn hỏng ở mỗi window
        ans = min (ans, cnt);
    }
    cout << ans;
    return 0;
}