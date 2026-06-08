// https://www.acmicpc.net/problem/9935
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// Example: Explosive string
// [12ab]:  '12ab'112ab2ab -> 1'12ab'2ab -> '12ab' -> FRULA
//  0123     0123 00123123    0 0123 123 
    
// [C4]:    m i r k o v C 4  n i z C C 4 4 -> mirkovniz'C4' -> mirkovniz
//  01     -1-1-1-1-1-1 0 1 -1-1-1 0 0 1 1
string s, expl_s;

int main(){
    cin >> s >> expl_s;
    int len = s.length();
    int len2 = expl_s.length();
    vector<pair<char,int>> st;       // Stack <character, position in ExplosiveString>
    
    for(int i = 0; i < len; i++){
        if (st.empty()) {
            int pos = -1;
            if (s[i] == expl_s[0]) pos = 0;
            if(pos+1 < len2) {      // Corner case: ExplosiveString chỉ là 1 kí tự thì không cần phải push
                st.push_back({s[i], pos});
            }
        } else {
            pair<char,int> temp = st.back();
            char ch = temp.first;
            int pos = temp.second;
            int next_pos = -1;
            if (pos >= 0) {
                // Case 1: Kí tự trước đó đang nằm trong chuỗi Explosive
                next_pos = pos + 1;
                if (s[i] == expl_s[next_pos]) {
                    // 1.1: Nếu kí tự đang xét chính là kí tự tiếp theo của chuỗi Explosive
                    if(next_pos+1 == len2){
                        // 1.1.1: Nếu hoàn thiện chuỗi Explosive -> Pop out
                        for(int k = 1; k <= len2 -1; k++){
                            st.pop_back();
                        }
                    } else if (next_pos+1 < len2){
                        // 1.1.2: Chưa hoàn thành chuỗi Explosive
                        st.push_back({s[i], next_pos});
                    }
                } else {
                    // 1.2: Bắt đầu một chuỗi Explo mới nếu có thể
                    if (s[i] == expl_s[0]) next_pos = 0;
                        else next_pos = -1;
                    if (next_pos+1 < len2){             // Corner case: ExplosiveString chỉ là 1 kí tự thì không cần phải push
                        st.push_back({s[i], next_pos});
                    }
                }
            } else {
                // Case 2: Kí tự trước đó không nằm trong chuỗi Explosive -> bắt đầu một chuỗi Explosive mới nếu có thể
                if (s[i] == expl_s[0]) next_pos = 0;
                    else next_pos = -1;
                if (next_pos+1 < len2) {                // Corner case: ExplosiveString chỉ là 1 kí tự thì không cần phải push
                    st.push_back({s[i], next_pos});
                }            
            }
        }
    }
    if (st.empty()) cout << "FRULA" << endl;
    else {
        for(auto x : st){
            cout << x.first;
        }
    }
    
    return 0;
}