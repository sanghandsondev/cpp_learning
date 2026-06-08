// https://www.acmicpc.net/problem/2493
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// Previous Greater Element (PGE): Phần tử lớn hơn gần nhất bên trái
int n;
vector<int> Tower;

int main() {
    cin >> n;
    Tower = vector<int> (n);
    for(int i = 0; i < n; i++){
        cin >> Tower[i];
    }

    stack<int> st;          // store index
    vector<int> PGE(n, 0);  // store index+1

    for(int i = n-1; i >= 0; i--){
        while(!st.empty() && Tower[st.top()] < Tower[i]){
                PGE[st.top()] = i + 1;
                st.pop();
        }
        st.push(i);
    }
    for(int i = 0; i < n; i++){
        cout << PGE[i] << " ";
    }
    
    return 0;
}