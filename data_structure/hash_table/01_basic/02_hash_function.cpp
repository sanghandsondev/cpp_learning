/**
 * ============================================================
 * BÀI 2: HASH FUNCTION - Các loại hàm băm
 * ============================================================
 * 
 * MỤC TIÊU:
 * - Hiểu hash function hoạt động như thế nào
 * - Biết các loại hash function phổ biến
 * - Hiểu tại sao hash function tốt quan trọng
 * 
 * NGUYÊN TẮC HASH FUNCTION TỐT:
 * 1. Deterministic: Cùng input → luôn cùng output
 * 2. Phân bố đều (uniform distribution)
 * 3. Tính toán nhanh
 * 4. Giảm thiểu collision
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

const int TABLE_SIZE = 11; // Dùng số nguyên tố → phân bố đều hơn

// ============================================================
// 1. Division Method (Phương pháp chia dư) - Phổ biến nhất
// ============================================================
// h(key) = key % TABLE_SIZE
// Ưu: Đơn giản, nhanh
// Nhược: Nếu TABLE_SIZE chọn không tốt → phân bố không đều

int hashDivision(int key) {
    return key % TABLE_SIZE;
}

// ============================================================
// 2. Multiplication Method (Phương pháp nhân)
// ============================================================
// h(key) = floor(TABLE_SIZE * frac(key * A))
// Trong đó A là hằng số 0 < A < 1 (Knuth gợi ý A ≈ 0.6180339887)
// Ưu: TABLE_SIZE không cần là số nguyên tố
// Nhược: Chậm hơn division method

int hashMultiplication(int key) {
    double A = 0.6180339887; // (sqrt(5) - 1) / 2 — Golden ratio
    double val = key * A;
    double fractional = val - floor(val); // Lấy phần thập phân
    return (int)(TABLE_SIZE * fractional);
}

// ============================================================
// 3. Hash cho String - Phương pháp cộng đơn giản
// ============================================================
// h(s) = (s[0] + s[1] + ... + s[n-1]) % TABLE_SIZE
// Nhược: "abc" và "cba" cho cùng hash (anagram problem)

int hashStringSum(const string& key) {
    int sum = 0;
    for (char c : key) {
        sum += c;
    }
    return sum % TABLE_SIZE;
}

// ============================================================
// 4. Hash cho String - Polynomial Rolling Hash (TỐT HƠN)
// ============================================================
// h(s) = (s[0]*p^0 + s[1]*p^1 + ... + s[n-1]*p^(n-1)) % m
// p thường = 31 hoặc 37 (số nguyên tố)
// Ưu: Phân biệt được "abc" và "cba"

int hashStringPolynomial(const string& key) {
    long long hash_value = 0;
    long long p = 31;          // base
    long long p_pow = 1;       // p^i
    
    for (char c : key) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % TABLE_SIZE;
        p_pow = (p_pow * p) % TABLE_SIZE;
    }
    return (int)hash_value;
}

// ============================================================
// 5. djb2 Hash - Hash phổ biến cho string trong thực tế
// ============================================================
// Được tạo bởi Daniel J. Bernstein
// Rất nhanh và phân bố khá đều

unsigned long hashDJB2(const string& key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}

// ============================================================
// DEMO: So sánh phân bố của các hash function
// ============================================================

void testDistribution(const string& name, vector<int>& buckets) {
    cout << "\n  " << name << ":" << endl;
    int maxCount = *max_element(buckets.begin(), buckets.end());
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "    [" << i << "] ";
        // Vẽ biểu đồ
        for (int j = 0; j < buckets[i]; j++) cout << "█";
        cout << " (" << buckets[i] << ")" << endl;
    }
    
    // Tính độ lệch chuẩn (đánh giá phân bố đều)
    double mean = 0;
    for (int b : buckets) mean += b;
    mean /= TABLE_SIZE;
    
    double variance = 0;
    for (int b : buckets) variance += (b - mean) * (b - mean);
    variance /= TABLE_SIZE;
    
    cout << "    → Độ lệch chuẩn: " << sqrt(variance) 
         << " (càng nhỏ → phân bố càng đều)" << endl;
}

int main() {
    // --- Demo 1: Hash cho số nguyên ---
    cout << "--- 1. So sánh hash function cho số nguyên ---" << endl;
    
    vector<int> testKeys = {10, 22, 31, 4, 15, 28, 17, 88, 59, 33, 
                            42, 56, 71, 93, 64, 27, 38, 49, 80, 11};
    
    vector<int> divBuckets(TABLE_SIZE, 0);
    vector<int> mulBuckets(TABLE_SIZE, 0);
    
    cout << "\n  Key → Division → Multiplication" << endl;
    for (int key : testKeys) {
        int divHash = hashDivision(key);
        int mulHash = hashMultiplication(key);
        divBuckets[divHash]++;
        mulBuckets[mulHash]++;
        cout << "  " << key << " → " << divHash << " → " << mulHash << endl;
    }
    
    testDistribution("Division Method", divBuckets);
    testDistribution("Multiplication Method", mulBuckets);

    // --- Demo 2: Hash cho string ---
    cout << "\n\n--- 2. So sánh hash function cho string ---" << endl;
    
    vector<string> names = {"an", "binh", "cuong", "dung", "em", 
                            "fong", "giang", "hoa", "ich", "kim",
                            "lan", "minh", "nam", "oanh", "phuc"};
    
    vector<int> sumBuckets(TABLE_SIZE, 0);
    vector<int> polyBuckets(TABLE_SIZE, 0);
    vector<int> djb2Buckets(TABLE_SIZE, 0);
    
    cout << "\n  Name → Sum → Polynomial → DJB2" << endl;
    for (const string& name : names) {
        int sumH = hashStringSum(name);
        int polyH = hashStringPolynomial(name);
        int djb2H = hashDJB2(name);
        sumBuckets[sumH]++;
        polyBuckets[polyH]++;
        djb2Buckets[djb2H]++;
        cout << "  \"" << name << "\"" << string(8 - name.size(), ' ')
             << " → " << sumH << " → " << polyH << " → " << djb2H << endl;
    }
    
    testDistribution("Sum Hash (đơn giản)", sumBuckets);
    testDistribution("Polynomial Hash", polyBuckets);
    testDistribution("DJB2 Hash", djb2Buckets);

    // --- Demo 3: Vấn đề Anagram ---
    cout << "\n\n--- 3. Vấn đề Anagram ---" << endl;
    cout << R"(
  "abc" và "cba" là anagram (hoán vị chữ cái)
  
  Sum hash:
    "abc" = 97+98+99 = 294 → 294 % 11 = )" << hashStringSum("abc") << R"(
    "cba" = 99+98+97 = 294 → 294 % 11 = )" << hashStringSum("cba") << R"(
    → COLLISION! (vì không phân biệt được vị trí ký tự)
  
  Polynomial hash:
    "abc" → )" << hashStringPolynomial("abc") << R"(
    "cba" → )" << hashStringPolynomial("cba") << R"(
    → KHÁC! (tốt hơn vì tính đến vị trí ký tự)
)" << endl;

    /* --- TỔNG KẾT ---
     Đây chỉ là demo đơn giản để minh họa cách hoạt động của hash function.
     1. Division method         phù hợp cho số nguyên, nhưng cần chọn TABLE_SIZE là số nguyên tố.
     2. Multiplication method   không phụ thuộc vào TABLE_SIZE, tốt cho số nguyên nhưng chậm hơn.

     3. Sum hash                quá đơn giản, không phân biệt được anagram.
     4. Polynomial hash         tốt cho string, phân biệt được vị trí ký tự.
     5. DJB2                    là hash phổ biến trong thực tế, nhanh và phân bố khá đều.

     ⚠️ Trong thực tế, C++ std::hash đã implement hash tốt rồi. 
     => Bạn chỉ cần tự viết khi dùng key là struct/class tự định nghĩa (xem bài 08_custom_hash.cpp)
    */

    return 0;
}
