#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Cho ma trận N*M. Tìm tổng đường dẫn lớn nhất trong ma trận 
// Đường dẫn tối đa là tổng của các phần tử từ hàng đầu tiên đến hàng cuối cùng mà bạn chỉ được phép di chuyển xuống dưới 1 đơn vị (theo cột / đường chéo)
// Bạn có thể bắt đầu từ bất kỳ phần tử nào trong hàng đầu tiên
// 4 6
// 10 10  2  0 20  4
//  1  0  0 30  2  5
//  0 10  4  0  2  0
//  1  0  2 20  0  4

// Hướng giải: < -- VỚI NHỮNG BÀI TÌM ĐƯỜNG ĐI KIỂU NÀY THÌ SẼ ĐỂ Ý DÙNG KIỂU KIỂU PREFIX SUM -- >
// Khi một phần tử đi xuống theo 3 hướng <=> đồng nghĩa việc 1 phần tử phía dưới có thể được chọn từ 1 trong 3 hướng ở trên nó
// Ví dụ ở hàng thứ 2, phần tử đầu tiên: A[1][0] thì sẽ có thể + với 2 phần tử A[0][0] và A[0][1]
//                      phần tử thứ 2:   A[1][1] thì sẽ có thể + với 3 phần tử A[0][0] và A[0][1] và A[0][2]
// Vậy bắt đầu xét từ hàng thứ 2 -> n-1 thì tại mỗi phần tử,sẽ lựa chọn cách + với phần tử nào để có path lớn nhất đến thời điểm xét nó -> prefix sum
// KẾT QUẢ: path lớn nhất sẽ là phần tử lớn nhất ở hàng cuối cùng sau khi đã cộng xong

int n,m;
int a[100][100];
vector<ll> path[100];  // path[0][v] -> path của phần tử đầu tiên hàng cuối cùng
                        // path[1][v] -> path của phần tử thứ 2 hàng cuối cùng

void solve(){
    // Input
    cin >> n >> m;
    for(int r = 0; r < n; r++){
        for(int c = 0; c < m; c++){
            cin >> a[r][c];
        }
    }
    
    // Xét từ hàng thứ 2 -> cuối
    for(int r = 1; r < n; r++){
        for(int c = 0; c < m; c++){
            if (c == 0) {   // - Phần tử đầu tiên của hàng
                ll maxi = max(a[r-1][c], a[r-1][c+1]);
                path[c].push_back(maxi);
                a[r][c] += maxi;
            } else if (c == m-1) {  // - Phần tử cuối của hàng
                ll maxi = max(a[r-1][c-1], a[r-1][c]);
                path[c].push_back(maxi);
                a[r][c] += maxi;
            } else {
                ll maxi = max(a[r-1][c-1],max(a[r-1][c], a[r-1][c+1]));
                path[c].push_back(maxi);
                a[r][c] += maxi;
            }
        }
    }
    for (int i = 0; i < m; i++){
        path[i].push_back(a[n-1][i]);
    }

    // Output
    ll maxEle = LONG_MIN;
    ll maxIndex = 0;
    for(int c = 0; c < m; c++){
        if (a[n-1][c] > maxEle) {
            maxIndex = c;
            maxEle = a[n-1][c];
        }
    }
    cout << "Maximum Path Sum: " << maxEle << endl;
    
    int res[n];
    res[0] = path[maxIndex][0];
    for(int i = 1; i < n; i++){
        res[i] = path[maxIndex][i] - path[maxIndex][i-1];
    }
    cout << "Path corresponding: ";
    for(int i = 0; i < n; i++){
        cout << res[i] << ' ';
    }
}

int main(){
    solve();
    return 0;
}