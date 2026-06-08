// https://www.acmicpc.net/problem/2839

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

// 11kg = 3kg * 2 + 5kg * 1
// 18kg = 3kg * 1 + 5kg * 3

// ----> THUẬT TOÁN THAM LAM: Ưu tiên thằng túi 5kg
    
int n;

int solve(){
    int cnt3 = 0, cnt5 = 0;
    int temp = n % 5;
    if (temp == 0) {            // chia het cho 5
        cnt5 = n / 5;
        cnt3 = 0;
    } else if (temp == 1) {     // chia 5 du 1
        n = n - 6;
        cnt3 += 2;
        cnt5 = n / 5;
    } else if (temp == 2) {     // chia 5 du 2
        if (n >= 12) {
            cnt3 = 4;
            n = n - 12;
            cnt5 = n / 5;
        }
    } else if (temp == 3) {     // chia 5 du 3
        n = n - 3;
        cnt3++;
        cnt5 = n / 5;
    } else {                    // chia 5 du 4
        if (n >= 9) {
            cnt3 = 3;
            n = n - 9;
            cnt5 = n / 5;
        }
    }
    if (cnt3 + cnt5 == 0) return -1;
    else return cnt3 + cnt5;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    cin >> n;
    cout << solve();
    
    return 0;
}