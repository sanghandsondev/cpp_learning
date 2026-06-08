# Generic Lambdas (C++14)

## Tổng quan

C++14 cho phép dùng `auto` làm parameter type trong lambda → lambda trở thành **template function**.

**Tại sao dùng?**
- Viết code ngắn gọn hơn, không cần khai báo type cụ thể
- Tái sử dụng lambda cho nhiều kiểu dữ liệu khác nhau
- Thay thế nhiều trường hợp phải viết function template dài dòng

> Ref: https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/

---

## Cách sử dụng

### Cơ bản: auto parameter

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // C++11: phải chỉ định type cụ thể
    auto add_old = [](int a, int b) { return a + b; };

    // C++14: dùng auto → hoạt động với mọi type
    auto add = [](auto a, auto b) { return a + b; };

    std::cout << add(3, 4) << '\n';        // int: 7
    std::cout << add(1.5, 2.3) << '\n';    // double: 3.8
    std::cout << add(std::string("Hello "), std::string("World")) << '\n'; // string

    return 0;
}
```

### Dùng với STL algorithms

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {5, 2, 8, 1, 9};
    std::vector<std::string> words = {"banana", "apple", "cherry"};

    // Một lambda dùng cho cả int và string
    auto print = [](const auto& item) {
        std::cout << item << ' ';
    };

    std::for_each(nums.begin(), nums.end(), print);
    std::cout << '\n';

    std::for_each(words.begin(), words.end(), print);
    std::cout << '\n';

    // Sort với generic comparator
    auto descending = [](const auto& a, const auto& b) {
        return a > b;
    };

    std::sort(nums.begin(), nums.end(), descending);
    std::sort(words.begin(), words.end(), descending);

    std::for_each(nums.begin(), nums.end(), print); // 9 8 5 2 1
    std::cout << '\n';

    return 0;
}
```

### Nhiều auto parameters với type khác nhau

```cpp
#include <iostream>
#include <map>
#include <algorithm>

int main() {
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // auto tự suy ra pair<const string, int>
    auto print_entry = [](const auto& pair) {
        std::cout << pair.first << ": " << pair.second << '\n';
    };

    for (const auto& entry : scores) {
        print_entry(entry);
    }

    return 0;
}
```

### Generic lambda lưu vào biến và truyền như callback

```cpp
#include <iostream>
#include <vector>
#include <functional>

// Nhận callback dạng generic
template <typename Func>
void process_data(const std::vector<int>& data, Func callback) {
    for (const auto& item : data) {
        callback(item);
    }
}

int main() {
    std::vector<int> values = {10, 20, 30, 40, 50};

    auto doubler = [](auto x) {
        std::cout << x * 2 << ' ';
    };

    process_data(values, doubler); // 20 40 60 80 100
    std::cout << '\n';

    return 0;
}
```

---

## Lỗi thường gặp

### 1. Quên `const auto&` → copy không cần thiết

```cpp
// SAI: copy mỗi lần gọi (chậm với object lớn)
auto bad = [](auto x) { std::cout << x; };

// ĐÚNG: dùng const reference
auto good = [](const auto& x) { std::cout << x; };
```

### 2. auto không hoạt động với initializer_list trực tiếp

```cpp
// SAI: compiler không suy ra được type
// auto bad = [](auto... args) { return {args...}; };

// ĐÚNG: chỉ định return type
auto good = [](auto a, auto b) -> std::vector<decltype(a)> {
    return {a, b};
};
```

### 3. Không thể lưu generic lambda vào std::function dễ dàng

```cpp
// std::function cần type cụ thể
// std::function<???> f = [](auto x) { return x; }; // Không compile

// Phải chỉ định type khi lưu vào std::function
std::function<int(int)> f = [](auto x) { return x; };
// Nhưng lúc này chỉ dùng được với int
```

---

## Khi nào KHÔNG nên dùng

- Khi cần debug: lỗi template thường khó đọc
- Khi type cụ thể giúp code rõ ràng hơn (self-documenting)
- Khi lambda phức tạp → nên viết thành function template với tên rõ ràng

---

## Compile

```bash
g++ -std=c++14 -o program main.cpp
```
