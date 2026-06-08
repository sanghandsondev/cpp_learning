// https://www.acmicpc.net/problem/14889

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n;
int a[21][21];

// Convert integer into binary number 
string decToBinary(int n) {
    string bin = "";
    while (n > 0) {
		int bit = n % 2;
        bin.push_back('0' + bit);
        n /= 2;
    }
  
	reverse(bin.begin(), bin.end());    // (can use stack for reversing the order)
    return bin;
}

// Calculate the abilities of each team
int ab(vector<int> &team){
    int sumx = 0;
    int len = team.size();
    if (len == 0 || len == 1) return 0;
    for(int i = 0; i < len - 1; i++){
        for(int j = i + 1; j < len; j++){
            sumx = sumx + a[team[i]][team[j]] + a[team[j]][team[i]];
        }
    }
    return sumx;
}

// Calculate difference between the abilities of 2 teams
int distanceX(int a, int b){
    if (a >= b) return a - b;
    return b - a;
}

void solve(){
    cin >> n;
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            cin >> a[i][j];
        }
    }
    int res = INT_MAX;
    int combine = 1;
    // 1. Combine case : 0 -> 2^n - 1 case   (with n = 4: 0000 -> 1111)
    for(int i = 1; i <= n; i++) combine *= 2;
    for(int mask = 0; mask < combine; mask++) {
        string tmp = decToBinary(mask);
        int check = 0;
        for(auto c : tmp) {
            if (c == '1') check++;
        }
        if (check != n/2) continue;
        // 2. if in case split the people evenly into 2 teams
        vector<int> link, start;
        vector<bool> check2(n+1, true);
        for (int i = tmp.length() - 1; i >= 0; i--){
            if(tmp[i] == '1') {
                link.push_back(i+1);
                check2[i+1] = false;
            }
        }
        for(int i = 1; i <= n; i++) {
            if (check2[i]) start.push_back(i);
        }
        // for(auto it : link ) cout << it << " ";
        // cout << endl;
        // for(auto it : start ) cout << it << " ";
        // cout << endl;
        int abi1 = ab(link);
        int abi2 = ab(start);
        int dis = distanceX(abi1, abi2);
        res = min(res, dis);
        // cout << abi1 << " " << abi2 << " " << dis << " " << res << endl;
    }
    cout << res;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}