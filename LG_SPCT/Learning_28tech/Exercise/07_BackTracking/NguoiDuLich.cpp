#include <bits/stdc++.h>
using namespace std;

// Cho n thành phố đánh số từ 1 -> n và các đoạn đường 2 chiều giữa chúng, chi phí đi lại giữa các thành phố này được cho bởi mảng 2 chiều C[][] 
// trong đó C[i][j] = C[j][i] là chi phí đi từ thành phố i -> j
// Chi phí giữa thành phố thứ i với chính nó là 0
// Một người du lịch xuất phát từ thành phố 1, muốn đi thăm tất cả các thành phố còn lại (mỗi thành phố đúng 1 lần) và cuối cùng quay lại thành phố 1
// Hãy chỉ ra cách đi sao cho chi phí đi lại là nhỏ nhất.
// 4
// 0 85 26 81
// 85 0 77 97
// 26 77 0 26
// 81 97 26 0

int n;
int c[100][100];      // Chi phí đi từ thành phố i -> j
int x[100];          // Lưu trữ đường đi
int visited[100];    // Đánh dấu các thành phố đã đi qua
int min_cost = INT_MAX;   // Chi phí nhỏ nhất

void Try(int i){
    for (int j = 1; j <= n; j++){
        if (visited[j] == 0){   // Nếu thành phố j chưa đi qua
            x[i] = j;           // Đánh dấu thành phố j đã đi qua
            visited[j] = 1;     // Đánh dấu thành phố j đã đi qua

            if (i == n){        // Nếu đã đi qua tất cả các thành phố
                int cost = 0;   // Tính chi phí hiện tại
                for (int k = 1; k <= n; k++){
                    cost += c[x[k]][x[k+1]];   // Tính chi phí từ thành phố k -> k+1
                    cout << x[i] << " -> ";
                }
                cost += c[x[n]][x[1]];         // + Tính chi phí từ thành phố n -> 1
                cout << "1 => ";
                cout << "Cost for this way: " << cost << endl;
                // --------- Cập nhật chi phí nhỏ nhất -------------
                min_cost = min(min_cost, cost);   
            } else {
                Try(i + 1);
            }
            
            visited[j] = 0;     // Trả lại trạng thái chưa đi qua cho thành phố j
        }
    }
}

int main(){
    cin >> n;
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            cin >> c[i][j];
        }
    }

    memset(visited, 0, sizeof(visited));   // Đánh dấu tất cả các thành phố chưa đi qua
    visited[1] = 1;      // Đánh dấu thành phố 1 đã đi qua
    x[1] = 1;           // Bắt đầu từ thành phố 1
    int cost = 0;       // Chi phí hiện tại
    for (int i = 2; i <= n; i++){
        x[i] = 0;       // Chưa đi qua thành phố nào khác
    }

    Try(2);   // Bắt đầu từ thành phố 2
    cout << "-> Min Cost: " << min_cost << endl;

    return 0;
}