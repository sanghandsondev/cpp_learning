#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Thuật toán Dijkstra: Tìm đường đi ngắn nhất từ 1 đỉnh S tới mọi đỉnh còn lại trên đồ thị (có trọng số)
// Start ---> ... ---> u ---> v
// CÔNG THỨC: d[v] = min(d[v], d[u] + len(u,v))
// -------- Step coding ---------
// Step1: Trừ đỉnh Start(1) thì các đỉnh còn lại được đánh dấu là infinity: d[1] = 0, d[2] = inf, d[3] = inf, d[4] = inf, ...
// Step2: Duyệt tới các đỉnh kề với đỉnh 1 theo CÔNG THỨC: xác định được d[2], d[3] với trường hợp đỉnh 2, 3 kề với đỉnh 1
// Step3: Xét d[i] của các đỉnh ngoại trừ đỉnh 1 để tìm được điểm đi tiếp theo, giả sử d[2] < d[3] thì sẽ chọn đỉnh 2 là đỉnh kế tiếp
// Step4: Tiếp tục duyệt các đỉnh kề với đỉnh 2 theo CÔNG THỨC: 
// Step5: Xét d[i] của các đỉnh ngoại trừ đỉnh 1 và 2 để tìm điểm đi tiếp theo, giả sử d[5] bé nhất thì sẽ chọn đỉnh 5 là đỉnh kế tiếp
// Step6: Lặp lại step4 -> step5 
// Cuối cùng chúng ta sẽ có một dãy d[] với d[i] là chi phí nhỏ nhất để đi từ đỉnh 1 đến đỉnh i
// ----------- INPUT: 
// 6 8 1 5
// 1 2 7
// 1 3 12
// 2 3 2
// 2 4 9
// 3 5 10
// 4 6 1
// 5 4 4
// 5 6 5
// ----------- OUTPUT: 0 7 9 16 19 17 (lần lượt là chi phí bé nhất từ đỉnh đầu tiên đến các đỉnh thứ i)

const int maxn = 100001;
const int INF = 1e9;
int n, m, s, e;    // số đỉnh, số cạnh, đỉnh bắt đầu, đỉnh kết thúc
vector <pair<int, int>> adj[maxn];
int pre[maxn];

void input(){
    cin >> n >> m >> s >> e;              // số đỉnh, số cạnh, đỉnh bắt đầu, đỉnh kết thúc
    for(int i = 0 ; i < m; i++){
        int x, y, w;
        cin >> x >> y >> w;     // đỉnh >> cạnh >> trọng số     (trọng số ở đây chính là chi phí đi lại giữa 2 điểm hoặc là độ dài của cạnh)
        adj[x].push_back({y,w});    // đồ thị có hướng
        // adj[y].push_back({x,w});    nếu đồ thị vô hướng thì thêm dòng code này
    }
}

void dijkstra(int start, int end){
    vector<ll> d(n+1, INF);
    d[start] = 0;       // đỉnh start
    vector<bool> visited(n+1, false);
    pre[start] = start;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> Q;     // --> sort theo thằng có khoảng cách nhỏ nhất
    // {Khoảng cách, đỉnh}
    Q.push({0, start});
    while(!Q.empty()){
        // Chọn ra đỉnh có khoảng cách từ Start là nhỏ nhất
        pair<int, int> top = Q.top();
        Q.pop();
        int u_point = top.second;
        int distance_s_u = top.first;
        // Marked point
        if (visited[u_point]) continue;     // Method2: if(distance_s_u > d[u_point]) continue;
        // Unmarked point ---> Relaxation: Cập nhật khoảng cách từ Start cho tới mọi đỉnh kề với đỉnh 'u_point'
        visited[u_point] = true;
        for(auto it : adj[u_point]) {
            int v_point = it.first;
            int distance_u_v = it.second;
            if (d[v_point] > d[u_point] + distance_u_v) {
                d[v_point] = d[u_point] + distance_u_v;
                Q.push({d[v_point], v_point});
                pre[v_point] = u_point;
            }
        }
    }
    cout << "Chi phi di lai nho nhat tu dinh Start cho den cac dinh con lai (1->n): ";
    for (int i = 1; i <= n; i++){
        cout << d[i] << ' ';
    }
    cout << endl;
    cout << "Chi phi nho nhat di lai tu dinh Start den dinh End: " << d[end] << endl;
    // ------ Tìm đường đi có chi phí nhỏ nhất từ đỉnh Start đến đỉnh End --------
    vector<int> path;
    while (1)
    {
        path.push_back(end);
        if (end == start) break;
        end = pre[end];
    }
    cout << "Duong di co chi phi nho nhat tuong tung tu dinh Start den dinh End: ";
    for(auto it = path.rbegin(); it != path.rend(); it++){
        cout << *it << ' ';
    }
    // reverse(path.begin(), path.end());
    // for (auto x : path){
    //     cout << x << ' ';
    // }
    
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    input();
    dijkstra(s,e);
    return 0;
}