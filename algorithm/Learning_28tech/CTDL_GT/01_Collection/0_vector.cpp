#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// push_back(), pop_back()
// size(), clear(), empty()

// begin(), end(), rbegin(), rend()
// front(), back()

int n, m;
vector<int> vec;
vector<vector<int>> vector2D; // Khởi tạo vector 2 chiều


int main(){
    cin >> n >> m;
    vec.assign(n, 0); // Khởi tạo vector 'vec' với n phần tử, mỗi phần tử có giá trị là 0
    vector2D.assign(m, vector<int>(n, 0));  // m hàng và n cột, mỗi phần tử có giá trị là 0
    // Tương đương với vector<vector<int>> vector2D(m, vector<int>(n, 0));

    // vector<int> v;
    vector<int> v(3, 1); // {1, 1, 1}
    v.push_back(2);
    v.push_back(3);
    v.pop_back();
    //--------Loop with index ----------------------
    cout << v.size() << endl;               // 4
    for (int i = 0; i < v.size(); i++){
        cout << v[i] << ' ';                // 1 1 1 2
    }
    cout << endl;
    for (int i = v.size()-1; i >= 0; i--){  // 2 1 1 1
        cout << v[i] << ' ';
    }
    cout << endl << "----\n";
    //------- Range base for loop ------------------
    for (auto &x : v){           // gán x = chính các phần tử trong vector v => x thay đổi thì các phần tử trong v cũng thay đổi
        x = 5;
        cout << x << ' ';
    }
    cout << endl;
    for (auto x : v){           // gán x = giá trị từng phần tử trong vector v => không thay đổi giá trị các phần tử trong v
        x = 4;
        cout << x << ' ';
    }
    cout << endl << "----\n";
    //--------- Iterator -------------------------------------
    vector <int> v1 = {1, 2, 3, 4, 5};
    // [rend][1-begin][2][3][4][5-rbegin][end]

    vector<int>::iterator it = v1.begin() + 2;  // index [0+2] -> v[2]
    cout << *it << endl;

    for (it = v1.begin(); it != v1.end(); it++){
        cout << *it << ' ';             // 1 1 1 2
    }
    cout << endl;
    for (auto it2 = v1.begin(); it2 != v1.end(); it2++){
        cout << *it2 << ' ';            // 1 1 1 2
    }
    cout << endl << "----\n";
    //--------------- Do phuc tap: O(n) --------------------------------
    v1.insert(v1.begin()+ 2, 100);    // Chen` vao vi tri v[2] - phan tu thu 3
    for (auto x : v1) cout << x << ' ';
    cout << "\n";

    v1.erase(v1.begin()+ 2);          // Xoa phan tu v[2] - phan tu thu 3 
    for (auto x : v1) cout << x << ' ';

    return 0;
}