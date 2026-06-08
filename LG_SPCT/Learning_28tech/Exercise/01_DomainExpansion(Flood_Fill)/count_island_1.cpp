// Bài toán: Ma trận [N, M] : N, M <= 50
// Đếm số miền các số 1 trong ma trận (được coi là cùng miền nếu chúng có chung cạnh)
// Input:
// 4 7
// 0 1 1 0 1 1 0
// 1 1 1 1 1 1 1
// 1 0 0 0 0 1 1
// 1 1 0 1 0 0 0

#include <iostream>
using namespace std;

#define ull long long
#define mod 1000000007

// 4 điểm cạnh xung quanh
int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};

int n,m;
int a[51][51];

void input(){
    cin >> n >> m;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cin >> a[i][j];
        }
    }
}
// ----- Sử dụng DFS. Nguyên tắc đệ quy: luôn có điểm dừng ---------
void dfs(int i, int j){
    // Đánh dấu là điểm này đã đi qua (đã xét). NOTE: mỗi điểm 1 chỉ có thể thuộc 1 miền duy nhất
    a[i][j] = 0;
    for (int k = 0; k < 4; k++){
        int i1 = i + dx[k];
        int j1 = j + dy[k];
        if (i1 >= 0 && i1 < n && j1 >= 0 && j1 < m && a[i1][j1] == 1) {
            dfs(i1, j1);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    input();
    int count = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            // Xét thấy điểm hiện tại là 1 -> loang ra xung quanh để gán các điểm chung miền với điểm hiện tại = 0
            if (a[i][j] == 1) {
                count++;
                dfs(i,j);
            }
        }
    }
    cout << "Number of island 1: " << count;
    
    return 0;
}