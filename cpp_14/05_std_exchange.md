# std::exchange (C++14)

## Tổng quan

`std::exchange(obj, new_value)` gán `new_value` vào `obj` và **trả về giá trị cũ** của `obj`. Atomic-style operation nhưng cho single thread.

**Tại sao dùng?**
- Move constructor/assignment: đặt source về trạng thái valid rõ ràng
- Implement "take ownership" pattern
- Flag: set-and-check trong 1 expression
- Cleaner code thay vì temp variable + assign + return

---

## Cách sử dụng

### Cơ bản

```cpp
#include <iostream>
#include <utility>
#include <string>
#include <vector>

int main() {
    int x = 42;
    int old = std::exchange(x, 0); // x = 0, old = 42
    
    std::cout << "old = " << old << '\n'; // 42
    std::cout << "x   = " << x << '\n';   // 0

    std::string name = "Hello";
    std::string prev = std::exchange(name, "World");
    std::cout << "prev = " << prev << '\n'; // Hello
    std::cout << "name = " << name << '\n'; // World

    return 0;
}
```

### Move constructor pattern (use case phổ biến nhất)

```cpp
#include <iostream>
#include <utility>
#include <string>
#include <cstdint>

class CANSocket {
    int m_fd;              // file descriptor (-1 = invalid)
    std::string m_iface;   // "can0", "vcan0"
    uint32_t m_baudrate;

public:
    CANSocket(std::string iface, uint32_t baud)
        : m_fd(42) // giả lập open socket
        , m_iface(std::move(iface))
        , m_baudrate(baud) {
        std::cout << "Opened " << m_iface << " @ " << m_baudrate << '\n';
    }

    ~CANSocket() {
        if (m_fd >= 0) {
            std::cout << "Closing fd=" << m_fd << " (" << m_iface << ")\n";
            // close(m_fd);
        }
    }

    // Move constructor - std::exchange làm source sạch sẽ
    CANSocket(CANSocket&& other) noexcept
        : m_fd(std::exchange(other.m_fd, -1))           // lấy fd, set source = -1
        , m_iface(std::exchange(other.m_iface, ""))     // lấy name, set source = ""
        , m_baudrate(std::exchange(other.m_baudrate, 0))
    {}

    // Move assignment
    CANSocket& operator=(CANSocket&& other) noexcept {
        if (this != &other) {
            // Close current resource
            if (m_fd >= 0) {
                std::cout << "Closing old fd=" << m_fd << '\n';
            }
            m_fd = std::exchange(other.m_fd, -1);
            m_iface = std::exchange(other.m_iface, "");
            m_baudrate = std::exchange(other.m_baudrate, 0);
        }
        return *this;
    }

    // Non-copyable
    CANSocket(const CANSocket&) = delete;
    CANSocket& operator=(const CANSocket&) = delete;

    bool isOpen() const { return m_fd >= 0; }
    const std::string& interface() const { return m_iface; }
};

int main() {
    CANSocket sock1("can0", 500000);
    std::cout << "sock1 open: " << sock1.isOpen() << '\n'; // 1

    CANSocket sock2 = std::move(sock1);
    std::cout << "sock1 open: " << sock1.isOpen() << '\n'; // 0 (moved-from)
    std::cout << "sock2 open: " << sock2.isOpen() << '\n'; // 1
    std::cout << "sock2 iface: " << sock2.interface() << '\n'; // can0

    return 0;
}
```

### One-shot flag pattern

```cpp
#include <iostream>
#include <utility>
#include <thread>
#include <vector>
#include <functional>

class OneShot {
    bool m_fired = false;
    std::function<void()> m_callback;

public:
    explicit OneShot(std::function<void()> cb)
        : m_callback(std::move(cb)) {}

    void fire() {
        // Set-and-check trong 1 expression
        if (!std::exchange(m_fired, true)) {
            // Chỉ chạy lần đầu tiên
            m_callback();
        } else {
            std::cout << "Already fired, ignoring\n";
        }
    }
};

int main() {
    OneShot trigger([]() {
        std::cout << "TRIGGERED! Initializing system...\n";
    });

    trigger.fire(); // TRIGGERED!
    trigger.fire(); // Already fired, ignoring
    trigger.fire(); // Already fired, ignoring

    return 0;
}
```

### Linked list: take ownership of next node

```cpp
#include <iostream>
#include <utility>
#include <memory>
#include <string>

template<typename T>
class SimpleQueue {
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(T d) : data(std::move(d)) {}
    };

    std::unique_ptr<Node> m_head;
    Node* m_tail = nullptr;

public:
    void push(T value) {
        auto node = std::make_unique<Node>(std::move(value));
        Node* raw = node.get();
        if (m_tail) {
            m_tail->next = std::move(node);
        } else {
            m_head = std::move(node);
        }
        m_tail = raw;
    }

    // Pop: take ownership of head's data
    T pop() {
        // exchange: lấy head, set head = head->next
        auto oldHead = std::exchange(m_head, std::move(m_head->next));
        if (!m_head) m_tail = nullptr;
        return std::move(oldHead->data);
    }

    bool empty() const { return !m_head; }
};

int main() {
    SimpleQueue<std::string> q;
    q.push("first");
    q.push("second");
    q.push("third");

    while (!q.empty()) {
        std::cout << q.pop() << '\n';
    }
    return 0;
}
```

### State machine transitions

```cpp
#include <iostream>
#include <utility>
#include <string>

enum class State { Idle, Connecting, Connected, Error };

std::string stateName(State s) {
    switch (s) {
        case State::Idle: return "Idle";
        case State::Connecting: return "Connecting";
        case State::Connected: return "Connected";
        case State::Error: return "Error";
    }
    return "?";
}

class Connection {
    State m_state = State::Idle;

    void onLeave(State old) {
        std::cout << "  Leaving: " << stateName(old) << '\n';
    }
    void onEnter(State current) {
        std::cout << "  Entering: " << stateName(current) << '\n';
    }

public:
    void transition(State newState) {
        // exchange: lấy old state + set new state
        State oldState = std::exchange(m_state, newState);
        
        if (oldState != newState) {
            onLeave(oldState);
            onEnter(newState);
        }
    }

    State state() const { return m_state; }
};

int main() {
    Connection conn;
    conn.transition(State::Connecting);
    conn.transition(State::Connected);
    conn.transition(State::Error);
    conn.transition(State::Idle);
    return 0;
}
```

---

## Lỗi thường gặp

### 1. Nhầm thứ tự return và assign

```cpp
int x = 10;

// std::exchange trả về GIÁ TRỊ CŨ
int old = std::exchange(x, 20);
// old = 10 (giá trị cũ)
// x = 20 (giá trị mới)

// KHÔNG PHẢI:
// old = 20, x = 10 ← SAI!
```

### 2. exchange KHÔNG phải atomic

```cpp
#include <atomic>

int shared = 0;

// SAI: exchange không thread-safe
// std::exchange(shared, 1); // DATA RACE nếu nhiều threads

// ĐÚNG: dùng atomic cho shared data
std::atomic<int> atomicShared{0};
int prev = atomicShared.exchange(1); // thread-safe
```

### 3. Đừng quá lạm dụng - readability

```cpp
// Quá clever → khó đọc
auto result = std::exchange(container, std::exchange(backup, {}));

// Rõ ràng hơn:
auto result = std::move(container);
container = std::move(backup);
backup = {};
```

---

## Khi nào dùng std::exchange?

| Pattern | Ví dụ |
|---------|-------|
| Move constructor/assignment | `m_fd(std::exchange(other.m_fd, -1))` |
| Take ownership | `auto old = std::exchange(m_ptr, nullptr)` |
| One-shot flag | `if (!std::exchange(m_done, true))` |
| State transition | `auto prev = std::exchange(m_state, newState)` |
| Reset and return | `return std::exchange(m_buffer, {})` |

---

## Compile

```bash
g++ -std=c++14 -o program main.cpp
```
