/**
 * ============================================================
 * BÀI 6: unordered_set - Hash Set trong C++ STL
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu unordered_set: chỉ lưu KEY, không có VALUE
 * - Dùng để kiểm tra sự tồn tại nhanh O(1)
 * - Dùng để loại bỏ phần tử trùng lặp
 * 
 * SO SÁNH:
 * - unordered_map<K,V>: Lưu cặp key-value
 * - unordered_set<K>:   Chỉ lưu key (tập hợp, không trùng lặp)
 */

#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 6: unordered_set trong C++" << endl;
    cout << "========================================\n" << endl;

    // ========================================================
    // 1. KHỞI TẠO VÀ THAO TÁC CƠ BẢN
    // ========================================================
    cout << "--- 1. Cơ bản ---\n" << endl;

    unordered_set<int> s = {1, 3, 5, 7, 9};

    // Thêm
    s.insert(2);
    s.insert(4);
    s.insert(1); // Đã có → không thêm (set = tập hợp, không trùng)

    // Kiểm tra tồn tại
    cout << "  Có 3 không? " << (s.count(3) ? "Có" : "Không") << endl;
    cout << "  Có 6 không? " << (s.count(6) ? "Có" : "Không") << endl;

    // Xóa
    s.erase(9);

    // Duyệt
    cout << "  Set: { ";
    for (int x : s) cout << x << " ";
    cout << "}" << endl;

    cout << "  Size: " << s.size() << endl;

    // ========================================================
    // 2. ỨNG DỤNG 1: Loại bỏ trùng lặp
    // ========================================================
    cout << "\n--- 2. Loại bỏ trùng lặp ---\n" << endl;

    vector<int> arr = {5, 3, 1, 3, 5, 7, 1, 9, 3, 7};
    cout << "  Mảng gốc:   ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    unordered_set<int> unique_set(arr.begin(), arr.end());
    cout << "  Sau loại bỏ: ";
    for (int x : unique_set) cout << x << " ";
    cout << endl;

    // ========================================================
    // 3. ỨNG DỤNG 2: Kiểm tra phần tử chung (Intersection)
    // ========================================================
    cout << "\n--- 3. Tìm phần tử chung của 2 mảng ---\n" << endl;

    vector<int> arr1 = {1, 2, 3, 4, 5};
    vector<int> arr2 = {3, 4, 5, 6, 7};

    unordered_set<int> set1(arr1.begin(), arr1.end());
    vector<int> intersection;

    for (int x : arr2) {
        if (set1.count(x)) {
            intersection.push_back(x);
        }
    }

    cout << "  arr1: ";
    for (int x : arr1) cout << x << " ";
    cout << "\n  arr2: ";
    for (int x : arr2) cout << x << " ";
    cout << "\n  Chung: ";
    for (int x : intersection) cout << x << " ";
    cout << endl;

    // ========================================================
    // 4. ỨNG DỤNG 3: Kiểm tra mảng có trùng lặp không
    // ========================================================
    cout << "\n--- 4. Kiểm tra trùng lặp ---\n" << endl;

    auto hasDuplicate = [](const vector<int>& v) -> bool {
        unordered_set<int> seen;
        for (int x : v) {
            if (seen.count(x)) return true; // Đã thấy rồi!
            seen.insert(x);
        }
        return false;
    };

    vector<int> a = {1, 2, 3, 4, 5};
    vector<int> b = {1, 2, 3, 2, 5};

    cout << "  {1,2,3,4,5} có trùng? " << (hasDuplicate(a) ? "Có" : "Không") << endl;
    cout << "  {1,2,3,2,5} có trùng? " << (hasDuplicate(b) ? "Có" : "Không") << endl;

    // ========================================================
    // 5. ỨNG DỤNG 4: Đếm phần tử distinct
    // ========================================================
    cout << "\n--- 5. Đếm phần tử distinct ---\n" << endl;

    vector<string> fruits = {"apple", "banana", "apple", "cherry", 
                              "banana", "date", "apple"};
    unordered_set<string> unique_fruits(fruits.begin(), fruits.end());
    
    cout << "  Tổng: " << fruits.size() << " trái" << endl;
    cout << "  Distinct: " << unique_fruits.size() << " loại" << endl;

    // ========================================================
    // SO SÁNH unordered_set vs set
    // ========================================================
    cout << "\n========================================" << endl;
    cout << "  SO SÁNH unordered_set vs set" << endl;
    cout << "========================================" << endl;
    cout << R"(
  ┌──────────────────┬─────────────────┬──────────────────┐
  │                  │ unordered_set   │      set         │
  ├──────────────────┼─────────────────┼──────────────────┤
  │ Cấu trúc bên    │ Hash Table      │ Red-Black Tree   │
  │ Insert           │ O(1) avg        │ O(log n)         │
  │ Search           │ O(1) avg        │ O(log n)         │
  │ Delete           │ O(1) avg        │ O(log n)         │
  │ Có thứ tự       │ ✗ Không         │ ✓ Có (sorted)    │
  │ Dùng khi         │ Chỉ cần tìm    │ Cần thứ tự hoặc │
  │                  │ nhanh           │ lower/upper_bound│
  └──────────────────┴─────────────────┴──────────────────┘
)" << endl;

    return 0;
}
