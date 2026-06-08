# std::optional (C++17)

## Tổng quan

`std::optional<T>` là container chứa **0 hoặc 1** giá trị. Thay thế cho các pattern xấu như: return -1, return nullptr, output parameter, special sentinel value.

**Tại sao dùng?**
- Diễn đạt rõ ràng "có thể không có giá trị" trong type system
- An toàn hơn pointer (không cần delete, không dangling)
- Thay thế `std::pair<T, bool>` pattern
- Self-documenting: đọc function signature biết ngay có thể fail

> Ref: https://www.learncpp.com/cpp-tutorial/stdoptional/

---

## Cách sử dụng

### Cơ bản

```cpp
#include <iostream>
#include <optional>
#include <string>

// Trả về optional thay vì magic value (-1, "", nullptr)
std::optional<int> findIndex(const std::string& text, char c) {
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == c) {
            return static_cast<int>(i); // có giá trị
        }
    }
    return std::nullopt; // không có giá trị
    // hoặc: return {};
}

int main() {
    auto result = findIndex("Hello", 'l');

    // Cách 1: check bằng bool conversion
    if (result) {
        std::cout << "Found at index: " << *result << '\n'; // 2
    } else {
        std::cout << "Not found\n";
    }

    // Cách 2: has_value()
    auto result2 = findIndex("Hello", 'z');
    if (result2.has_value()) {
        std::cout << "Found: " << result2.value() << '\n';
    } else {
        std::cout << "Not found\n";
    }

    // Cách 3: value_or() - cung cấp default
    int idx = findIndex("Hello", 'z').value_or(-1);
    std::cout << "Index: " << idx << '\n'; // -1

    return 0;
}
```

### Function trả về complex object

```cpp
#include <iostream>
#include <optional>
#include <string>
#include <map>

struct UserConfig {
    std::string name;
    int timeout;
    bool debug;
};

class ConfigManager {
    std::map<std::string, UserConfig> configs_;

public:
    ConfigManager() {
        configs_["default"] = {"Default", 30, false};
        configs_["debug"] = {"Debug Mode", 60, true};
    }

    // Trả về optional reference wrapper (tránh copy)
    std::optional<UserConfig> getConfig(const std::string& key) const {
        auto it = configs_.find(key);
        if (it != configs_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

int main() {
    ConfigManager mgr;

    if (auto config = mgr.getConfig("debug")) {
        std::cout << "Name: " << config->name << '\n';
        std::cout << "Timeout: " << config->timeout << '\n';
        std::cout << "Debug: " << std::boolalpha << config->debug << '\n';
    }

    if (auto config = mgr.getConfig("nonexistent")) {
        std::cout << "Found\n";
    } else {
        std::cout << "Config not found\n";
    }

    return 0;
}
```

### Optional với parsing / validation

```cpp
#include <iostream>
#include <optional>
#include <string>
#include <charconv>

// Parse string to int, trả về nullopt nếu invalid
std::optional<int> parseInt(const std::string& str) {
    int value{};
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
    if (ec == std::errc() && ptr == str.data() + str.size()) {
        return value;
    }
    return std::nullopt;
}

// Chain multiple optionals
std::optional<double> divide(double a, double b) {
    if (b == 0.0) return std::nullopt;
    return a / b;
}

int main() {
    // Parsing
    auto num1 = parseInt("42");
    auto num2 = parseInt("abc");
    auto num3 = parseInt("12.3");

    std::cout << "42  → " << num1.value_or(-1) << '\n';  // 42
    std::cout << "abc → " << num2.value_or(-1) << '\n';  // -1
    std::cout << "12.3→ " << num3.value_or(-1) << '\n';  // -1

    // Division
    auto result = divide(10.0, 3.0);
    if (result) {
        std::cout << "10/3 = " << *result << '\n';
    }

    auto bad = divide(10.0, 0.0);
    std::cout << "10/0 = " << bad.value_or(0.0) << '\n'; // 0.0

    return 0;
}
```

### Optional trong class members

```cpp
#include <iostream>
#include <optional>
#include <string>

class NetworkConnection {
    std::string host_;
    int port_;
    std::optional<std::string> username_;  // có thể không cần auth
    std::optional<std::string> password_;
    std::optional<int> timeout_;           // dùng default nếu không set

public:
    NetworkConnection(std::string host, int port)
        : host_(std::move(host)), port_(port) {}

    // Builder pattern với optional
    NetworkConnection& withAuth(std::string user, std::string pass) {
        username_ = std::move(user);
        password_ = std::move(pass);
        return *this;
    }

    NetworkConnection& withTimeout(int ms) {
        timeout_ = ms;
        return *this;
    }

    void connect() const {
        std::cout << "Connecting to " << host_ << ":" << port_ << '\n';
        if (username_) {
            std::cout << "  Auth: " << *username_ << '\n';
        }
        std::cout << "  Timeout: " << timeout_.value_or(5000) << "ms\n";
    }
};

int main() {
    NetworkConnection conn1("192.168.1.1", 8080);
    conn1.connect();
    // Connecting to 192.168.1.1:8080
    //   Timeout: 5000ms

    NetworkConnection conn2("10.0.0.1", 443);
    conn2.withAuth("admin", "secret").withTimeout(3000);
    conn2.connect();
    // Connecting to 10.0.0.1:443
    //   Auth: admin
    //   Timeout: 3000ms

    return 0;
}
```

---

## Lỗi thường gặp

### 1. Truy cập value khi empty → undefined behavior hoặc exception

```cpp
std::optional<int> empty;

// SAI: UB nếu dùng * khi empty
// int x = *empty;  // Undefined Behavior!

// SAI: .value() throw std::bad_optional_access
// int y = empty.value(); // throws exception

// ĐÚNG: check trước
if (empty) {
    int x = *empty;
}

// ĐÚNG: dùng value_or
int z = empty.value_or(0);
```

### 2. Optional<reference> không tồn tại

```cpp
// SAI: không compile
// std::optional<int&> ref;

// Dùng std::reference_wrapper hoặc pointer
#include <functional>
int x = 42;
std::optional<std::reference_wrapper<int>> ref = x;
ref->get() = 100;
// Nhưng thường đơn giản dùng pointer cho trường hợp này
```

### 3. So sánh optional

```cpp
#include <optional>

std::optional<int> a = 5;
std::optional<int> b = 10;
std::optional<int> empty;

// OK: so sánh 2 optionals
bool r1 = (a < b);      // true (5 < 10)
bool r2 = (a < empty);  // false (có value > empty)
bool r3 = (empty < a);  // true (empty < có value)

// OK: so sánh với value trực tiếp
bool r4 = (a == 5);     // true
bool r5 = (empty == 5); // false
```

### 4. Performance: optional thêm 1 byte (hoặc hơn do alignment)

```cpp
#include <iostream>
#include <optional>

struct Small { char c; };

int main() {
    std::cout << sizeof(int) << '\n';                  // 4
    std::cout << sizeof(std::optional<int>) << '\n';   // 8 (4 + bool + padding)
    std::cout << sizeof(Small) << '\n';                // 1
    std::cout << sizeof(std::optional<Small>) << '\n'; // 2
    return 0;
}
```

---

## Khi nào dùng optional vs alternatives

| Tình huống | Dùng |
|------------|------|
| Function có thể fail, trả về value | `std::optional<T>` |
| Function có thể fail, cần error info | `std::variant<T, Error>` hoặc exception |
| Nullable pointer semantics | `T*` hoặc `std::unique_ptr<T>` |
| Optional parameter | `std::optional<T>` hoặc overload |
| Container rỗng | `std::vector<T>` (empty vector) |

---

## Compile

```bash
g++ -std=c++17 -o program main.cpp
```
