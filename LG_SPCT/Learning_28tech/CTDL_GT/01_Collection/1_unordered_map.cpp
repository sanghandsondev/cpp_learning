#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
using ll = long long;

// unordered_map <key, value> -----> key là duy nhất cho mỗi phần tử, tuy nhiên không còn được sắp xếp tăng dần

// begin(), end(), rbegin(), rend()
// size()
// insert({key, value})
// erase(key)
// count(key)
// find(key)

// -- NOTE: ứng dụng để lưu tần suất xuất hiện của một phần tử nào đó trong mảng (1D/2D) / xâu mà không quan tâm THỨ TỰ của key

int main(){
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
    unordered_map <int, int> mp;
    mp[100] = 200;          // {100, 200}                   Chèn hoặc update value cho key
    mp[200] = 400;          // {200, 400} {100, 200}        Chèn vào trước dãy chứ không phải vào sau
    mp.insert({50, 300});   // {50, 300} {200, 400} {100, 200}      Chỉ chèn nếu chưa tồn tại (vẫn chèn vào trước)
    mp.insert({100, 500});  // {50, 300} {200, 400} {100, 200}      do key 100 đã tồn tại nên không update giá trị cho key 100
    mp[200] = 900;          // {50, 300} {200, 900} {100, 200}      map[key] = value thì vẫn thay đổi giá trị phần tử bình thường

    cout << mp.size() << endl;  // output:  3
    cout << mp[400] << endl;    // output:  0
    
    for (auto x : mp){
        cout << x.first << " " << x.second << endl;
    }
    for (auto it = mp.begin(); it != mp.end(); it++){
        cout << (*it).first << " " << (*it).second << endl;
    }
    
    // -------------
    if(mp.count(100) != 0) cout << "FOUND 100\n";
    if(mp.find(200) != mp.end()) cout << "FOUND 200\n";

    // -------------
    mp.erase(200);
    if(mp.find(200) == mp.end()) cout << "NOT FOUND 200 after calling erase() function\n";

    return 0;
}