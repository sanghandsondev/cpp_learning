#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// -- Set là một container mà (mỗi phần tử trong đó là duy nhất) + (Tự động sắp xếp các phần tử trong theo thứ tự tăng dần)
// ** Set không có chỉ số index do không phải dạng tuyến tính như mảng và vector ---> nó là dạng tree

// insert(i): thêm phần tử, nếu chưa tồn tại thì tự động sắp xếp
// clear(): xóa tất cả phần tử
// empty(): 
// size(): 
// find(i): tìm ra vị trí iterator của phần tử có giá trị = i
// count(i): tìm ra số lần xuất hiện của phần tử có giá trị = i

// erase(i): xóa phần tử có giá trị = i
// erase(it): xóa phần tử ở vị trí iterator = it


int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    set<int> se;
    se.insert(2);   // O(logN)
    se.insert(3);
    se.insert(2);
    se.insert(1);   // 1 2 3 
    cout << se.size() << endl;

    for (auto x : se){
        cout << x << ' ';
    }
    cout << endl;
    // ----- Iterator, nhưng không thể sử dụng it += 1 ------------
    for (auto it = se.begin(); it != se.end(); it++){
        cout << *it << ' ';
    }
    cout << endl;
    // ------ find() Tim kiem phan tu trong Set ----------
    // ------ erase() Xoa phan tu trong Set --------------
    if (se.find(2) != se.end()) {
        cout << "FOUND 2 -> ERASE 2\n"; 
        se.erase(2);
        for (auto x : se){
            cout << x << ' ';
        }
    }
    else cout << "NOT FOUND 2\n";

    auto it = se.find(4);
    if (it != se.end()) se.erase(it);

    return 0;
}