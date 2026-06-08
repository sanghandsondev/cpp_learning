# Coroutines (C++20)

## Tổng quan

Coroutines cho phép **suspend và resume** function execution. Dùng `co_await`, `co_yield`, `co_return` để viết async code trông giống synchronous.

**Tại sao dùng?**
- Async I/O không cần callback hell
- Generator patterns (lazy sequences)
- Cooperative multitasking (lightweight threads)
- Event-driven programming (GUI, network, embedded)

**Lưu ý:** C++20 cung cấp cơ chế (machinery), nhưng KHÔNG có ready-to-use types. Thực tế dùng library (cppcoro, folly, boost.asio) hoặc tự viết.

---

## Cách sử dụng

### Generator (co_yield) - Use case phổ biến nhất

```cpp
#include <iostream>
#include <coroutine>
#include <optional>
#include <cstdint>

// Simple Generator type
template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = std::move(value);
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle h) : m_handle(h) {}
    ~Generator() { if (m_handle) m_handle.destroy(); }

    // Non-copyable, movable
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : m_handle(std::exchange(other.m_handle, nullptr)) {}

    // Iterator interface
    bool next() {
        m_handle.resume();
        return !m_handle.done();
    }

    T value() const { return m_handle.promise().current_value; }

private:
    Handle m_handle;
};

// === USAGE: Generator functions ===

// Fibonacci sequence (infinite, lazy)
Generator<uint64_t> fibonacci() {
    uint64_t a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

// Range generator
Generator<int> range(int start, int end, int step = 1) {
    for (int i = start; i < end; i += step) {
        co_yield i;
    }
}

// Filter generator (composable)
Generator<int> filterEven(Generator<int> source) {
    while (source.next()) {
        if (source.value() % 2 == 0) {
            co_yield source.value();
        }
    }
}

int main() {
    // Fibonacci: lấy 10 số đầu
    std::cout << "Fibonacci: ";
    auto fib = fibonacci();
    for (int i = 0; i < 10 && fib.next(); ++i) {
        std::cout << fib.value() << ' ';
    }
    std::cout << '\n'; // 0 1 1 2 3 5 8 13 21 34

    // Range
    std::cout << "Range(0,20,3): ";
    auto r = range(0, 20, 3);
    while (r.next()) {
        std::cout << r.value() << ' ';
    }
    std::cout << '\n'; // 0 3 6 9 12 15 18

    // Composed: even numbers in range
    std::cout << "Even in range(0,20): ";
    auto evens = filterEven(range(0, 20));
    while (evens.next()) {
        std::cout << evens.value() << ' ';
    }
    std::cout << '\n'; // 0 2 4 6 8 10 12 14 16 18

    return 0;
}
```

### Simple Task (co_await + co_return)

```cpp
#include <iostream>
#include <coroutine>
#include <optional>
#include <string>

// Simple Task that can return a value
template<typename T>
class Task {
public:
    struct promise_type {
        std::optional<T> result;

        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; } // start immediately
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T value) { result = std::move(value); }
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Task(Handle h) : m_handle(h) {}
    ~Task() { if (m_handle) m_handle.destroy(); }

    Task(Task&& other) noexcept : m_handle(std::exchange(other.m_handle, nullptr)) {}
    Task(const Task&) = delete;

    T result() const { return *m_handle.promise().result; }

    // Awaitable: cho phép co_await Task
    bool await_ready() const { return m_handle.done(); }
    void await_suspend(std::coroutine_handle<> awaiter) {
        // Simple: resume immediately (synchronous for demo)
        m_handle.resume();
    }
    T await_resume() { return result(); }

private:
    Handle m_handle;
};

// Async-style functions
Task<int> computeValue(int input) {
    // Simulate computation
    co_return input * input;
}

Task<std::string> formatResult(int value) {
    co_return "Result: " + std::to_string(value);
}

Task<std::string> processData(int input) {
    // Chain tasks (looks synchronous!)
    auto squared = co_await computeValue(input);
    auto message = co_await formatResult(squared);
    co_return message;
}

int main() {
    auto task = processData(7);
    std::cout << task.result() << '\n'; // Result: 49
    return 0;
}
```

### Practical: Byte stream reader (co_yield cho parsing)

```cpp
#include <iostream>
#include <coroutine>
#include <vector>
#include <cstdint>
#include <string>

// Generator (same as above, abbreviated)
template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = std::move(value);
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Generator(Handle h) : m_handle(h) {}
    ~Generator() { if (m_handle) m_handle.destroy(); }
    Generator(Generator&& o) noexcept : m_handle(std::exchange(o.m_handle, nullptr)) {}
    Generator(const Generator&) = delete;
    bool next() { m_handle.resume(); return !m_handle.done(); }
    T value() const { return m_handle.promise().current_value; }
private:
    Handle m_handle;
};

// Parse CAN frames from byte stream
struct CANFrame {
    uint32_t id;
    uint8_t dlc;
    std::vector<uint8_t> data;
};

Generator<CANFrame> parseFrames(const std::vector<uint8_t>& stream) {
    size_t pos = 0;
    while (pos + 5 <= stream.size()) { // minimum: 4 (id) + 1 (dlc)
        CANFrame frame;

        // Parse ID (4 bytes big-endian)
        frame.id = (stream[pos] << 24) | (stream[pos+1] << 16) |
                   (stream[pos+2] << 8) | stream[pos+3];
        pos += 4;

        // Parse DLC
        frame.dlc = stream[pos++];
        if (frame.dlc > 8 || pos + frame.dlc > stream.size()) break;

        // Parse data
        frame.data.assign(stream.begin() + pos, stream.begin() + pos + frame.dlc);
        pos += frame.dlc;

        co_yield frame; // Yield one frame at a time
    }
}

int main() {
    // Simulated byte stream with 2 CAN frames
    std::vector<uint8_t> stream = {
        // Frame 1: ID=0x100, DLC=3, Data=AA BB CC
        0x00, 0x00, 0x01, 0x00, 0x03, 0xAA, 0xBB, 0xCC,
        // Frame 2: ID=0x200, DLC=2, Data=DE AD
        0x00, 0x00, 0x02, 0x00, 0x02, 0xDE, 0xAD,
    };

    auto frames = parseFrames(stream);
    int count = 0;
    while (frames.next()) {
        auto& f = frames.value();
        // Lưu ý: value() trả về copy nên dùng auto (không phải reference to temp)
        std::cout << "Frame " << ++count << ": ID=0x" << std::hex << f.id
                  << " DLC=" << std::dec << static_cast<int>(f.dlc)
                  << " Data=";
        for (auto b : f.data) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << '\n';
    }

    return 0;
}
```

---

## Key concepts

| Keyword | Nghĩa |
|---------|--------|
| `co_yield expr` | Suspend + trả giá trị cho caller (generator) |
| `co_return expr` | Kết thúc coroutine + trả kết quả cuối |
| `co_await expr` | Suspend + đợi expr hoàn thành |

---

## Lỗi thường gặp

### 1. Dangling reference trong coroutine

```cpp
// SAI: parameter reference → coroutine outlive caller
Generator<char> bad(const std::string& str) {
    for (char c : str) { // str có thể đã bị destroy!
        co_yield c;
    }
}

// ĐÚNG: capture by value
Generator<char> good(std::string str) { // copy vào coroutine frame
    for (char c : str) {
        co_yield c;
    }
}
```

### 2. Quên destroy coroutine handle

```cpp
// Coroutine frame allocated trên heap → phải destroy!
// Nếu Generator destructor không gọi m_handle.destroy() → MEMORY LEAK
```

### 3. Coroutine != thread

```cpp
// Coroutine là COOPERATIVE: chỉ suspend tại co_await/co_yield
// Không bị preempt giống thread
// Một coroutine chạy blocking code → block toàn bộ thread!

Generator<int> blocking() {
    std::this_thread::sleep_for(std::chrono::seconds(10)); // Block thread!
    co_yield 1;
}
// Dùng async I/O thay vì blocking calls trong coroutines
```

---

## Khi nào dùng coroutines?

| Use case | Pattern |
|----------|---------|
| Lazy sequences | `co_yield` (Generator) |
| Async I/O | `co_await` (Task + event loop) |
| Parser/tokenizer | `co_yield` tokens |
| State machine streaming | `co_yield` states |
| Pipeline processing | Chain generators |

---

## Libraries thực tế (recommend)

- **cppcoro**: lightweight, header-only generators/tasks
- **Boost.Asio**: async I/O với coroutine support
- **folly::coro**: Facebook's production coroutine library
- **libunifex**: Sender/receiver model (future std::execution)

---

## Compile

```bash
g++ -std=c++20 -fcoroutines -o program main.cpp
# GCC 10+: -fcoroutines flag cần thiết
# GCC 11+: không cần flag
# Clang 14+: built-in support
```
