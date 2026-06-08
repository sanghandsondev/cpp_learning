#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// Cho ma trận vuông A[N][N], nhiệm vụ của bạn là đưa ra số các phần tử xuất hiện ở tất cả các hàng
// Ví dụ dưới đây cho ta kết quả là 2 (tương ứng với số 2 và 3 khi chúng xuất hiện ở tất cả các hàng)
// 1
// 4
// 2 3 2 3
// 1 2 3 2
// 2 6 2 3
// 5 2 5 3

int tc, n;
int a[105][105];

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> tc;
    while(tc--){
        cin >> n;
        for (int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                cin >> a[r][c];
            }
        }
        map<int, int> mp;
        // Duyệt qua các phần tử của hàng đầu tiên, thằng nào xuất hiện thì cho mp = 1
        for(int c = 0; c < n; c++){
            mp[a[0][c]] = 1;
        }
        // Duyệt các hàng còn lại
        for(int r = 1; r < n; r++){
            for(int c = 0; c < n; c++){
                if (mp[a[r][c]] == r) {     // Ví dụ tại thời điểm hàng thứ 3 (r = 2), nếu mp của số 2 bằng r (=2) thì có nghĩa 
                    mp[a[r][c]]++;          // trước đó số 2 vẫn đang thỏa mãn điều kiện 
                }
            }
        }
        // Sau khi hoàn tất việc quét các phần tử thì các giá trị thỏa mãn là các giá trị có map bằng N
        int cnt = 0;
        vector<int> res;
        for(auto it : mp){
            if (it.second == n){
                res.push_back(it.first);
                cnt++;
            }
        }
        cout << "So cac phan tu duoc xuat hien o tat ca cac hang`: " << cnt << " --> ";
        for(auto it : res){
            cout << it << " ";
        }
    }
    return 0;
}