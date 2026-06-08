//https://www.acmicpc.net/problem/14502

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int dy[4] = {-1, 1, 0, 0};
const int dx[4] = {0, 0, -1, 1};

int N, M;
int lab[8][8];
int temp[8][8];
vector<pair<int, int>> emptyCells, virusCells;      // store position of EmptyCell and VirusCell due to small number of them
int maxSafeArea = 0;

void spreadVirus() {        // Floodfill algorithm --> BFS 
    queue<pair<int, int>> q;
    int copied[8][8];

    // Copy current state to temp
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            copied[i][j] = temp[i][j];

    // Add initial virus locations
    for (auto it : virusCells) q.push({it.first, it.second});
    
    while (!q.empty()) {
        pair<int,int> tmp = q.front(); q.pop();
        int y = tmp.first;
        int x = tmp.second;
        for (int d = 0; d < 4; ++d) {
            int ny = y + dy[d], nx = x + dx[d];
            if (ny >= 0 && ny < N && nx >= 0 && nx < M && copied[ny][nx] == 0) {
                copied[ny][nx] = 2;
                q.push({ny, nx});
            }
        }
    }

    // Count safe area
    int safe = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (copied[i][j] == 0) ++safe;

    maxSafeArea = max(maxSafeArea, safe);
}

// BACKTRACKING KHÔNG TRUY VẾT
void buildWalls(int count, int idx) {
    if (count == 3) {       // --- Case: enough 3 empty_cell selection
        // Copy lab[][] to temp[][] for calculating
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                temp[i][j] = lab[i][j];
        spreadVirus();      //  calculate
        return;
    }

    for (int i = idx; i < emptyCells.size(); ++i) { // Khác với hoán vị thì thằng này giống như tổ hợp
        pair<int,int> tmp = emptyCells[i];
        int y = tmp.first;
        int x = tmp.second;
        lab[y][x] = 1;  // Fill wall
        buildWalls(count + 1, i + 1);
        lab[y][x] = 0;  // Remove wall (backtrack)
    }
}

int main() {
    cin >> N >> M;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            cin >> lab[i][j];
            if (lab[i][j] == 0) emptyCells.push_back({i, j});
            else if (lab[i][j] == 2) virusCells.push_back({i, j});
        }

    buildWalls(0, 0);   // backtracking for search exactly '3' empty_cell (Chọn bất kì 3 ô trống trong số các ô trống có thể - Brute Force)
    cout << maxSafeArea << endl;
    return 0;
}