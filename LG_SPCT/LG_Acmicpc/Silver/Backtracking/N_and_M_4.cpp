// https://www.acmicpc.net/problem/15649
// Given natural numbers N and M, write a program to find all sequences of length M that satisfy the conditions below.

// A sequence of M natural numbers selected from 1 to N
// You can choose the same number multiple times.
// The sequence must be in non-descending order.
// If a sequence A of length K satisfies A 1 ≤ A 2 ≤ ... ≤ A K-1 ≤ A K , it is said to be in non-descending order.

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, m;
int X[10];
// bool visited[10];
ostringstream output;   // BỞI VÌ OUTPUT khi N = M = 8 là quá nhiều hoán vị, cho nên cần sử dụng ostringstream để in 1 lần duy nhất

void backtrack(int count){
    for(int i = 1; i <= n; i++){
        // if (!visited[i]) {
        if (i >= X[count-1]) {
            // ---
            X[count] = i;
            // visited[i] = true;
            if (count == m) {
                for(int j = 1; j <= m; j++){
                    output << X[j] << " ";
                }
                output << endl;
            } else {
                backtrack(count + 1);
            }
            // ---
            // visited[i] = false;
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