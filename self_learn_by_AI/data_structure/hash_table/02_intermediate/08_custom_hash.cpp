/**
 * ============================================================
 * BÀI 8: CUSTOM HASH - Tự viết hash cho struct/class
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Biết cách dùng unordered_map/set với kiểu dữ liệu tự định nghĩa
 * - C++ không biết hash struct của bạn → bạn phải tự viết!
 * 
 * KHI NÀO CẦN?
 * - Khi key là pair, tuple, hoặc struct/class bạn tự tạo
 * - Ví dụ: Dùng tọa độ (x, y) làm key
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
using namespace std;

// ============================================================
// 1. Dùng pair<int,int> làm key
// ============================================================
// C++ KHÔNG có sẵn hash cho pair → phải tự viết

struct PairHash {
    size_t operator()(const pair<int, int>& p) const {
        // Kết hợp hash của 2 thành phần
        size_t h1 = hash<int>{}(p.first);
        size_t h2 = hash<int>{}(p.second);
        // Công thức kết hợp phổ biến (từ Boost)
        return h1 ^ (h2 << 1);
    }
};

void demoPairKey() {
    cout << "--- 1. pair<int,int> làm key ---\n" << endl;

    // Truyền PairHash làm template argument thứ 3
    unordered_map<pair<int, int>, string, PairHash> grid;

    grid[{0, 0}] = "Origin";
    grid[{1, 2}] = "Point A";
    grid[{3, 4}] = "Point B";
    grid[{-1, 5}] = "Point C";

    // Tìm kiếm
    pair<int, int> target = {1, 2};
    if (grid.count(target)) {
        cout << "  (" << target.first << "," << target.second << ") = \"" 
             << grid[target] << "\"" << endl;
    }

    // Duyệt
    cout << "  Tất cả điểm:" << endl;
    for (const auto& [pos, name] : grid) {
        cout << "    (" << pos.first << "," << pos.second << ") → " << name << endl;
    }
}

// ============================================================
// 2. Dùng struct tự định nghĩa làm key
// ============================================================

struct Student {
    string name;
    int id;

    // BẮT BUỘC: phải có operator== để so sánh khi collision
    bool operator==(const Student& other) const {
        return name == other.name && id == other.id;
    }
};

// Cách 1: Viết struct hash riêng
struct StudentHash {
    size_t operator()(const Student& s) const {
        size_t h1 = hash<string>{}(s.name);
        size_t h2 = hash<int>{}(s.id);
        return h1 ^ (h2 << 1);
    }
};

void demoStructKey() {
    cout << "\n--- 2. Struct tự định nghĩa làm key ---\n" << endl;

    unordered_map<Student, float, StudentHash> scores;

    scores[{"An", 1001}] = 9.5;
    scores[{"Binh", 1002}] = 8.0;
    scores[{"Cuong", 1003}] = 7.5;

    Student target = {"Binh", 1002};
    if (scores.count(target)) {
        cout << "  Điểm của Binh (ID 1002): " << scores[target] << endl;
    }

    for (const auto& [student, score] : scores) {
        cout << "  " << student.name << " (ID:" << student.id << ") → " << score << endl;
    }
}

// ============================================================
// 3. Cách 2: Specialize std::hash (cách "chính thống" hơn)
// ============================================================

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Specialize std::hash cho Point
namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            size_t h1 = hash<int>{}(p.x);
            size_t h2 = hash<int>{}(p.y);
            return h1 ^ (h2 * 2654435761); // Fibonacci hashing
        }
    };
}

void demoSpecializeHash() {
    cout << "\n--- 3. Specialize std::hash ---\n" << endl;

    // Không cần truyền hash function → dùng std::hash<Point> tự động
    unordered_set<Point> visited;

    visited.insert({0, 0});
    visited.insert({1, 0});
    visited.insert({0, 1});
    visited.insert({1, 1});

    Point check = {1, 0};
    cout << "  (1,0) đã visited? " << (visited.count(check) ? "Có" : "Không") << endl;

    check = {2, 2};
    cout << "  (2,2) đã visited? " << (visited.count(check) ? "Có" : "Không") << endl;

    // Ứng dụng: BFS/DFS trên lưới → đánh dấu ô đã thăm
    cout << "\n  Ứng dụng: Đánh dấu ô đã thăm trong BFS/DFS" << endl;
    cout << "  unordered_set<Point> visited;" << endl;
    cout << "  if (!visited.count({nx, ny})) { // ô chưa thăm" << endl;
    cout << "      visited.insert({nx, ny});" << endl;
    cout << "      queue.push({nx, ny});" << endl;
    cout << "  }" << endl;
}

// ============================================================
// 4. Dùng Lambda hash (C++20 / nhanh gọn)
// ============================================================

void demoLambdaHash() {
    cout << "\n--- 4. Lambda hash (cách nhanh gọn) ---\n" << endl;

    auto pairHash = [](const pair<int, int>& p) {
        return hash<long long>{}((long long)p.first << 32 | p.second);
    };

    // Phải truyền kiểu hash vào template
    unordered_map<pair<int, int>, int, decltype(pairHash)> mp(10, pairHash);

    mp[{1, 2}] = 100;
    mp[{3, 4}] = 200;

    cout << "  {1,2} → " << mp[{1, 2}] << endl;
    cout << "  {3,4} → " << mp[{3, 4}] << endl;
}

// ============================================================
// 5. Hash function tốt: Kết hợp nhiều field
// ============================================================

void demoGoodHash() {
    cout << "\n--- 5. Công thức kết hợp hash tốt ---\n" << endl;

    cout << R"(
  // Cách 1: XOR + shift (đơn giản, OK cho hầu hết trường hợp)
  return h1 ^ (h2 << 1);
  
  // Cách 2: Boost hash_combine (tốt hơn)
  size_t seed = 0;
  seed ^= hash<T1>{}(val1) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  seed ^= hash<T2>{}(val2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
  
  // Cách 3: Fibonacci hashing
  return h1 ^ (h2 * 2654435761);
  
  // ⚠️ Tránh: return h1 + h2;  (quá nhiều collision)
  // ⚠️ Tránh: return h1 ^ h2;  (hash(a,b) == hash(b,a))
)" << endl;

    // Implement hash_combine giống Boost
    auto hash_combine = [](size_t& seed, size_t val) {
        seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    };

    // Ví dụ: hash cho struct có 3 fields
    struct RGB {
        int r, g, b;
        bool operator==(const RGB& o) const { return r==o.r && g==o.g && b==o.b; }
    };

    auto rgbHash = [&hash_combine](const RGB& c) {
        size_t seed = 0;
        hash_combine(seed, hash<int>{}(c.r));
        hash_combine(seed, hash<int>{}(c.g));
        hash_combine(seed, hash<int>{}(c.b));
        return seed;
    };

    unordered_map<RGB, string, decltype(rgbHash)> colorNames(10, rgbHash);
    colorNames[{255, 0, 0}] = "Red";
    colorNames[{0, 255, 0}] = "Green";
    colorNames[{0, 0, 255}] = "Blue";

    cout << "  (255,0,0) = " << colorNames[{255, 0, 0}] << endl;
    cout << "  (0,255,0) = " << colorNames[{0, 255, 0}] << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 8: CUSTOM HASH" << endl;
    cout << "========================================\n" << endl;

    demoPairKey();
    demoStructKey();
    demoSpecializeHash();
    demoLambdaHash();
    demoGoodHash();

    cout << "\n========================================" << endl;
    cout << "  TỔNG KẾT" << endl;
    cout << "========================================" << endl;
    cout << R"(
  Để dùng custom type làm key trong unordered_map/set:
  
  1. Định nghĩa operator== cho struct
  2. Viết hash function (chọn 1 trong các cách):
     a) Struct riêng: struct MyHash { size_t operator()(...) }
     b) Specialize std::hash<MyType>
     c) Lambda function
  
  3. Truyền vào template:
     unordered_map<MyType, Value, MyHash> mp;
  
  💡 Tip thực tế cho competitive programming:
  - pair<int,int> → dùng PairHash ở trên
  - string → đã có sẵn std::hash<string>
  - Nếu lười → chuyển thành string rồi hash:
    string key = to_string(x) + "," + to_string(y);
    mp[key] = value;  // Chậm hơn nhưng đơn giản
)" << endl;

    return 0;
}
