#include <iostream>
using namespace std;

#define ull long long
#define mod 1000000007

// ------------------- Domain expansion: Kỹ thuật loang xung quanh của mảng 2 chiều -----------------------------
// Ứng dụng vào hàm đệ quy để tìm các nước đi tiếp theo của quân cờ thỏa mãn, phạm vi phát sóng của trạm, ....
// Ví dụ: [i-1, j-1]   [i-1, j]   [i-1, j+1]
//        [i,   j-1]   [i,   j]   [i,   j+1]
//        [i+1, j-1]   [i+1, j]   [i+1, j+1]

// BÀI TOÁN: Cho ma trận A[N,M]. Điểm được coi là cực đại trong ma trận nếu nó lớn hơn tất cả các số ở ô xung quanh mà chung đỉnh với nó
// Hãy đếm xong ma trận có bao nhiêu điểm cực đại
// Input
// 5 3
// 1 1 2
// 1 1 1
// 1 1 2
// 2 2 1
// 2 1 2

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int n,m;
int a[1001][1001];

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cin >> a[i][j];
        }
    }
    int count = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            // check 8 ô xung quanh điểm (i,j)
            bool check = true;
            for (int k = 0; k < 8; k++){
                int i1 = i + dx[k];
                int j1 = j + dy[k];
                // check thỏa mãn điểm xung quanh nằm trong ma trận
                if (i1 >= 0 && i1 < n && j1 >= 0 && j1 < m) {
                    if (a[i1][j1] >= a[i][j]) {
                        check = false;
                        break;
                    }
                }
            }
            if (check) count++;
        }
    }
    cout << count;
    return 0;
}