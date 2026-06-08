// https://www.acmicpc.net/problem/15649
// BÀI TOÁN 1: Liệt kê Dãy con ko lặp có độ dài M trong khoảng từ 1 -> N

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, m;
int X[10];
bool visited[10];
ostringstream output;   // BỞI VÌ OUTPUT khi N = M = 8 là quá nhiều hoán vị, cho nên cần sử dụng ostringstream để in 1 lần duy nhất

void backtrack(int count){
    for(int i = 1; i <= n; i++){
        if (!visited[i]) {
            // ---
            X[count] = i;
            visited[i] = true;
            if (count == m) {
                for(int j = 1; j <= m; j++){
                    output << X[j] << " ";
                }
                output << endl;
            } else {
                backtrack(count + 1);
            }
            // ---
            visited[i] = false;
        }
    }
}

void solve(){
    cin >> n >> m;
    backtrack(1);
    cout << output.str();
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}