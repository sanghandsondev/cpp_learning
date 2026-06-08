// https://www.acmicpc.net/problem/17298
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

// Next Greater Element (NGE): Phần tử lớn hơn gần nhất bên phải

int n;
vector<int> A;

int main() {
    cin >> n;
    A = vector<int>(n);
    for(int i = 0; i < n; i++){
        cin >> A[i];
    }
    
    stack<int> st;              // Store index
    vector<int> NGE(n, -1);     // Store value

    for(int i = 0; i < n; i++){
        while(!st.empty() && A[st.top()] < A[i]){
            NGE[st.top()] = A[i];
            st.pop();
        }
        st.push(i);
        
    }
    for(int i = 0; i < n; i++){
        cout << NGE[i] << " ";
    }
    
    return 0;
}