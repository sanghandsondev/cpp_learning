/**
 * ============================================================
 * BÀI 5: unordered_map - Hash Map trong C++ STL
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Thành thạo sử dụng unordered_map (ĐÂY LÀ CÁI BẠN SẼ DÙNG NHIỀU NHẤT)
 * - Biết tất cả các thao tác quan trọng
 * - Áp dụng được vào bài toán thuật toán
 * 
 * unordered_map<Key, Value>:
 * - Lưu cặp key-value
 * - Key là duy nhất
 * - Thứ tự KHÔNG được đảm bảo (khác map)
 * - Trung bình O(1) cho insert, search, delete
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 5: unordered_map trong C++" << endl;
    cout << "========================================\n" << endl;

    // ========================================================
    // 1. KHỞI TẠO
    // ========================================================
    cout << "--- 1. Các cách khởi tạo ---\n" << endl;

    // Cách 1: Rỗng
    unordered_map<string, int> age;

    // Cách 2: Initializer list
    unordered_map<string, int> score = {
        {"An", 95},
        {"Binh", 80},
        {"Cuong", 75}
    };

    // Cách 3: Copy
    unordered_map<string, int> score_copy(score);

    cout << "  Đã tạo 3 unordered_map\n" << endl;

    // ========================================================
    // 2. THÊM PHẦN TỬ
    // ========================================================
    cout << "--- 2. Thêm phần tử ---\n" << endl;

    // Cách 1: operator[] — Nếu key chưa có → tạo mới, có rồi → ghi đè
    age["An"] = 20;
    age["Binh"] = 21;
    age["Cuong"] = 19;

    // Cách 2: insert() — Chỉ thêm nếu key chưa tồn tại
    auto result = age.insert({"Dung", 22});
    cout << "  Insert \"Dung\": " << (result.second ? "Thành công" : "Đã tồn tại") << endl;

    result = age.insert({"An", 99}); // An đã có → không ghi đè!
    cout << "  Insert \"An\" lần 2: " << (result.second ? "Thành công" : "Đã tồn tại") << endl;
    cout << "  age[\"An\"] = " << age["An"] << " (vẫn là 20, không bị ghi đè)\n" << endl;

    // Cách 3: emplace() — Tương tự insert nhưng hiệu quả hơn
    age.emplace("Em", 23);

    // ⚠️ CHÚ Ý QUAN TRỌNG: operator[] tạo phần tử mới nếu key không tồn tại!
    cout << "  ⚠️ Cẩn thận với operator[]:" << endl;
    cout << "  age[\"Xyz\"] = " << age["Xyz"] << endl; // Tạo "Xyz" với value = 0!
    cout << "  → \"Xyz\" giờ đã tồn tại với value = 0!\n" << endl;

    // ========================================================
    // 3. TRUY CẬP PHẦN TỬ
    // ========================================================
    cout << "--- 3. Truy cập phần tử ---\n" << endl;

    // Cách 1: operator[] (tạo mới nếu không có — CẨN THẬN!)
    cout << "  age[\"An\"] = " << age["An"] << endl;

    // Cách 2: at() — Throw exception nếu key không tồn tại (AN TOÀN HƠN)
    try {
        cout << "  age.at(\"Binh\") = " << age.at("Binh") << endl;
        cout << "  age.at(\"NotExist\") = " << age.at("NotExist") << endl;
    } catch (const out_of_range& e) {
        cout << "  → Exception: " << e.what() << endl;
    }

    // ========================================================
    // 4. KIỂM TRA TỒN TẠI (QUAN TRỌNG NHẤT!)
    // ========================================================
    cout << "\n--- 4. Kiểm tra key tồn tại ---\n" << endl;

    // Cách 1: count() — Trả về 0 hoặc 1
    if (age.count("An")) {
        cout << "  count(\"An\") = 1 → Tồn tại" << endl;
    }
    if (!age.count("Nobody")) {
        cout << "  count(\"Nobody\") = 0 → Không tồn tại" << endl;
    }

    // Cách 2: find() — Trả về iterator
    auto it = age.find("Binh");
    if (it != age.end()) {
        cout << "  find(\"Binh\") → key=\"" << it->first 
             << "\", value=" << it->second << endl;
    }

    it = age.find("Nobody");
    if (it == age.end()) {
        cout << "  find(\"Nobody\") → Không tìm thấy" << endl;
    }

    // Cách 3: contains() — C++20 (ngắn gọn nhất)
    // if (age.contains("An")) { ... }

    // ========================================================
    // 5. DUYỆT PHẦN TỬ
    // ========================================================
    cout << "\n--- 5. Duyệt tất cả phần tử ---\n" << endl;

    // Cách 1: Range-based for loop (HAY DÙNG NHẤT)
    cout << "  Range-based for:" << endl;
    for (const auto& [name, a] : age) {  // Structured binding (C++17)
        cout << "    " << name << " → " << a << endl;
    }

    // Cách 2: Iterator
    cout << "\n  Iterator:" << endl;
    for (auto it = age.begin(); it != age.end(); ++it) {
        cout << "    " << it->first << " → " << it->second << endl;
    }

    // ========================================================
    // 6. XÓA PHẦN TỬ
    // ========================================================
    cout << "\n--- 6. Xóa phần tử ---\n" << endl;

    cout << "  Size trước: " << age.size() << endl;

    // Xóa theo key
    age.erase("Xyz");
    cout << "  Sau erase(\"Xyz\"): size = " << age.size() << endl;

    // Xóa theo iterator
    auto it2 = age.find("Em");
    if (it2 != age.end()) {
        age.erase(it2);
        cout << "  Sau erase iterator \"Em\": size = " << age.size() << endl;
    }

    // Xóa tất cả
    // age.clear();

    // ========================================================
    // 7. CÁC HÀM HỮU ÍCH KHÁC
    // ========================================================
    cout << "\n--- 7. Các hàm khác ---\n" << endl;

    cout << "  size()         = " << age.size() << endl;
    cout << "  empty()        = " << (age.empty() ? "true" : "false") << endl;
    cout << "  bucket_count() = " << age.bucket_count() << " (số bucket/ô)" << endl;
    cout << "  load_factor()  = " << age.load_factor() << endl;
    cout << "  max_load_factor() = " << age.max_load_factor() << endl;

    // ========================================================
    // 8. VÍ DỤ THỰC TẾ: Đếm từ
    // ========================================================
    cout << "\n--- 8. Ví dụ: Đếm tần suất từ ---\n" << endl;

    vector<string> words = {"hello", "world", "hello", "cpp", "world", 
                            "hello", "hash", "map", "hello", "cpp"};

    unordered_map<string, int> freq;
    for (const string& w : words) {
        freq[w]++;  // Nếu chưa có → tạo với value=0 → tăng lên 1
    }

    cout << "  Tần suất từ:" << endl;
    for (const auto& [word, count] : freq) {
        cout << "    \"" << word << "\" xuất hiện " << count << " lần" << endl;
    }

    // ========================================================
    // TỔNG KẾT
    // ========================================================
    cout << "\n========================================" << endl;
    cout << "  TỔNG KẾT: Các thao tác hay dùng nhất" << endl;
    cout << "========================================" << endl;
    cout << R"(
  // Khai báo
  unordered_map<string, int> mp;
  
  // Thêm/Cập nhật
  mp[key] = value;
  
  // Kiểm tra tồn tại (DÙNG NHIỀU NHẤT)
  if (mp.count(key)) { ... }
  if (mp.find(key) != mp.end()) { ... }
  
  // Đếm tần suất (PATTERN RẤT HAY)
  for (auto x : arr) mp[x]++;
  
  // Duyệt
  for (auto& [k, v] : mp) { ... }
  
  // Xóa
  mp.erase(key);
)" << endl;

    return 0;
}
