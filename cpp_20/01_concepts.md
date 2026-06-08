# Concepts (C++20)

## Tổng quan

Concepts cho phép đặt **ràng buộc (constraints)** lên template parameters. Thay vì template chấp nhận mọi type rồi lỗi compile khó hiểu, concepts cho biết rõ ràng type cần đáp ứng gì.

**Tại sao dùng?**
- Error messages dễ đọc hơn 100 lần so với SFINAE
- Self-documenting: đọc code biết ngay type cần gì
- Thay thế `enable_if`, `static_assert` checks
- Overload resolution rõ ràng dựa trên constraints

> Ref: https://www.learncpp.com/cpp-tutorial/introduction-to-concepts/

---

## Cách sử dụng

### Dùng standard concepts

```cpp
#include <iostream>
#include <concepts>
#include <string>
#include <vector>

// Chỉ chấp nhận integer types
template<std::integral T>
T add(T a, T b) {
    return a + b;
}

// Chỉ chấp nhận floating point
template<std::floating_point T>
T divide(T a, T b) {
    return a / b;
}

// Chấp nhận bất kỳ type nào copyable
template<std::copyable T>
void store(T value) {
    std::cout << "Stored: " << value << '\n';
}

int main() {
    std::cout << add(3, 4) << '\n';       // OK: int is integral
    std::cout << add(10L, 20L) << '\n';   // OK: long is integral
    // add(3.14, 2.0);                    // Error: double is NOT integral

    std::cout << divide(10.0, 3.0) << '\n';  // OK
    // divide(10, 3);                         // Error: int is NOT floating_point

    store(42);
    store(std::string("hello"));
    return 0;
}
```

### Viết concept tự định nghĩa

```cpp
#include <iostream>
#include <concepts>
#include <string>
#include <vector>

// Concept: type phải có .size() và hỗ trợ range-for
template<typename T>
concept Container = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
    { t.begin() };
    { t.end() };
};

// Concept: type phải có operator<< cho std::ostream
template<typename T>
concept Printable = requires(std::ostream& os, T t) {
    { os << t } -> std::same_as<std::ostream&>;
};

// Concept kết hợp
template<typename T>
concept PrintableContainer = Container<T> && requires(T t) {
    requires Printable<decltype(*t.begin())>;
};

// Dùng concept
template<PrintableContainer T>
void printAll(const T& container) {
    std::cout << "[" << container.size() << " items]: ";
    for (const auto& item : container) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"hello", "world"};

    printAll(nums);   // [5 items]: 1 2 3 4 5
    printAll(words);  // [2 items]: hello world

    // printAll(42); // Error: int does not satisfy PrintableContainer
    return 0;
}
```

### Syntax variations (3 cách viết)

```cpp
#include <iostream>
#include <concepts>

// === Cách 1: requires clause ===
template<typename T>
requires std::integral<T>
T gcd_v1(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// === Cách 2: Constrained template parameter (ngắn nhất) ===
template<std::integral T>
T gcd_v2(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// === Cách 3: Trailing requires clause ===
template<typename T>
T gcd_v3(T a, T b) requires std::integral<T> {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// === Cách 4: Abbreviated function template (auto + concept) ===
auto gcd_v4(std::integral auto a, std::integral auto b) {
    while (b != 0) {
        auto temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    std::cout << gcd_v1(12, 8) << '\n'; // 4
    std::cout << gcd_v2(24, 36) << '\n'; // 12
    std::cout << gcd_v3(100, 75) << '\n'; // 25
    std::cout << gcd_v4(48, 18) << '\n'; // 6
    return 0;
}
```

### Concept cho class interface (thay thế pure virtual)

```cpp
#include <iostream>
#include <concepts>
#include <string>
#include <vector>
#include <memory>

// Định nghĩa "interface" bằng concept
template<typename T>
concept Serializable = requires(T t, std::string data) {
    { t.serialize() } -> std::convertible_to<std::string>;
    { T::deserialize(data) } -> std::same_as<T>;
};

// Implement
struct SensorData {
    int id;
    double value;

    std::string serialize() const {
        return std::to_string(id) + ":" + std::to_string(value);
    }

    static SensorData deserialize(const std::string& data) {
        auto pos = data.find(':');
        return {
            std::stoi(data.substr(0, pos)),
            std::stod(data.substr(pos + 1))
        };
    }
};

// Function chỉ chấp nhận Serializable types
template<Serializable T>
void saveToFile(const T& obj) {
    std::string data = obj.serialize();
    std::cout << "Saving: " << data << '\n';
    // file write logic...
}

template<Serializable T>
T loadFromFile(const std::string& data) {
    std::cout << "Loading: " << data << '\n';
    return T::deserialize(data);
}

int main() {
    SensorData sensor{42, 25.5};
    saveToFile(sensor);

    auto loaded = loadFromFile<SensorData>("100:37.2");
    std::cout << "ID: " << loaded.id << " Value: " << loaded.value << '\n';

    return 0;
}
```

### Overloading với concepts (more constrained wins)

```cpp
#include <iostream>
#include <concepts>
#include <vector>
#include <string>

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// General version
template<typename T>
std::string toString(const T& value) {
    return "unknown";
}

// More specific: numeric types
template<Numeric T>
std::string toString(const T& value) {
    return std::to_string(value);
}

// Even more specific: integral types (subsumes Numeric)
template<std::integral T>
std::string toString(const T& value) {
    return "int:" + std::to_string(value);
}

int main() {
    std::cout << toString(42) << '\n';     // int:42 (most specific)
    std::cout << toString(3.14) << '\n';   // 3.140000 (Numeric)
    std::cout << toString("hi") << '\n';   // unknown (general)

    return 0;
}
```

---

## Standard concepts phổ biến (header `<concepts>`)

| Concept | Ý nghĩa |
|---------|----------|
| `std::integral` | int, char, long, etc. |
| `std::floating_point` | float, double |
| `std::same_as<T, U>` | T và U cùng type |
| `std::convertible_to<From, To>` | From convert được sang To |
| `std::copyable` | Copy construct + copy assign |
| `std::movable` | Move construct + move assign |
| `std::invocable<F, Args...>` | F gọi được với Args |
| `std::ranges::range` | Có begin() và end() |

---

## Lỗi thường gặp

### 1. Concept quá strict

```cpp
// Quá strict: yêu cầu CHÍNH XÁC size_t
template<typename T>
concept BadContainer = requires(T t) {
    { t.size() } -> std::same_as<std::size_t>; // Một số container trả về type khác!
};

// Tốt hơn: chấp nhận convertible
template<typename T>
concept GoodContainer = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};
```

### 2. Quên requires expression syntax

```cpp
// SAI: thiếu braces cho compound requirement
template<typename T>
concept Bad = requires(T t) {
    t.size() -> std::convertible_to<int>; // Syntax error!
};

// ĐÚNG: dùng { expr } -> concept
template<typename T>
concept Good = requires(T t) {
    { t.size() } -> std::convertible_to<int>;
};
```

### 3. Circular concepts

```cpp
// SAI: concept không thể recursive
// template<typename T>
// concept Node = requires(T t) {
//     { t.next() } -> Node; // Error: circular!
// };
```

---

## Compile

```bash
g++ -std=c++20 -o program main.cpp
```
