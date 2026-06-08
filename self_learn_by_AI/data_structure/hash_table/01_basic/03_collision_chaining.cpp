/**
 * ============================================================
 * BÀI 3: COLLISION HANDLING - Separate Chaining (Linked List)
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu collision (xung đột) là gì và tại sao xảy ra
 * - Hiểu cách Separate Chaining giải quyết collision
 * - Đây là lý do tại sao cần biết Linked List!
 * 
 * Ý TƯỞNG:
 * - Mỗi ô trong bảng là một danh sách liên kết (linked list)
 * - Khi collision xảy ra → thêm phần tử vào cuối list tại ô đó
 * - Khi tìm kiếm → hash ra index → duyệt list tại ô đó
 * 
 * HÌNH DUNG
 *   Index 0: → [key=10, val="A"] → [key=21, val="B"] → NULL
 *   Index 1: → [key=1, val="C"] → NULL
 *   Index 2: → NULL (trống)
 *   Index 3: → [key=3, val="D"] → [key=14, val="E"] → [key=25, val="F"] → NULL
 *   ...
 */

#include <iostream>
#include <string>
#include <list>    // Dùng std::list (doubly linked list) của C++
using namespace std;

// ============================================================
// Cách 1: Tự implement Linked List (để hiểu bản chất)
// ============================================================

class HashTableManual {
private:
    static const int TABLE_SIZE = 7;

    // Node trong linked list
    struct Node {
        string key;
        int value;
        Node* next;

        Node(string k, int v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[TABLE_SIZE]; // Mảng các con trỏ đầu list

public:
    HashTableManual() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr; // Ban đầu tất cả list rỗng
        }
    }

    ~HashTableManual() {
        // Giải phóng bộ nhớ
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) sum += c;
        return sum % TABLE_SIZE;
    }

    // INSERT: Thêm hoặc cập nhật
    void insert(const string& key, int value) {
        int index = hashFunction(key);

        // Kiểm tra key đã tồn tại chưa → cập nhật
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value; // Cập nhật
                cout << "  Update: \"" << key << "\" = " << value 
                     << " (index " << index << ")" << endl;
                return;
            }
            current = current->next;
        }

        // Key chưa tồn tại → thêm vào đầu list (O(1))
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        cout << "  Insert: \"" << key << "\" = " << value 
             << " (index " << index << ")" << endl;
    }

    // SEARCH: Tìm kiếm
    int search(const string& key) {
        int index = hashFunction(key);
        
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return -1; // Không tìm thấy
    }

    // REMOVE: Xóa phần tử
    bool remove(const string& key) {
        int index = hashFunction(key);

        Node* current = table[index];
        Node* prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                cout << "  Removed: \"" << key << "\" (index " << index << ")" << endl;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // DISPLAY: Hiển thị bảng
    void display() {
        cout << "\n  Bảng băm (Separate Chaining - Manual):" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << i << "]: ";
            Node* current = table[i];
            if (!current) {
                cout << "∅ (trống)";
            }
            while (current) {
                cout << "{\"" << current->key << "\":" << current->value << "}";
                if (current->next) cout << " → ";
                current = current->next;
            }
            cout << endl;
        }
    }
};

// ============================================================
// Cách 2: Dùng std::list (thực tế hay dùng hơn)
// ============================================================

class HashTableSTL {
private:
    static const int TABLE_SIZE = 7;

    struct Entry {
        string key;
        int value;
    };

    list<Entry> table[TABLE_SIZE];      // Mỗi ô là một std::list<Entry>

public:
    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) sum += c;
        return sum % TABLE_SIZE;
    }

    void insert(const string& key, int value) {
        int index = hashFunction(key);

        // Kiểm tra key đã tồn tại
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }

        // Thêm mới
        table[index].push_back({key, value});
    }

    int search(const string& key) {
        int index = hashFunction(key);
        for (const auto& entry : table[index]) {
            if (entry.key == key) return entry.value;
        }
        return -1;
    }

    void display() {
        cout << "\n  Bảng băm (Separate Chaining - STL):" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << i << "]: ";
            if (table[i].empty()) {
                cout << "∅";
            }
            for (auto it = table[i].begin(); it != table[i].end(); ++it) {
                if (it != table[i].begin()) cout << " → ";
                cout << "{\"" << it->key << "\":" << it->value << "}";
            }
            cout << endl;
        }
    }
};

// ============================================================
// MAIN: Demo
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 3: SEPARATE CHAINING" << endl;
    cout << "========================================\n" << endl;

    // --- Demo collision ---
    cout << "--- Demo: Collision xảy ra ---" << endl;
    cout << R"(
  Hash function: (tổng ASCII) % 7
  
  "An"    → (65+110) = 175 → 175 % 7 = 0
  "Ha"    → (72+97)  = 169 → 169 % 7 = 1
  "nA"    → (110+65) = 175 → 175 % 7 = 0  ← COLLISION với "An"!
  
  Separate Chaining giải quyết bằng cách:
  [0]: {"An":95} → {"nA":80} → NULL
  [1]: {"Ha":90} → NULL
)" << endl;

    // --- Demo với manual linked list ---
    cout << "--- Cách 1: Tự implement linked list ---\n" << endl;
    
    HashTableManual ht1;
    ht1.insert("An", 95);
    ht1.insert("Binh", 80);
    ht1.insert("Cuong", 75);
    ht1.insert("Dung", 90);
    ht1.insert("Em", 85);
    ht1.insert("Giang", 70);
    ht1.insert("Hoa", 88);
    ht1.insert("Kim", 92);

    ht1.display();

    cout << "\n  Tìm \"Cuong\": " << ht1.search("Cuong") << endl;
    cout << "  Tìm \"Xyz\":   " << ht1.search("Xyz") << " (không tìm thấy)" << endl;

    cout << endl;
    ht1.remove("Cuong");
    ht1.display();

    // --- Demo với STL ---
    cout << "\n\n--- Cách 2: Dùng std::list ---\n" << endl;

    HashTableSTL ht2;
    ht2.insert("cat", 1);
    ht2.insert("dog", 2);
    ht2.insert("fish", 3);
    ht2.insert("bird", 4);
    ht2.insert("lion", 5);

    ht2.display();

    cout << "\n  Tìm \"dog\": " << ht2.search("dog") << endl;

    // --- Phân tích độ phức tạp ---
    cout << "\n\n========================================" << endl;
    cout << "  PHÂN TÍCH ĐỘ PHỨC TẠP" << endl;
    cout << "========================================" << endl;
    cout << R"(
  Gọi n = số phần tử, m = TABLE_SIZE
  Load factor α = n/m (trung bình mỗi ô có α phần tử)
  
  ┌──────────┬────────────────┬──────────────────┐
  │ Thao tác │  Trung bình    │  Trường hợp xấu  │
  ├──────────┼────────────────┼──────────────────┤
  │ Insert   │    O(1)        │     O(n)         │
  │ Search   │    O(1 + α)    │     O(n)         │
  │ Delete   │    O(1 + α)    │     O(n)         │
  └──────────┴────────────────┴──────────────────┘
  
  Trường hợp xấu nhất: Tất cả key hash về cùng 1 ô
  → Linked list dài n → O(n)
  
  → Giữ α nhỏ (< 1) bằng cách rehashing khi bảng đầy
    (Xem bài 07_load_factor_rehashing.cpp)
)" << endl;

    return 0;
}
