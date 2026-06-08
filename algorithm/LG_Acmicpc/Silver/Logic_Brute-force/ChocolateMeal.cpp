// https://www.acmicpc.net/problem/2885

#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long
#define mod 1000000007

int k;

// Convert integer into binary number
string tobinary(int n){
    if (n == 0) return "0";
    string res;
    while(n > 0) {
        res.insert(res.begin(), '0' + (n % 2));
        n /= 2;
    }
    return res;
}
// string decToBinary(int n) {
//     string bin = "";
	
//     while (n > 0) {
// 		int bit = n % 2;
//          bin.push_back('0' + bit);
//          n /= 2;
//     }
  
// 	reverse(bin.begin(), bin.end());
//     return bin;
// }

void solve(){
    cin >> k;
    int size = 1;
    int powerOfSize = 0;
    int cut = 0;
    while(size < k){
        size*=2;
        powerOfSize++;
    }
    // 15 = 8 + 4 + 2 + 1   -> minPeace: 1 (2^0): (16/2/2/2/2) -> 4 lần cắt (2^4 -> 2^0)
    // 14 = 8 + 4 + 2       -> minPeace: 2 (2^1): (16/2/2/2)   -> 3 lần cắt (2^4 -> 2^1)
    // 13 = 8 + 4 + 1       -> minPeace: 1 (2^0): (16/2/2/2/2) -> 4 lần cắt (2^4 -> 2^0)
    // 12 = 8 + 4           -> minPeace: 4 (2^2): (16/2/2)     -> 2 lần cắt (2^4 -> 2^2)
    // --- Vì vậy cần tìm vị trí số 1 cuối cùng trong số nhị phân của K thì sẽ xác định được số lần cắt ---
    string bin = tobinary(k);   // k = 14 = 8 + 4 + 2    -> 1110
    int len = bin.length();
    int dm = 0;
    for (int i = len - 1; i >= 0; i--){
        if (bin[i] == '1') break;
        dm++;
    }
    // ---[ minPeace = 2^dm ] ---
    cut = powerOfSize - dm;     
    cout << size << " " << cut;
}

int main(){
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve();
    return 0;
}