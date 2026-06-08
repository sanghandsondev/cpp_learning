# 🗂️ Hash Table / Hash Map - Hướng dẫn học từ cơ bản đến nâng cao

## 📚 Mục lục

1. [Bản chất của Hash Table](#1-bản-chất-của-hash-table)
2. [Hash Function là gì?](#2-hash-function-là-gì)
3. [Xử lý xung đột (Collision Handling)](#3-xử-lý-xung-đột-collision-handling)
4. [Load Factor & Rehashing](#4-load-factor--rehashing)
5. [Hash Table trong C++ (unordered_map / unordered_set)](#5-hash-table-trong-c)
6. [So sánh với các CTDL khác](#6-so-sánh-với-các-ctdl-khác)
7. [Ứng dụng trong thuật toán](#7-ứng-dụng-trong-thuật-toán)
8. [Thứ tự học](#8-thứ-tự-học-các-file)

---

## 1. Bản chất của Hash Table

### Hash Table là gì?

Hash Table (bảng băm) là một **cấu trúc dữ liệu lưu trữ dữ liệu theo cặp key-value** (khóa - giá trị), cho phép **tìm kiếm, thêm, xóa** phần tử với thời gian trung bình **O(1)**.

### Ý tưởng cốt lõi

Hãy tưởng tượng bạn có **một tủ có 10 ngăn kéo** (đánh số 0-9). Khi bạn muốn cất một món đồ:

1. Bạn dùng một **công thức** (hash function) để tính xem món đồ nên bỏ vào ngăn nào
2. Bạn bỏ món đồ vào ngăn đó
3. Khi cần tìm lại, bạn dùng **cùng công thức** → biết ngay ngăn nào → lấy ra ngay

```
Ví dụ đời thực:
- Key: "Tên học sinh"
- Value: "Điểm thi"
- Hash function: Lấy chữ cái đầu → A=0, B=1, C=2...

"An"  → ngăn 0 → điểm 9.5
"Bình" → ngăn 1 → điểm 8.0
"Cường" → ngăn 2 → điểm 7.5
```

### Tại sao nhanh O(1)?

- **Array**: Truy cập arr[5] → O(1) vì biết vị trí chính xác
- **Hash Table**: Biến key thành index → truy cập như array → O(1)

```
key "hello" → hash("hello") = 3 → table[3] → value
```

### Có cần biết Linked List không?

**CÓ, nhưng không bắt buộc phải master.** Lý do:

- Một trong các cách xử lý **xung đột** (collision) phổ biến nhất là **Separate Chaining** — mỗi ô trong bảng chứa một linked list
- Khi 2 key khác nhau ra cùng index → cả 2 được nối vào linked list ở ô đó
- Nhưng bạn chỉ cần hiểu linked list ở mức cơ bản (thêm/xóa node) là đủ

```
Bảng băm với Separate Chaining:

Index 0: → ["An", 9.5] → ["Anh", 8.0] → NULL
Index 1: → ["Bình", 8.0] → NULL
Index 2: → ["Cường", 7.5] → NULL
Index 3: → (trống)
...
```

---

## 2. Hash Function là gì?

Hash function là hàm biến đổi **key** (bất kỳ kiểu dữ liệu) thành **một số nguyên** (index trong mảng).

### Yêu cầu của hash function tốt:
- **Deterministic**: Cùng input → luôn cùng output
- **Phân bố đều**: Các key nên rải đều trên bảng, tránh dồn vào 1 chỗ
- **Nhanh**: Tính toán phải nhanh

### Ví dụ đơn giản:
```cpp
// Hash cho số nguyên
int hash(int key, int table_size) {
    return key % table_size;
}

// Hash cho string
int hash(string key, int table_size) {
    int sum = 0;
    for (char c : key) sum += c;
    return sum % table_size;
}
```

---

## 3. Xử lý xung đột (Collision Handling)

**Collision** xảy ra khi 2 key khác nhau cho ra **cùng index**.

### Cách 1: Separate Chaining (dùng Linked List)
- Mỗi ô là một danh sách liên kết
- Key bị trùng index → thêm vào cuối list

### Cách 2: Open Addressing (Linear Probing)
- Nếu ô đã có người → tìm ô trống tiếp theo
- Đơn giản hơn, không cần linked list

### Cách 3: Open Addressing (Quadratic Probing)
- Thay vì tìm ô kế tiếp (+1, +2, +3...), nhảy theo bình phương (+1, +4, +9...)

---

## 4. Load Factor & Rehashing

```
Load Factor = Số phần tử / Kích thước bảng
```

- Load factor cao → nhiều collision → chậm
- Khi load factor > ngưỡng (thường 0.75) → **Rehashing**: tạo bảng mới lớn hơn, hash lại tất cả

---

## 5. Hash Table trong C++

C++ cung cấp sẵn:

| CTDL | Mô tả | Header |
|------|--------|--------|
| `unordered_map<K,V>` | Hash map (key-value) | `<unordered_map>` |
| `unordered_set<T>` | Hash set (chỉ key) | `<unordered_set>` |
| `map<K,V>` | Cây BST (có thứ tự, O(log n)) | `<map>` |
| `set<T>` | Cây BST (có thứ tự, O(log n)) | `<set>` |

### Khi nào dùng cái nào?
- **unordered_map**: Cần tìm kiếm nhanh O(1), không cần thứ tự
- **map**: Cần duyệt theo thứ tự, hoặc cần O(log n) ổn định (worst case)

---

## 6. So sánh với các CTDL khác

| Thao tác | Array | Linked List | BST (map) | Hash Table |
|----------|-------|-------------|-----------|------------|
| Tìm kiếm | O(n) | O(n) | O(log n) | **O(1)** avg |
| Thêm | O(1)* | O(1) | O(log n) | **O(1)** avg |
| Xóa | O(n) | O(1)** | O(log n) | **O(1)** avg |
| Có thứ tự | Có | Có | Có | **Không** |

> *: Thêm cuối mảng. **: Nếu biết vị trí node.

---

## 7. Ứng dụng trong thuật toán

Hash Table cực kỳ hữu ích trong:

1. **Two Sum**: Tìm 2 số có tổng = target → dùng map lưu số đã gặp
2. **Đếm tần suất**: Đếm số lần xuất hiện của mỗi phần tử
3. **Kiểm tra trùng lặp**: Dùng set kiểm tra phần tử đã tồn tại
4. **Group Anagrams**: Nhóm các chuỗi là hoán vị của nhau
5. **Subarray Sum**: Tìm mảng con có tổng = k
6. **LRU Cache**: Kết hợp hash map + doubly linked list
7. **Graph adjacency list**: Lưu đồ thị

---

## 8. Thứ tự học các file

### Cơ bản (basic/)
| # | File | Nội dung |
|---|------|----------|
| 1 | `01_concept.cpp` | Khái niệm hash table, tự xây dựng bảng băm đơn giản |
| 2 | `02_hash_function.cpp` | Các loại hash function |
| 3 | `03_collision_chaining.cpp` | Xử lý xung đột bằng Separate Chaining |
| 4 | `04_collision_open_addressing.cpp` | Xử lý xung đột bằng Open Addressing |

### Trung cấp (intermediate/)
| # | File | Nội dung |
|---|------|----------|
| 5 | `05_unordered_map_usage.cpp` | Sử dụng unordered_map trong C++ |
| 6 | `06_unordered_set_usage.cpp` | Sử dụng unordered_set trong C++ |
| 7 | `07_load_factor_rehashing.cpp` | Load factor và rehashing |
| 8 | `08_custom_hash.cpp` | Tự viết hash function cho struct/class |

### Ứng dụng thuật toán (applications/)
| # | File | Nội dung |
|---|------|----------|
| 9 | `09_two_sum.cpp` | Bài toán Two Sum kinh điển |
| 10 | `10_frequency_count.cpp` | Đếm tần suất phần tử |
| 11 | `11_group_anagrams.cpp` | Nhóm các anagram |
| 12 | `12_subarray_sum_k.cpp` | Tìm mảng con có tổng = k |
| 13 | `13_longest_consecutive.cpp` | Dãy số liên tiếp dài nhất |

---

> **Lời khuyên**: Hãy đọc file theo thứ tự, chạy từng file, thay đổi input để hiểu rõ hơn. Hash table là CTDL quan trọng nhất trong competitive programming và phỏng vấn!
