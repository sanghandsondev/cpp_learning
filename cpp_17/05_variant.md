# std::variant (C++17)

## Tổng quan

`std::variant<Types...>` là **type-safe union**: chứa ĐÚNG 1 giá trị từ danh sách types tại bất kỳ thời điểm nào.

**Tại sao dùng?**
- State machine: mỗi state có data khác nhau
- Message/event types: CAN frame, serial, network...
- Error handling: `variant<Result, Error>` thay exception
- Thay thế C union + enum tag (unsafe) hoặc inheritance + virtual (overhead)
- Zero heap allocation (stack-based)

---

## Cách sử dụng

### Cơ bản

```cpp
#include <iostream>
#include <variant>
#include <string>

int main() {
    // Variant chứa int HOẶC double HOẶC string
    std::variant<int, double, std::string> value;

    value = 42;
    std::cout << std::get<int>(value) << '\n'; // 42

    value = 3.14;
    std::cout << std::get<double>(value) << '\n'; // 3.14

    value = std::string("Hello");
    std::cout << std::get<std::string>(value) << '\n'; // Hello

    // Check type hiện tại
    std::cout << "Index: " << value.index() << '\n'; // 2 (string)

    // holds_alternative
    if (std::holds_alternative<std::string>(value)) {
        std::cout << "Is string: " << std::get<std::string>(value) << '\n';
    }

    // get_if: trả về pointer (nullptr nếu sai type)
    if (auto* ptr = std::get_if<int>(&value)) {
        std::cout << "Int: " << *ptr << '\n';
    } else {
        std::cout << "Not an int\n";
    }

    return 0;
}
```

### std::visit - pattern matching

```cpp
#include <iostream>
#include <variant>
#include <string>
#include <vector>

// Message types
struct TextMessage {
    std::string content;
};

struct BinaryMessage {
    std::vector<uint8_t> data;
};

struct ControlMessage {
    int command;
    int param;
};

using Message = std::variant<TextMessage, BinaryMessage, ControlMessage>;

// Visitor pattern: xử lý từng type
struct MessageHandler {
    void operator()(const TextMessage& msg) const {
        std::cout << "Text: " << msg.content << '\n';
    }
    void operator()(const BinaryMessage& msg) const {
        std::cout << "Binary [" << msg.data.size() << " bytes]: ";
        for (auto b : msg.data) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << std::dec << '\n';
    }
    void operator()(const ControlMessage& msg) const {
        std::cout << "Control: cmd=" << msg.command << " param=" << msg.param << '\n';
    }
};

int main() {
    std::vector<Message> inbox = {
        TextMessage{"Hello World"},
        BinaryMessage{{0xDE, 0xAD, 0xBE, 0xEF}},
        ControlMessage{1, 100},
        TextMessage{"Goodbye"},
    };

    // Process tất cả messages
    for (const auto& msg : inbox) {
        std::visit(MessageHandler{}, msg);
    }

    return 0;
}
```

### Overloaded lambda pattern (modern visitor)

```cpp
#include <iostream>
#include <variant>
#include <string>

// Helper: overloaded lambdas
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // deduction guide

using Value = std::variant<int, double, std::string, bool>;

void printValue(const Value& v) {
    std::visit(overloaded{
        [](int i)                { std::cout << "int: " << i << '\n'; },
        [](double d)             { std::cout << "double: " << d << '\n'; },
        [](const std::string& s) { std::cout << "string: " << s << '\n'; },
        [](bool b)               { std::cout << "bool: " << std::boolalpha << b << '\n'; },
    }, v);
}

int main() {
    std::vector<Value> values = {42, 3.14, std::string("hello"), true};
    for (const auto& v : values) {
        printValue(v);
    }
    return 0;
}
```

### State machine (real-world)

```cpp
#include <iostream>
#include <variant>
#include <string>

// States with associated data
struct Disconnected {};
struct Connecting { std::string host; int port; };
struct Connected { int socketFd; std::string host; };
struct Error { int code; std::string message; };

using ConnectionState = std::variant<Disconnected, Connecting, Connected, Error>;

// Events
struct ConnectEvent { std::string host; int port; };
struct ConnectedEvent { int fd; };
struct DisconnectEvent {};
struct ErrorEvent { int code; std::string msg; };

class StateMachine {
    ConnectionState m_state = Disconnected{};

public:
    void handle(const ConnectEvent& e) {
        std::visit(overloaded{
            [&](Disconnected) {
                std::cout << "Connecting to " << e.host << ":" << e.port << "...\n";
                m_state = Connecting{e.host, e.port};
            },
            [](auto&) {
                std::cout << "Cannot connect: not in disconnected state\n";
            }
        }, m_state);
    }

    void handle(const ConnectedEvent& e) {
        std::visit(overloaded{
            [&](const Connecting& s) {
                std::cout << "Connected to " << s.host << " (fd=" << e.fd << ")\n";
                m_state = Connected{e.fd, s.host};
            },
            [](auto&) {
                std::cout << "Unexpected connected event\n";
            }
        }, m_state);
    }

    void handle(const DisconnectEvent&) {
        std::visit(overloaded{
            [&](Connected& s) {
                std::cout << "Disconnecting from " << s.host << "\n";
                m_state = Disconnected{};
            },
            [&](auto&) {
                m_state = Disconnected{};
            }
        }, m_state);
    }

    void handle(const ErrorEvent& e) {
        std::cout << "Error: " << e.msg << " (code " << e.code << ")\n";
        m_state = Error{e.code, e.msg};
    }

    void printState() const {
        std::visit(overloaded{
            [](const Disconnected&) { std::cout << "State: Disconnected\n"; },
            [](const Connecting& s) { std::cout << "State: Connecting to " << s.host << "\n"; },
            [](const Connected& s)  { std::cout << "State: Connected (" << s.host << ")\n"; },
            [](const Error& s)      { std::cout << "State: Error - " << s.message << "\n"; },
        }, m_state);
    }
};

// overloaded helper (same as above)
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    StateMachine sm;
    sm.printState();

    sm.handle(ConnectEvent{"192.168.1.1", 8080});
    sm.printState();

    sm.handle(ConnectedEvent{5});
    sm.printState();

    sm.handle(DisconnectEvent{});
    sm.printState();

    return 0;
}
```

### Error handling pattern (Result type)

```cpp
#include <iostream>
#include <variant>
#include <string>
#include <vector>

struct ParseError {
    int line;
    std::string reason;
};

template<typename T>
using Result = std::variant<T, ParseError>;

struct Config {
    std::string name;
    int value;
};

Result<Config> parseConfig(const std::string& input) {
    if (input.empty()) {
        return ParseError{0, "Empty input"};
    }

    auto pos = input.find('=');
    if (pos == std::string::npos) {
        return ParseError{1, "Missing '=' delimiter"};
    }

    try {
        return Config{
            input.substr(0, pos),
            std::stoi(input.substr(pos + 1))
        };
    } catch (...) {
        return ParseError{1, "Invalid integer value"};
    }
}

int main() {
    auto results = {
        parseConfig("timeout=30"),
        parseConfig(""),
        parseConfig("invalid"),
        parseConfig("port=abc"),
    };

    for (const auto& result : results) {
        std::visit(overloaded{
            [](const Config& c) {
                std::cout << "OK: " << c.name << " = " << c.value << '\n';
            },
            [](const ParseError& e) {
                std::cout << "Error (line " << e.line << "): " << e.reason << '\n';
            }
        }, result);
    }

    return 0;
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
```

---

## Lỗi thường gặp

### 1. std::get throw nếu sai type

```cpp
std::variant<int, std::string> v = "hello";

// CRASH: throws std::bad_variant_access
// int x = std::get<int>(v);

// ĐÚNG: check trước
if (std::holds_alternative<int>(v)) {
    int x = std::get<int>(v);
}

// HOẶC: dùng get_if (trả nullptr nếu sai)
if (auto* p = std::get_if<int>(&v)) {
    std::cout << *p;
}
```

### 2. variant "valueless by exception"

```cpp
struct ThrowOnCopy {
    ThrowOnCopy() = default;
    ThrowOnCopy(const ThrowOnCopy&) { throw std::runtime_error("oops"); }
};

std::variant<int, ThrowOnCopy> v = 42;
try {
    ThrowOnCopy t;
    v = t; // throw during assignment
} catch (...) {
    // v có thể "valueless_by_exception"!
    if (v.valueless_by_exception()) {
        std::cout << "Variant is in invalid state!\n";
    }
}
```

### 3. Bool conversion gotcha

```cpp
// CẢNH BÁO: string literal prefer bool over std::string!
std::variant<bool, std::string> v = "hello"; // Chọn BOOL, không phải string!
std::cout << std::holds_alternative<bool>(v) << '\n'; // true!

// ĐÚNG: cast rõ ràng
std::variant<bool, std::string> v2 = std::string("hello");
```

---

## So sánh với alternatives

| Approach | Heap alloc | Type-safe | Extensible | Performance |
|----------|-----------|-----------|------------|-------------|
| `std::variant` | ❌ No | ✅ | Fixed types | ⚡ Fast |
| `std::any` | ✅ Possible | ⚠️ Runtime | Open types | Slower |
| `inheritance + virtual` | ✅ Yes | ✅ | Open types | Virtual call overhead |
| C `union` + `enum` | ❌ No | ❌ Unsafe | Fixed types | ⚡ Fast |

---

## Compile

```bash
g++ -std=c++17 -o program main.cpp
```
