/**
 * ============================================================
 * BÀI 7: LOAD FACTOR & REHASHING
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu Load Factor ảnh hưởng hiệu suất như thế nào
 * - Hiểu Rehashing là gì và khi nào xảy ra
 * - Biết cách kiểm soát trong unordered_map
 * 
 * KHÁI NIỆM:
 * Load Factor (α) = Số phần tử / Số bucket
 * 
 * - α nhỏ → ít collision → nhanh nhưng tốn bộ nhớ
 * - α lớn → nhiều collision → chậm nhưng tiết kiệm bộ nhớ
 * - Khi α > max_load_factor → REHASHING (tạo bảng mới lớn hơn)
 */

#include <iostream>
#include <unordered_map>
#include <chrono>
using namespace std;

// ============================================================
// 1. Tự implement Rehashing để hiểu bản chất
// ============================================================

class RehashingDemo {
private:
    struct Entry {
        int key;
        int value;
        bool occupied;
    };

    Entry* table;
    int capacity;       // Kích thước bảng hiện tại
    int size_;          // Số phần tử hiện tại
    float maxLoadFactor;

    int hashFunction(int key) {
        return ((key % capacity) + capacity) % capacity; // Đảm bảo dương
    }

    void rehash() {
        int oldCapacity = capacity;
        Entry* oldTable = table;

        capacity *= 2;  // Nhân đôi kích thước
        table = new Entry[capacity];
        for (int i = 0; i < capacity; i++) table[i].occupied = false;

        cout << "\n  🔄 REHASHING: " << oldCapacity << " → " << capacity << " buckets" << endl;

        // Hash lại tất cả phần tử
        int reinserted = 0;
        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].occupied) {
                int newIndex = hashFunction(oldTable[i].key);
                // Linear probing
                while (table[newIndex].occupied) {
                    newIndex = (newIndex + 1) % capacity;
                }
                table[newIndex] = oldTable[i];
                reinserted++;
            }
        }
        cout << "  → Đã rehash " << reinserted << " phần tử\n" << endl;

        delete[] oldTable;
    }

public:
    RehashingDemo(int initialCapacity = 4, float mlf = 0.75f)
        : capacity(initialCapacity), size_(0), maxLoadFactor(mlf) {
        table = new Entry[capacity];
        for (int i = 0; i < capacity; i++) table[i].occupied = false;
    }

    ~RehashingDemo() { delete[] table; }

    void insert(int key, int value) {
        float currentLF = (float)(size_ + 1) / capacity;
        
        // Kiểm tra cần rehash không
        if (currentLF > maxLoadFactor) {
            cout << "  ⚠️ Load factor sẽ là " << currentLF 
                 << " > " << maxLoadFactor << " → cần rehash!" << endl;
            rehash();
        }

        int index = hashFunction(key);
        while (table[index].occupied) {
            if (table[index].key == key) {
                table[index].value = value;
                return;
            }
            index = (index + 1) % capacity;
        }

        table[index] = {key, value, true};
        size_++;
        
        cout << "  Insert key=" << key << " (size=" << size_ 
             << ", capacity=" << capacity 
             << ", LF=" << (float)size_/capacity << ")" << endl;
    }

    void display() {
        cout << "\n  Trạng thái: size=" << size_ << ", capacity=" << capacity 
             << ", LF=" << (float)size_/capacity << endl;
        for (int i = 0; i < capacity; i++) {
            cout << "  [" << i << "] ";
            if (table[i].occupied)
                cout << "key=" << table[i].key << ", val=" << table[i].value;
            else
                cout << "(empty)";
            cout << endl;
        }
    }
};

// ============================================================
// 2. Kiểm soát Load Factor trong unordered_map
// ============================================================

void demoSTLLoadFactor() {
    cout << "\n=== unordered_map Load Factor ===\n" << endl;

    unordered_map<int, int> mp;

    cout << "  Quá trình thêm phần tử:" << endl;
    cout << "  ┌──────┬──────┬─────────┬──────────────┐" << endl;
    cout << "  │ Size │ Buck │  LF     │ MaxLF        │" << endl;
    cout << "  ├──────┼──────┼─────────┼──────────────┤" << endl;

    for (int i = 0; i < 20; i++) {
        mp[i] = i * 10;
        if (i % 3 == 0 || i < 5) { // In một số dòng
            printf("  │ %4zu │ %4zu │ %.4f │ %.4f       │\n", 
                   mp.size(), mp.bucket_count(), 
                   mp.load_factor(), mp.max_load_factor());
        }
    }
    cout << "  └──────┴──────┴─────────┴──────────────┘" << endl;

    // Thay đổi max_load_factor
    cout << "\n  Thay đổi max_load_factor:" << endl;
    
    unordered_map<int, int> mp2;
    mp2.max_load_factor(0.5f); // Giảm → rehash sớm hơn → ít collision hơn
    cout << "  Set max_load_factor = 0.5" << endl;

    for (int i = 0; i < 10; i++) mp2[i] = i;
    cout << "  Sau 10 phần tử: buckets=" << mp2.bucket_count() 
         << ", LF=" << mp2.load_factor() << endl;

    unordered_map<int, int> mp3;
    mp3.max_load_factor(2.0f); // Tăng → rehash muộn hơn → tiết kiệm bộ nhớ
    cout << "\n  Set max_load_factor = 2.0" << endl;

    for (int i = 0; i < 10; i++) mp3[i] = i;
    cout << "  Sau 10 phần tử: buckets=" << mp3.bucket_count() 
         << ", LF=" << mp3.load_factor() << endl;

    // Reserve
    cout << "\n  Dùng reserve() để tránh rehash:" << endl;
    unordered_map<int, int> mp4;
    mp4.reserve(100); // Đặt trước cho 100 phần tử
    cout << "  reserve(100) → buckets = " << mp4.bucket_count() << endl;
    cout << "  → Không cần rehash khi thêm ≤ 100 phần tử!" << endl;
}

// ============================================================
// 3. Benchmark: Ảnh hưởng của Load Factor đến tốc độ
// ============================================================

void benchmark() {
    cout << "\n\n=== Benchmark: Load Factor ảnh hưởng tốc độ ===\n" << endl;

    int N = 100000;

    // Test 1: Không reserve (có rehash nhiều lần)
    auto start = chrono::high_resolution_clock::now();
    {
        unordered_map<int, int> mp;
        for (int i = 0; i < N; i++) mp[i] = i;
    }
    auto end = chrono::high_resolution_clock::now();
    auto time1 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    // Test 2: Reserve trước (không rehash)
    start = chrono::high_resolution_clock::now();
    {
        unordered_map<int, int> mp;
        mp.reserve(N);
        for (int i = 0; i < N; i++) mp[i] = i;
    }
    end = chrono::high_resolution_clock::now();
    auto time2 = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "  Insert " << N << " phần tử:" << endl;
    cout << "  Không reserve: " << time1 << " μs" << endl;
    cout << "  Có reserve:    " << time2 << " μs" << endl;
    cout << "  Nhanh hơn:     " << (float)time1/time2 << "x" << endl;

    cout << R"(
  💡 TIP: Nếu biết trước số phần tử → dùng reserve()
     để tránh rehash → nhanh hơn đáng kể!
)" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 7: LOAD FACTOR & REHASHING" << endl;
    cout << "========================================\n" << endl;

    // 1. Demo tự implement
    cout << "=== 1. Tự implement Rehashing ===\n" << endl;
    cout << "  (Bảng ban đầu: 4 ô, max_load_factor = 0.75)\n" << endl;

    RehashingDemo demo(4, 0.75f);
    demo.insert(1, 10);
    demo.insert(5, 50);
    demo.insert(9, 90);    // LF = 3/4 = 0.75 → sắp rehash
    demo.insert(13, 130);  // LF sẽ > 0.75 → REHASH!
    demo.insert(2, 20);
    demo.insert(6, 60);
    demo.insert(10, 100);  // Có thể rehash lần nữa

    demo.display();

    // 2. STL demo
    demoSTLLoadFactor();

    // 3. Benchmark
    benchmark();

    // Tổng kết
    cout << "========================================" << endl;
    cout << "  TỔNG KẾT" << endl;
    cout << "========================================" << endl;
    cout << R"(
  Load Factor = size / bucket_count
  
  ┌─────────────┬──────────────────────────────────────┐
  │ LF thấp     │ Ít collision, nhanh, tốn bộ nhớ     │
  │ LF cao      │ Nhiều collision, chậm, tiết kiệm RAM │
  │ LF > max    │ Tự động REHASH (tạo bảng mới, lớn hơn)│
  └─────────────┴──────────────────────────────────────┘
  
  Mặc định: max_load_factor = 1.0
  
  Khi rehash:
  1. Tạo bảng mới (thường gấp đôi)
  2. Hash lại TẤT CẢ phần tử → O(n)
  3. Xóa bảng cũ
  
  → Rehash tốn O(n) nhưng hiếm khi xảy ra
  → Amortized (trung bình) vẫn là O(1) cho mỗi insert
)" << endl;

    return 0;
}
