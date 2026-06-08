#include <bits/stdc++.h>
using namespace std;

// Tìm cách sắp xếp N quân hậu vào N hàng trên bàng cờ vua N * N sao cho không có 2 quân hậu nào ăn nhau
// Gọi X = (X1, X2, ... Xn) là một nghiệm của bài toán, khi đó nếu Xi = j thì có nghĩa ta xếp quân hậu hàng thứ i nằm ở cột j

// Note: Vì các con hậu trên mỗi hàng khác nhau nên mình sẽ cần quản lý đường chéo xuôi, đường chéo ngược và cột dọc của một con hậu (i,j)
// Các đường chéo xuôi được đánh chỉ số từ 1 -> 2n - 1
// Các đường chéo ngược đc đánh chỉ số từ 1 -> 2n - 1
// Ô (i,j) bị đường chéo xuôi "i-j+n" và đường chéo ngược "i+j-1" quản lý

int N, X[100], column[100], d1[100], d2[100];
int a[100][100];
int cnt;

void show(){
    // for (int i = 1; i <= N; i++) {
    //     cout << "Queen " << i << "th is on " << X[i] << "column\n";
    // }
    memset(a, 0, sizeof(a));
    for (int i = 1; i <= N; i++){
        a[i][X[i]] = 1;
    }
    for (int i = 1; i <= N; i++){
        for (int j = 1; j <= N; j++){
            cout << a[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void Try(int i){
    // Duyệt tất cả các cột mà Queen có thể được đặt trên một hàng
    for (int j = 1; j <= N; j++) {
        if (column[j] == 1 && d1[i - j + N] == 1 && d2[i+j-1] == 1){    // Điều kiện thỏa mãn rằng ô (i,j) đó chưa bị con Hậu nào quản lý - Status
            X[i] = j;
            column[j] = d1[i - j + N] = d2[i + j - 1] = 0;      // Status 1 -> 0
            if (i == N) {
                cnt++;
                show();
            } else {
                Try(i+1);
            }
            // Backtrack: Trả lại status trước khi xét X[i] = j hiện tại để xét thằng j trong vòng lặp mới
            column[j] = d1[i - j + N] = d2[i + j - 1] = 1;      // Status 0 -> 1
        }
    }
}

int main(){
    N = 5;
    cnt = 0;
    for (int i = 1; i <= 99; i++){
        column[i] = d1[i] = d2[i] = 1;
    }
    Try(1);
    cout << "So nghiem thoa man: " << cnt << endl;
    return 0;
}