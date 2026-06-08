#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define ull long long

// ** NOTE: Stack thường ứng dụng vào các bài toán liên quan đến sự cân đối của mảng
// ** NOTE: Ứng dụng thứ 2 đó là khi duyệt sẽ lưu các phần tử trước dó vào stack, đến khi phần tử hiện tại thỏa mãn điều kiện hoặc trigger gì đó
            // --> Thực hiện 1 vòng lặp xét ngược lại các giá trị đã lưu vào stack để tính toán, sau đó pop chúng ra. Rồi tiếp tục duyệt đến cuối mảng

// LIFO: Last in first out
// push, pop, top, size, empty
// ------------------ Bài toán: kiểm tra dấu ngoặc hợp lệ -----------------
// Ví dụ: (()) ,  ()()   --> thỏa mãn
//        ((()) ,  ()()) --> ko thỏa mãn; 
// Hướng dẫn: Khi là '(' thì push vào stack, khi là ')' thì kiểm tra và pop nếu thỏa mãn

bool solve(string s){
    stack<char> st;
    for (char x : s){
        if (x == '('){
            st.push(x);
        } else {    // ')'
            if (st.empty()){
                return false;   // stack mà rỗng chứng tỏ trước đó k có kí tự '(' thừa, mà có kí ')' xuất hiện -> false
            } else 
                st.pop();
        }
    }
    if (st.empty())
        return 1;
    else
        return 0;       // trường hợp vẫn còn kí tự '(' ở trong stack -> false
}

int main(){
    string s1 = "(()())";       // valid
    string s2 = "(()()))";      // invalid
    if (solve(s1)) cout << "VALID\n"; else cout << "INVALID\n";
    if (solve(s2)) cout << "VALID\n"; else cout << "INVALID\n";
    
    return 0;
}