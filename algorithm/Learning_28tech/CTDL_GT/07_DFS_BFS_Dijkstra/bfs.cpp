#include <bits/stdc++.h>
using namespace std;

// BFS : Breadth First Search
// Thuật toán tìm kiếm theo chiều rộng: Ưu tiên chiều rộng hơn là chiều sâu (các phần tử đồng cấp sẽ được duyệt trước)
// Thuật toán sẽ tìm kiếm xung quanh để mở rộng trước khi đi xuống sâu hơn
// -------> SỬ DỤNG CHO ĐỒ THỊ KHÔNG CÓ TRỌNG SỐ, Chỉ quan tâm số bước (Step)

// BFS(u){
//     // Step 1: Khởi tạo
//     queue               //  Tạo một hàng đợi rỗng
//     push(queue, u);     // Đẩy đỉnh u vào hàng đợi
//     visited[u] = true;  // Đánh dấu đỉnh u đã được thăm
//     // Step 2: Lặp khi mà hàng đợi vẫn còn phần tử
//     while(queue!=empty){
//         v = queue.front();    // Lấy ra đỉnh ở đầu hàng đợi
//         queue.pop();        // Xóa đỉnh
//         < Thăm đỉnh v >;
//         for (int x : ke[v]) {       // Duyệt các đỉnh/cạnh kề với v mà chưa được thăm và đẩy vào queue
//             if (!visited[x]) {
//                 push(queue, x);
//                 visited[x] = true;
//             }
//         }
//     }
// }

// 10 11    (số lượng đỉnh + cạnh)
// 1 2
// 1 3
// 1 5
// 1 10
// 2 4
// 3 6
// 3 7
// 3 9
// 6 7
// 5 8
// 8 9

int n, m;   // số lượng đỉnh + cạnh
vector<int> adj[1001];
bool visited[1001];

void inp(){
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        int x,y; cin >> x >>  y;
        adj[x].push_back(y);
        adj[y].push_back(x);        // Đồ thị Vô hướng, nếu đồ thị có hướng thì bỏ dòng này đi
    }
    memset(visited, false, sizeof(visited));
}

void bfs(int u){
    queue<int> q;
    q.push(u);
    visited[u] = true;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        cout << v << " ";
        for (int x : adj[v]){
            if(!visited[x]){
                q.push(x);
                visited[x] = true;
            }
        }
    }
}

int main(){
    inp();
    bfs(1);
    return 0;
}