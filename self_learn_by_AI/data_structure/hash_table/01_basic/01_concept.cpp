/**
 * ============================================================
 * BÀI 1: KHÁI NIỆM HASH TABLE - Tự xây dựng bảng băm đơn giản
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu hash table hoạt động như thế nào "bên trong"
 * - Tự tay xây dựng một hash table đơn giản nhất
 * - Hiểu tại sao tìm kiếm O(1)
 * 
 * BẢN CHẤT:
 * Hash table = Mảng + Hash function
 * - Mảng: Nơi lưu trữ dữ liệu
 * - Hash function: Công thức biến key → index trong mảng
 * 
 * VÍ DỤ THỰC TẾ:
 * Tưởng tượng bạn có tủ 10 ngăn (index 0-9).
 * Bạn muốn cất "điểm của học sinh" vào tủ:
 *   - Lấy mã số học sinh (key)
 *   - Chia dư cho 10 (hash function)  
 *   - Bỏ vào ngăn tương ứng
 *   - Khi cần tìm → chia dư lại → biết ngay ngăn nào!
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// PHẦN 1: Hash table đơn giản nhất - chỉ lưu số nguyên
// ============================================================

class SimpleHashTable {
private:
    static const int TABLE_SIZE = 10;   // Maximum size of the hash table
    int table[TABLE_SIZE];              // Array store data
    bool occupied[TABLE_SIZE];          // Mark occupied slots

public:
    SimpleHashTable() {
        // Initialize table and occupied flags
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = 0;
            occupied[i] = false;
        }
    }

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    void insert(int key, int value) {
        int index = hashFunction(key);
        table[index] = value;
        occupied[index] = true;
        cout << "  Insert: key=" << key 
             << " → hash=" << index 
             << " → value=" << value << endl;
    }

    int search(int key) {
        int index = hashFunction(key);
        if (occupied[index]) {
            return table[index];
        }
        return -1; // Không tìm thấy
    }

    void remove(int key) {
        int index = hashFunction(key);
        if (occupied[index]) {
            occupied[index] = false;
            cout << "  Removed key=" << key << " at index=" << index << endl;
        }
    }

    void display() {
        cout << "\n  Trạng thái bảng băm:" << endl;
        cout << "  +-------+-------+----------+" << endl;
        cout << "  | Index | Value | Occupied |" << endl;
        cout << "  +-------+-------+----------+" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  |   " << i << "   |  ";
            if (occupied[i])
                cout << table[i] << "   |    ✓     |" << endl;
            else
                cout << " -  |    ✗     |" << endl;
        }
        cout << "  +-------+-------+----------+" << endl;
    }
};

// ============================================================
// PHẦN 2: Hash table lưu key-value (string → int)
// ============================================================

class StudentScoreTable {
private:
    static const int TABLE_SIZE = 7; // Dùng số nguyên tố thì phân bố đều hơn

    struct Entry {
        string name;    // key
        float score;    // value
        bool occupied;
    };

    Entry table[TABLE_SIZE];

public:
    StudentScoreTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].occupied = false;
        }
    }

    // Hash function cho string: cộng mã ASCII các ký tự
    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) {
            sum += c;   // Cộng mã ASCII
        }
        return sum % TABLE_SIZE;
    }

    void insert(const string& name, float score) {
        int index = hashFunction(name);
        table[index] = {name, score, true};
        cout << "  Insert: \"" << name << "\" → hash=" << index 
             << " → score=" << score << endl;
    }

    float search(const string& name) {
        int index = hashFunction(name);
        if (table[index].occupied && table[index].name == name) {
            return table[index].score;
        }
        return -1;
    }

    void display() {
        cout << "\n  Bảng điểm học sinh:" << endl;
        cout << "  +-------+----------+-------+" << endl;
        cout << "  | Index |   Name   | Score |" << endl;
        cout << "  +-------+----------+-------+" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  |   " << i << "   | ";
            if (table[i].occupied)
                cout << table[i].name << string(8 - table[i].name.size(), ' ') 
                     << " | " << table[i].score << "  |" << endl;
            else
                cout << "  ---    |  ---  |" << endl;
        }
        cout << "  +-------+----------+-------+" << endl;
    }
};

int main() {
    // --- Demo 1: Hash table đơn giản ---
    SimpleHashTable ht;
    ht.insert(15, 150);    // 15 % 10 = 5
    ht.insert(23, 230);    // 23 % 10 = 3
    ht.insert(42, 420);    // 42 % 10 = 2
    ht.insert(8, 80);      // 8  % 10 = 8
    ht.insert(37, 370);    // 37 % 10 = 7

    ht.display();

    cout << "\n  Tìm key=23: value=" << ht.search(23) << endl;
    cout << "  Tìm key=99: value=" << ht.search(99) << " (không tìm thấy)" << endl;

    // --- Demo 2: Bảng điểm học sinh ---
    StudentScoreTable st;
    st.insert("An", 9.5);       // 'An' → 65+110 = 175 → 175 % 7 = 0
    st.insert("Binh", 8.0);     // 'Binh' → 66+105+110+104 = 385 → 385 % 7 = 0 (COLLISION!)
    st.insert("Cuong", 7.5);    // 'Cuong' → 67+117+111+110+103 = 508 → 508 % 7 = 4
    st.insert("Dung", 9.0);     // 'Dung' → 68+117+110+103 = 398 → 398 % 7 = 6

    st.display();

    cout << "\n  Tìm điểm của \"An\": " << st.search("An") << endl;
    cout << "  Tìm điểm của \"Xyz\": " << st.search("Xyz") << " (không tìm thấy)" << endl;

    /* Vấn đề: COLLISION (xung đột)
     Nếu hai key khác nhau nhưng hash function trả về cùng index → COLLISION
     Ví dụ: key=15 và key=25 đều hash về index=5 → COLLISION!
     => Cần cách xử lý để lưu được cả hai key → Xem bài 3 (Chaining) và bài 4 (Open Addressing)
    */

    return 0;
}
