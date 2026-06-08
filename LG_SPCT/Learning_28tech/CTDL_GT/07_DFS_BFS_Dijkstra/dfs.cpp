#include <bits/stdc++.h>
using namespace std;

// DFS : Depth First Search
// Thuật toán tìm kiếm theo chiều sâu: Ưu tiên duyệt xuống sâu nhất có thể trong 1 nhánh trước rồi mới quay lại
// -------> SỬ DỤNG CHO ĐỒ THỊ KHÔNG CÓ TRỌNG SỐ

// DFS(u){
//     < Thẩm định u >
//     visited[u] = true;          // Đánh dấu u đã được ghé thăm
//     // Duyệt các đỉnh kề với đỉnh u
//     for (v : adj[u]) {
//         if (!visited[v]) {  // Nếu v chưa được thăm
//             DFS (v)
//         }
//     }
// }

// 1 --- 2 --- 4                                1
//  |--- 3 --- 6
//  |     |---- 7
//  |     |---- 9 --|
//  |--- 5 --- 8 ---|

// ------> DFS: 1 2 4 3 6 7 9 8 5

// Input (đỉnh + cạnh / nếu là đồ thị có hướng thì là đầu -> đích)
// 9 9
// 1 2
// 1 3
// 1 5
// 2 4
// 3 6
// 3 7
// 3 9
// 5 8
// 8 9

int n,m;
vector<int> adj[1001];
bool visited[1001];

void inp(){
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        int x,  y; cin >> x >> y;   // đỉnh + Cạnh
        adj[x].push_back(y);
        adj[y].push_back(x);    // Vì đồ thị vô hướng cho nên sẽ xét cả chiều ngược lại, còn nếu đồ thị có hướng chỉ xét chiều xuôi thì không cần dòng code này
    }
    memset(visited, false, sizeof(visited));
}

void dfs(int u){
    cout << u << " ";
    visited[u] = true;
    for(int x : adj[u]) {
        if (!visited[x]){
            dfs(x);
        }
    }
}

int main(){
    inp();
    dfs(1);
    return 0;
}