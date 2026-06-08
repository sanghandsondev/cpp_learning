// https://www.acmicpc.net/problem/1969

// -- THAM LAM: Xét từng ký tự của các DNA, chọn ưu tiên thằng xuất hiện nhiều nhất ---> Tổng distance với các DNA ở từng kí tự là nhỏ nhất

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int n, m;
string a[1001];

void solve(){
    cin >> n >> m;
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }
    int minDis = 0;
    string minDNA = "";
    for(int k = 0; k < m; k++){
        int cnt[4] = {0};
        string tmp_char = "ACGT";
        for(int i = 0; i < n; i++){
            if (a[i][k] == 'A') {
                cnt[0]++;
            } else if (a[i][k] == 'C') {
                cnt[1]++;
            } else if (a[i][k] == 'G') {
                cnt[2]++;
            } else if (a[i][k] == 'T') {
                cnt[3]++;
            }
        }
        // cout << cnt[0] << cnt[1] << cnt[2] << cnt[3] << endl;
        int maxCnt = cnt[0];
        char maxChar = 'A';
        for(int i = 0; i < 4; i++){
            if (maxCnt < cnt[i]) {
                maxCnt = cnt[i];
                maxChar = tmp_char[i];
            }
        }
        minDis += (cnt[0] + cnt[1] + cnt[2] + cnt[3] - maxCnt);
        minDNA += maxChar;
    }
    cout << minDNA << endl << minDis;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);    
    solve();
    return 0;
}