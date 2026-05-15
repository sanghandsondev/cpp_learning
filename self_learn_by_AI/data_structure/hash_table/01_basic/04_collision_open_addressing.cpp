/**
 * ============================================================
 * BÀI 4: COLLISION HANDLING - Open Addressing
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu Open Addressing: KHÔNG dùng linked list
 * - Khi collision → tìm ô trống khác trong CHÍNH bảng đó
 * - 3 cách tìm ô trống: Linear, Quadratic, Double Hashing
 * 
 * SO SÁNH VỚI CHAINING:
 * - Chaining:        Mỗi ô chứa linked list → dùng thêm bộ nhớ ngoài
 * - Open Addressing: Tất cả dữ liệu nằm trong bảng → không cần bộ nhớ ngoài
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 1. LINEAR PROBING
// ============================================================
// Khi collision tại index i:
//   Thử i+1, i+2, i+3, ... (quay vòng)
// 
// Ưu: Đơn giản, cache-friendly (truy cập liên tiếp trong bộ nhớ)
// Nhược: Primary clustering (các phần tử tụ thành cụm)

class LinearProbingTable {
private:
    static const int TABLE_SIZE = 11;

    struct Entry {
        int key;
        string value;
        bool occupied;
        bool deleted;  // Đánh dấu "đã xóa" (lazy deletion)
    };

    Entry table[TABLE_SIZE];

public:
    LinearProbingTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].occupied = false;
            table[i].deleted = false;
        }
    }

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    void insert(int key, const string& value) {
        int index = hashFunction(key);
        int originalIndex = index;
        int probeCount = 0;

        // Tìm ô trống hoặc ô đã xóa
        while (table[index].occupied && !table[index].deleted) {
            if (table[index].key == key) {
                table[index].value = value; // Cập nhật
                cout << "  Update key=" << key << " at index=" << index << endl;
                return;
            }
            index = (index + 1) % TABLE_SIZE; // Linear: +1
            probeCount++;
            if (index == originalIndex) {
                cout << "  ⚠️ Bảng đầy! Không thể insert key=" << key << endl;
                return;
            }
        }

        table[index] = {key, value, true, false};
        cout << "  Insert key=" << key << " → hash=" << originalIndex;
        if (probeCount > 0) 
            cout << " → probe " << probeCount << " lần → index=" << index;
        cout << endl;
    }

    string search(int key) {
        int index = hashFunction(key);
        int originalIndex = index;

        while (table[index].occupied || table[index].deleted) {
            if (table[index].occupied && table[index].key == key) {
                return table[index].value;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == originalIndex) break;
        }
        return "NOT_FOUND";
    }

    // Lazy deletion: Đánh dấu "deleted" thay vì xóa thật
    // Vì nếu xóa thật, sẽ phá vỡ chuỗi probe
    bool remove(int key) {
        int index = hashFunction(key);
        int originalIndex = index;

        while (table[index].occupied || table[index].deleted) {
            if (table[index].occupied && table[index].key == key) {
                table[index].occupied = false;
                table[index].deleted = true; // Đánh dấu, không xóa thật!
                cout << "  Removed key=" << key << " at index=" << index 
                     << " (lazy deletion)" << endl;
                return true;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == originalIndex) break;
        }
        return false;
    }

    void display() {
        cout << "\n  ┌───────┬──────┬──────────┬─────────┐" << endl;
        cout << "  │ Index │ Key  │  Value   │ Status  │" << endl;
        cout << "  ├───────┼──────┼──────────┼─────────┤" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  │  " << (i < 10 ? " " : "") << i << "  │";
            if (table[i].occupied) {
                cout << "  " << table[i].key 
                     << (table[i].key < 10 ? "  " : " ") << " │ "
                     << table[i].value 
                     << string(8 - table[i].value.size(), ' ') << " │   ✓     │";
            } else if (table[i].deleted) {
                cout << " ---  │  ---     │ deleted │";
            } else {
                cout << " ---  │  ---     │  empty  │";
            }
            cout << endl;
        }
        cout << "  └───────┴──────┴──────────┴─────────┘" << endl;
    }
};

// ============================================================
// 2. QUADRATIC PROBING
// ============================================================
// Khi collision tại index i:
//   Thử i+1², i+2², i+3², ... = i+1, i+4, i+9, ...
// 
// Ưu: Giảm primary clustering
// Nhược: Secondary clustering, không đảm bảo tìm được ô trống

class QuadraticProbingTable {
private:
    static const int TABLE_SIZE = 11;

    struct Entry {
        int key;
        string value;
        bool occupied;
        bool deleted;
    };

    Entry table[TABLE_SIZE];

public:
    QuadraticProbingTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].occupied = false;
            table[i].deleted = false;
        }
    }

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    void insert(int key, const string& value) {
        int index = hashFunction(key);
        int originalIndex = index;

        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = (originalIndex + i * i) % TABLE_SIZE; // Quadratic: +i²

            if (!table[probeIndex].occupied || table[probeIndex].deleted) {
                table[probeIndex] = {key, value, true, false};
                cout << "  Insert key=" << key << " → hash=" << originalIndex;
                if (i > 0) cout << " → probe " << i << " lần (+" << i*i << ") → index=" << probeIndex;
                cout << endl;
                return;
            }
            if (table[probeIndex].key == key) {
                table[probeIndex].value = value;
                return;
            }
        }
        cout << "  ⚠️ Không tìm được ô trống cho key=" << key << endl;
    }

    string search(int key) {
        int originalIndex = hashFunction(key);
        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = (originalIndex + i * i) % TABLE_SIZE;
            if (!table[probeIndex].occupied && !table[probeIndex].deleted) break;
            if (table[probeIndex].occupied && table[probeIndex].key == key) {
                return table[probeIndex].value;
            }
        }
        return "NOT_FOUND";
    }

    void display() {
        cout << "\n  Quadratic Probing Table:" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << (i < 10 ? " " : "") << i << "] ";
            if (table[i].occupied)
                cout << "key=" << table[i].key << ", val=\"" << table[i].value << "\"";
            else if (table[i].deleted)
                cout << "(deleted)";
            else
                cout << "(empty)";
            cout << endl;
        }
    }
};

// ============================================================
// MAIN: Demo
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 4: OPEN ADDRESSING" << endl;
    cout << "========================================\n" << endl;

    // --- Demo 1: Linear Probing ---
    cout << "=== 1. LINEAR PROBING ===" << endl;
    cout << "  (collision → thử ô kế tiếp: +1, +2, +3...)\n" << endl;

    LinearProbingTable lp;
    lp.insert(10, "Apple");    // 10 % 11 = 10
    lp.insert(21, "Banana");   // 21 % 11 = 10 → collision! → 10+1=11%11=0
    lp.insert(32, "Cherry");   // 32 % 11 = 10 → collision! → thử 0 → collision! → 1
    lp.insert(5, "Date");      // 5  % 11 = 5
    lp.insert(16, "Elderberry"); // 16 % 11 = 5 → collision! → 6
    lp.insert(3, "Fig");       // 3  % 11 = 3

    lp.display();

    cout << "\n  Search key=21: \"" << lp.search(21) << "\"" << endl;
    cout << "  Search key=99: \"" << lp.search(99) << "\"" << endl;

    cout << endl;
    lp.remove(21);
    cout << "  Search key=32 (sau khi xóa 21): \"" << lp.search(32) << "\"" << endl;
    cout << "  ↑ Vẫn tìm được nhờ lazy deletion!\n" << endl;

    // --- Demo 2: Quadratic Probing ---
    cout << "\n=== 2. QUADRATIC PROBING ===" << endl;
    cout << "  (collision → thử +1², +2², +3²... = +1, +4, +9...)\n" << endl;

    QuadraticProbingTable qp;
    qp.insert(10, "Apple");
    qp.insert(21, "Banana");   // 10 → collision → 10+1=11%11=0
    qp.insert(32, "Cherry");   // 10 → collision → 10+1=0 collision → 10+4=14%11=3
    qp.insert(5, "Date");
    qp.insert(16, "Elderberry");

    qp.display();

    // --- So sánh ---
    cout << "\n\n========================================" << endl;
    cout << "  SO SÁNH CÁC PHƯƠNG PHÁP" << endl;
    cout << "========================================" << endl;
    cout << R"(
  ┌───────────────────┬──────────────────┬──────────────────┬──────────────────┐
  │                   │ Separate Chain.  │ Linear Probing   │ Quadratic Prob.  │
  ├───────────────────┼──────────────────┼──────────────────┼──────────────────┤
  │ Bộ nhớ thêm       │ Có (linked list) │ Không            │ Không            │
  │ Cache friendly    │ Kém             │ Tốt nhất         │ Khá              │
  │ Clustering        │ Không           │ Primary cluster  │ Secondary clust. │
  │ Load factor max   │ > 1 được        │ < 1 bắt buộc     │ < 1 bắt buộc     │
  │ Xóa phần tử      │ Dễ             │ Cần lazy delete  │ Cần lazy delete  │
  │ Implement         │ Phức tạp hơn    │ Đơn giản nhất    │ Đơn giản         │
  └───────────────────┴──────────────────┴──────────────────┴──────────────────┘
  
  💡 Trong thực tế:
  - C++ unordered_map dùng Separate Chaining
  - Python dict dùng Open Addressing
  - Java HashMap dùng Separate Chaining (chuyển sang Red-Black Tree khi list dài)
)" << endl;

    return 0;
}
