# Structured Bindings (C++17)

## Tổng quan

Structured bindings cho phép "unpack" các thành phần của struct, pair, tuple, array vào các biến riêng lẻ trong một dòng.

**Tại sao dùng?**
- Code sạch hơn nhiều: không cần `.first`, `.second`, `std::get<0>()`
- Đặt tên có ý nghĩa cho từng thành phần
- Đọc code dễ hiểu hơn, đặc biệt khi iterate map

> Ref: https://www.learncpp.com/cpp-tutorial/structured-bindings/

---

## Cách sử dụng

### Cơ bản: pair và tuple

```cpp
#include <iostream>
#include <tuple>
#include <string>
#include <map>

std::pair<int, std::string> getUser() {
    return {42, "Alice"};
}

std::tuple<int, double, std::string> getSensorData() {
    return {1, 25.5, "Temperature"};
}

int main() {
    // C++11/14: xấu và khó đọc
    auto user_old = getUser();
    std::cout << user_old.first << " " << user_old.second << '\n';

    // C++17: structured binding - đẹp!
    auto [id, name] = getUser();
    std::cout << id << " " << name << '\n'; // 42 Alice

    // Tuple unpacking
    auto [sensorId, value, sensorName] = getSensorData();
    std::cout << sensorName << "[" << sensorId << "] = " << value << '\n';

    return 0;
}
```

### Iterate map (use case phổ biến nhất)

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    // C++11: verbose
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << '\n';
    }

    // C++17: clean và rõ ràng
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << '\n';
    }

    // Có thể modify nếu không dùng const
    for (auto& [name, score] : scores) {
        score += 5; // tăng điểm cho tất cả
    }

    // Kết hợp với insert result
    auto [iter, inserted] = scores.insert({"David", 88});
    if (inserted) {
        std::cout << "Added: " << iter->first << '\n';
    } else {
        std::cout << "Already exists\n";
    }

    return 0;
}
```

### Struct unpacking

```cpp
#include <iostream>
#include <vector>
#include <string>

struct CANFrame {
    uint32_t id;
    uint8_t dlc;
    std::string data;
};

CANFrame receiveFrame() {
    return {0x123, 8, "DEADBEEF"};
}

int main() {
    // Unpack struct fields
    auto [frameId, dlc, data] = receiveFrame();
    std::cout << "ID: 0x" << std::hex << frameId
              << " DLC: " << std::dec << static_cast<int>(dlc)
              << " Data: " << data << '\n';

    // Dùng trong vector of structs
    std::vector<CANFrame> frames = {
        {0x100, 8, "01020304"},
        {0x200, 4, "AABB"},
        {0x300, 2, "FF"},
    };

    for (const auto& [id, len, payload] : frames) {
        std::cout << "Frame 0x" << std::hex << id
                  << " [" << std::dec << static_cast<int>(len) << "]: "
                  << payload << '\n';
    }

    return 0;
}
```

### Array unpacking

```cpp
#include <iostream>
#include <array>

int main() {
    int coords[] = {10, 20, 30};
    auto [x, y, z] = coords;
    std::cout << "x=" << x << " y=" << y << " z=" << z << '\n';

    std::array<double, 3> rgb = {0.5, 0.8, 0.3};
    auto [r, g, b] = rgb;
    std::cout << "R=" << r << " G=" << g << " B=" << b << '\n';

    return 0;
}
```

### Reference bindings (tránh copy)

```cpp
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::pair<std::string, int> player = {"Player1", 100};

    // const reference: đọc nhưng không copy
    const auto& [name, hp] = player;
    std::cout << name << ": " << hp << "HP\n";

    // Mutable reference: modify trực tiếp
    auto& [playerName, health] = player;
    health -= 20;
    std::cout << playerName << ": " << health << "HP\n"; // 80

    return 0;
}
```

---

## Lỗi thường gặp

### 1. Số biến không khớp số thành phần

```cpp
std::tuple<int, double, std::string> data = {1, 2.0, "hi"};

// SAI: phải unpack ĐÚNG số lượng
// auto [a, b] = data;  // Error: 2 != 3

// ĐÚNG:
auto [a, b, c] = data;

// Nếu không cần hết → dùng std::ignore (chỉ với std::tie)
// Hoặc đặt tên biến _ (convention, không phải feature)
auto [id, _, value] = data; // biến _ vẫn tồn tại
```

### 2. Structured binding không thể dùng với auto type mismatch

```cpp
// SAI: không thể chỉ định type cho từng biến
// auto [int x, double y] = std::make_pair(1, 2.0); // Syntax error

// ĐÚNG: tất cả theo auto
auto [x, y] = std::make_pair(1, 2.0); // x: int, y: double
```

### 3. Không thể move từng phần

```cpp
#include <string>

std::pair<std::string, std::string> getData() {
    return {"hello", "world"};
}

int main() {
    // Move toàn bộ pair
    auto [a, b] = getData(); // OK: move semantics áp dụng cho cả pair

    // Nhưng KHÔNG thể: auto [&&a, b] = getData();
    return 0;
}
```

### 4. Structured binding với class → chỉ khi tất cả members public

```cpp
struct Good {
    int x;
    int y; // Tất cả public → OK
};

class Bad {
    int x;
public:
    int y; // Mix private/public → KHÔNG thể structured binding
};

int main() {
    Good g{1, 2};
    auto [gx, gy] = g; // OK

    // Bad b;
    // auto [bx, by] = b; // Error!
    return 0;
}
```

---

## Ứng dụng thực tế

```cpp
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>

// Function trả về nhiều giá trị
struct Result {
    bool success;
    int errorCode;
    std::string message;
};

Result processMessage(int msgId) {
    if (msgId < 0) return {false, -1, "Invalid ID"};
    if (msgId > 1000) return {false, -2, "ID out of range"};
    return {true, 0, "OK"};
}

int main() {
    // Pattern: check result và xử lý
    if (auto [ok, code, msg] = processMessage(42); ok) {
        std::cout << "Success: " << msg << '\n';
    } else {
        std::cout << "Error " << code << ": " << msg << '\n';
    }

    // Pattern: find min/max cùng lúc
    std::vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6};
    auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());
    std::cout << "Min: " << *minIt << " Max: " << *maxIt << '\n';

    return 0;
}
```

---

## Compile

```bash
g++ -std=c++17 -o program main.cpp
```
