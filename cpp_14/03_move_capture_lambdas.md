# Move Capture in Lambdas (C++14)

## Tổng quan

C++14 cho phép **init capture** trong lambda: `[x = expr]`. Đặc biệt quan trọng là `[x = std::move(obj)]` để **move** object vào lambda thay vì copy.

**Tại sao dùng?**
- Truyền ownership vào lambda (unique_ptr, heavy objects)
- Thread programming: move data vào thread lambda
- Tránh copy lớn (vector, string) khi lambda outlive scope
- C++11 chỉ có capture by value (copy) hoặc reference (dangling risk)

---

## Cách sử dụng

### Cơ bản: move vào lambda

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>

int main() {
    // Move unique_ptr vào lambda (không thể copy!)
    auto ptr = std::make_unique<int>(42);
    
    auto lambda = [p = std::move(ptr)]() {
        std::cout << "Value: " << *p << '\n';
    };
    // ptr giờ là nullptr
    lambda(); // Value: 42

    // Move heavy object tránh copy
    std::vector<int> bigData(1000000, 42);
    
    auto process = [data = std::move(bigData)]() {
        std::cout << "Processing " << data.size() << " elements\n";
    };
    // bigData giờ là empty
    process();

    return 0;
}
```

### Thread: move data vào thread

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>

class Connection {
    std::string m_addr;
public:
    explicit Connection(std::string addr) : m_addr(std::move(addr)) {
        std::cout << "Connection created: " << m_addr << '\n';
    }
    ~Connection() { std::cout << "Connection closed: " << m_addr << '\n'; }
    
    // Non-copyable
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;
    
    void send(const std::string& msg) {
        std::cout << "[" << m_addr << "] Sending: " << msg << '\n';
    }
};

int main() {
    auto conn = std::make_unique<Connection>("192.168.1.100:8080");
    
    // Move ownership vào thread (unique_ptr không copy được)
    std::thread worker([c = std::move(conn)]() mutable {
        c->send("Hello from thread");
        c->send("Data payload");
        // Connection tự destroy khi thread kết thúc
    });
    
    // conn giờ là nullptr - thread owns it
    std::cout << "Main: conn is " << (conn ? "valid" : "null") << '\n';
    
    worker.join();
    return 0;
}
```

### Init capture: tạo biến mới trong capture

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main() {
    // Tạo counter trong lambda (mutable state)
    auto counter = [count = 0]() mutable {
        return ++count;
    };
    
    std::cout << counter() << '\n'; // 1
    std::cout << counter() << '\n'; // 2
    std::cout << counter() << '\n'; // 3

    // Capture expression (không chỉ biến)
    const std::string prefix = "sensor_";
    int id = 42;
    
    auto makeId = [name = prefix + std::to_string(id)]() {
        return name;
    };
    std::cout << makeId() << '\n'; // sensor_42

    // Move từng phần tử vào closure
    std::vector<std::string> commands = {"start", "process", "stop"};
    
    auto executor = [cmds = std::move(commands)]() {
        for (const auto& cmd : cmds) {
            std::cout << "Executing: " << cmd << '\n';
        }
    };
    // commands giờ empty
    executor();

    return 0;
}
```

### Async task pattern: move vào async lambda

```cpp
#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <memory>

// Heavy computation: move data thay vì copy
std::future<long long> asyncSum(std::vector<int> data) {
    // std::async mặc định copy arguments → dùng move capture
    return std::async(std::launch::async,
        [d = std::move(data)]() {
            return std::accumulate(d.begin(), d.end(), 0LL);
        }
    );
}

// Producer: tạo data và move vào worker thread
void producerConsumer() {
    auto produce = []() {
        std::vector<int> data(1000);
        std::iota(data.begin(), data.end(), 1); // fill 1..1000
        return data;
    };
    
    auto data = produce();
    
    // Move vào consumer thread
    std::thread consumer([d = std::move(data)]() {
        long long sum = std::accumulate(d.begin(), d.end(), 0LL);
        std::cout << "Sum: " << sum << '\n'; // 500500
    });
    
    consumer.join();
}

int main() {
    std::vector<int> bigVec(100000, 1);
    auto future = asyncSum(std::move(bigVec));
    
    std::cout << "Doing other work...\n";
    std::cout << "Result: " << future.get() << '\n'; // 100000
    
    producerConsumer();
    return 0;
}
```

### Callback với ownership transfer

```cpp
#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <string>

using Callback = std::function<void()>;

class EventLoop {
    std::vector<Callback> m_tasks;
public:
    void post(Callback task) {
        m_tasks.push_back(std::move(task));
    }
    
    void run() {
        for (auto& task : m_tasks) {
            task();
        }
        m_tasks.clear();
    }
};

int main() {
    EventLoop loop;
    
    // Move unique resource vào callback
    auto buffer = std::make_unique<std::vector<uint8_t>>(1024, 0xFF);
    
    loop.post([buf = std::move(buffer)]() {
        std::cout << "Processing buffer of " << buf->size() << " bytes\n";
    });
    
    // Move string vào callback
    std::string message = "Important notification";
    loop.post([msg = std::move(message)]() {
        std::cout << "Sending: " << msg << '\n';
    });
    
    loop.run();
    return 0;
}
```

---

## Lỗi thường gặp

### 1. Quên `mutable` khi cần modify captured value

```cpp
auto ptr = std::make_unique<int>(10);

// SAI: lambda const by default, không gọi được non-const methods
// auto bad = [p = std::move(ptr)]() {
//     *p = 20;  // Error: p là const trong lambda
// };

// ĐÚNG: thêm mutable
auto good = [p = std::move(ptr)]() mutable {
    *p = 20;  // OK
    std::cout << *p << '\n';
};
good();
```

### 2. Dùng object sau khi move

```cpp
std::vector<int> data = {1, 2, 3};
auto lambda = [d = std::move(data)]() { /* ... */ };

// SAI: data đã bị move!
// std::cout << data.size(); // Undefined - có thể = 0 hoặc crash
// ĐÚNG: không dùng data sau move
```

### 3. Reference capture + thread = dangling

```cpp
void dangerous() {
    std::string msg = "hello";
    
    // SAI: reference capture + thread → msg có thể bị destroy trước thread dùng
    // std::thread t([&msg]() {
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    //     std::cout << msg;  // DANGLING!
    // });
    // t.detach();
    
    // ĐÚNG: move capture → thread owns data
    std::thread t([m = std::move(msg)]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << m; // Safe: thread owns the string
    });
    t.join(); // hoặc detach, data vẫn safe
}
```

---

## Key takeaway

| Capture | Behavior | Use case |
|---------|----------|----------|
| `[x]` | Copy x | Small, copyable types |
| `[&x]` | Reference x | Synchronous lambdas, x outlives lambda |
| `[x = std::move(obj)]` | Move obj vào x | Thread, async, unique_ptr, heavy objects |
| `[x = expr]` | Init x từ expression | Computed values, counters |

---

## Compile

```bash
g++ -std=c++14 -pthread -o program main.cpp
```
