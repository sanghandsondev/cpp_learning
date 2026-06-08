#include <bits/stdc++.h>
using namespace std;

// BÀI TOÁN: Cho một mảng số ng uyên a có n phần tử: a[n], mảng này được sắp xếp tăng dần
// Hãy tìm hai vị trí khác nhau bất kỳ sao cho tổng của hai phần tử ở hai vị trí đó có giá trị là X
// Ví dụ: a  = [2,5,6,8,10,12,15], X = 16
// left = 0, right = 6
// a[0] + a[6] = 2 + 15 = 17 > 16, mà a[6] + a[i] > a[6] + a[0] = 17 > 16 nên không quan tâm a[6] nữa ===> right = 5
// a[0] + a[5] = 2 + 12 = 14 < 16, mà a[0] + a[i] < a[0] + a[5] = 14 < 16 nên không quan tâm a[0] nữa ===> left = 1
// a[1] + a[5] ...

// Kết luận:
// - Nếu a[l] + a[r] = X, ta tìm được vị trí cần tìm (l và r)
// - Nếu a[l] + a[r] < X, không quan tâm đến a[l] nữa, l++
// - Nếu a[l] + a[r] > X, không quan tâm đến a[r] nữa, r--

void solve(){
    int N = 7, X = 16;
    int a[7] = {2, 5, 6, 8, 10, 12, 15};
    int l = 0, r = 6;
    while (l < r)
    {
        if (a[l] + a[r] == X) {
            cout << l << " " << r;
            return;
        }
        else if (a[l] + a[r] < X) {
            l++;
        } else {
            r--;
        }
    }
    cout << "No solution";
}

int main(){
    solve();
    return 0;
}