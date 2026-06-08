# std::shared_mutex & std::scoped_lock (C++17)

## Tổng quan

- `std::shared_mutex`: reader-writer lock (nhẹ hơn `shared_timed_mutex`, không timeout)
- `std::scoped_lock`: lock **nhiều mutexes cùng lúc** mà **không deadlock** (thay thế `std::lock_guard`)

**Tại sao dùng?**
- `shared_mutex`: read-heavy workloads (config, cache, routing tables)
- `scoped_lock`: lock nhiều mutex theo thứ tự an toàn, tránh deadlock kinh điển
- RAII: tự unlock khi out of scope, exception-safe

---

## std::shared_mutex

### Cơ bản: thread-safe cache

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

class ThreadSafeCache {
    mutable std::shared_mutex m_mutex;
    std::unordered_map<std::string, std::string> m_data;

public:
    // READ: shared lock (concurrent readers OK)
    std::optional<std::string> get(const std::string& key) const {
        std::shared_lock lock(m_mutex); // C++17 CTAD: không cần <std::shared_mutex>
        auto it = m_data.find(key);
        if (it != m_data.end()) return it->second;
        return std::nullopt;
    }

    // WRITE: unique lock (exclusive)
    void put(const std::string& key, const std::string& value) {
        std::unique_lock lock(m_mutex);
        m_data[key] = value;
    }

    // WRITE: remove
    bool remove(const std::string& key) {
        std::unique_lock lock(m_mutex);
        return m_data.erase(key) > 0;
    }

    size_t size() const {
        std::shared_lock lock(m_mutex);
        return m_data.size();
    }
};

int main() {
    ThreadSafeCache cache;

    // Writer thread
    std::thread writer([&cache]() {
        for (int i = 0; i < 100; ++i) {
            cache.put("key" + std::to_string(i), "value" + std::to_string(i));
        }
    });

    // Multiple reader threads
    std::vector<std::thread> readers;
    for (int id = 0; id < 4; ++id) {
        readers.emplace_back([&cache, id]() {
            int found = 0;
            for (int i = 0; i < 200; ++i) {
                if (cache.get("key" + std::to_string(i % 100))) {
                    ++found;
                }
            }
            std::cout << "Reader " << id << " found " << found << " entries\n";
        });
    }

    writer.join();
    for (auto& r : readers) r.join();
    std::cout << "Final cache size: " << cache.size() << '\n';

    return 0;
}
```

### Real-world: CAN message routing table

```cpp
#include <iostream>
#include <shared_mutex>
#include <unordered_map>
#include <functional>
#include <vector>
#include <thread>
#include <cstdint>

using MessageHandler = std::function<void(uint32_t id, const std::vector<uint8_t>&)>;

class MessageRouter {
    mutable std::shared_mutex m_mutex;
    std::unordered_map<uint32_t, MessageHandler> m_handlers;

public:
    // Registration (rare) → exclusive lock
    void registerHandler(uint32_t canId, MessageHandler handler) {
        std::unique_lock lock(m_mutex);
        m_handlers[canId] = std::move(handler);
    }

    void unregisterHandler(uint32_t canId) {
        std::unique_lock lock(m_mutex);
        m_handlers.erase(canId);
    }

    // Dispatch (frequent) → shared lock
    void dispatch(uint32_t canId, const std::vector<uint8_t>& data) const {
        std::shared_lock lock(m_mutex);
        auto it = m_handlers.find(canId);
        if (it != m_handlers.end()) {
            it->second(canId, data);
        }
    }
};

int main() {
    MessageRouter router;

    router.registerHandler(0x100, [](uint32_t id, const auto& data) {
        std::cout << "Engine speed handler: ID=0x" << std::hex << id
                  << " len=" << std::dec << data.size() << '\n';
    });

    router.registerHandler(0x200, [](uint32_t id, const auto& data) {
        std::cout << "Vehicle speed handler: ID=0x" << std::hex << id << '\n';
    });

    // Dispatch threads (high frequency)
    std::vector<std::thread> dispatchers;
    for (int i = 0; i < 3; ++i) {
        dispatchers.emplace_back([&router]() {
            for (int j = 0; j < 100; ++j) {
                router.dispatch(0x100, {0x01, 0x02, 0x03});
                router.dispatch(0x200, {0xAA, 0xBB});
            }
        });
    }

    for (auto& t : dispatchers) t.join();
    return 0;
}
```

---

## std::scoped_lock

### Giải quyết deadlock khi lock nhiều mutex

```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <string>

class BankAccount {
    mutable std::mutex m_mutex;
    std::string m_name;
    double m_balance;

public:
    BankAccount(std::string name, double balance)
        : m_name(std::move(name)), m_balance(balance) {}

    // DEADLOCK EXAMPLE (DON'T DO THIS):
    // void transferBad(BankAccount& to, double amount) {
    //     std::lock_guard lock1(m_mutex);      // lock A
    //     std::lock_guard lock2(to.m_mutex);   // lock B
    //     // Thread 1: lock A → wait B
    //     // Thread 2: lock B → wait A → DEADLOCK!
    // }

    // ĐÚNG: scoped_lock locks cả 2 mutex atomically (deadlock-free)
    friend void transfer(BankAccount& from, BankAccount& to, double amount) {
        // Lock cả 2 mutex cùng lúc - thứ tự không quan trọng!
        std::scoped_lock lock(from.m_mutex, to.m_mutex);

        if (from.m_balance >= amount) {
            from.m_balance -= amount;
            to.m_balance += amount;
            std::cout << from.m_name << " → " << to.m_name
                      << ": $" << amount << '\n';
        }
    }

    double balance() const {
        std::lock_guard lock(m_mutex);
        return m_balance;
    }

    const std::string& name() const { return m_name; }
};

int main() {
    BankAccount alice("Alice", 1000.0);
    BankAccount bob("Bob", 1000.0);

    // 2 threads transfer ngược chiều → KHÔNG deadlock nhờ scoped_lock
    std::thread t1([&]() {
        for (int i = 0; i < 100; ++i) {
            transfer(alice, bob, 10.0);
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100; ++i) {
            transfer(bob, alice, 10.0);
        }
    });

    t1.join();
    t2.join();

    std::cout << alice.name() << ": $" << alice.balance() << '\n';
    std::cout << bob.name() << ": $" << bob.balance() << '\n';
    std::cout << "Total: $" << (alice.balance() + bob.balance()) << '\n'; // 2000
    return 0;
}
```

### scoped_lock với 1 mutex (thay thế lock_guard)

```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Counter {
    std::mutex m_mutex;
    int m_count = 0;

public:
    void increment() {
        // scoped_lock thay thế lock_guard (C++17)
        std::scoped_lock lock(m_mutex);
        ++m_count;
    }

    int get() const {
        // Lưu ý: scoped_lock không có defer/try_lock
        // Nếu cần, dùng unique_lock
        return m_count;
    }
};

int main() {
    Counter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 10000; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) t.join();
    std::cout << "Count: " << counter.get() << '\n'; // 100000
    return 0;
}
```

### Real-world: lock nhiều resources khi swap/merge

```cpp
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>

class MessageQueue {
    mutable std::mutex m_mutex;
    std::vector<std::string> m_messages;
    std::string m_name;

public:
    explicit MessageQueue(std::string name) : m_name(std::move(name)) {}

    void push(std::string msg) {
        std::scoped_lock lock(m_mutex);
        m_messages.push_back(std::move(msg));
    }

    // Merge: lấy tất cả messages từ other vào this
    // Cần lock cả 2 queues → scoped_lock tránh deadlock
    void mergeFrom(MessageQueue& other) {
        std::scoped_lock lock(m_mutex, other.m_mutex);

        m_messages.insert(m_messages.end(),
            std::make_move_iterator(other.m_messages.begin()),
            std::make_move_iterator(other.m_messages.end()));
        other.m_messages.clear();

        std::cout << "Merged " << other.m_name << " into " << m_name << '\n';
    }

    size_t size() const {
        std::scoped_lock lock(m_mutex);
        return m_messages.size();
    }

    void print() const {
        std::scoped_lock lock(m_mutex);
        std::cout << m_name << " [" << m_messages.size() << "]: ";
        for (const auto& m : m_messages) std::cout << m << " ";
        std::cout << '\n';
    }
};

int main() {
    MessageQueue primary("Primary");
    MessageQueue secondary("Secondary");

    std::thread t1([&primary]() {
        for (int i = 0; i < 5; ++i)
            primary.push("P" + std::to_string(i));
    });

    std::thread t2([&secondary]() {
        for (int i = 0; i < 5; ++i)
            secondary.push("S" + std::to_string(i));
    });

    t1.join();
    t2.join();

    primary.print();
    secondary.print();

    primary.mergeFrom(secondary);

    primary.print();
    secondary.print(); // empty
    return 0;
}
```

---

## Lỗi thường gặp

### 1. Nhầm shared_lock (read) và unique_lock (write)

```cpp
std::shared_mutex mtx;
int data = 0;

// shared_lock = READ ONLY (nhiều threads vào được)
// unique_lock = WRITE (exclusive)

// SAI: dùng shared_lock để write → race condition
void bad_write() {
    std::shared_lock lock(mtx);
    data++; // RACE CONDITION!
}
```

### 2. scoped_lock không có defer_lock/try_lock

```cpp
std::mutex m1, m2;

// scoped_lock luôn lock ngay lập tức
std::scoped_lock lock(m1, m2); // Lock ngay

// Nếu cần defer hoặc try → dùng unique_lock
std::unique_lock lock1(m1, std::defer_lock);
std::unique_lock lock2(m2, std::defer_lock);
std::lock(lock1, lock2); // Lock cả 2 sau
```

### 3. Recursive locking → deadlock

```cpp
std::shared_mutex mtx; // KHÔNG recursive!

void outer() {
    std::unique_lock lock(mtx);
    // inner(); // Nếu inner() cũng lock mtx → DEADLOCK!
}

// Giải pháp: tách logic internal không cần lock
void inner_impl() { /* no lock needed */ }
void inner() {
    std::unique_lock lock(mtx);
    inner_impl();
}
void outer() {
    std::unique_lock lock(mtx);
    inner_impl(); // Gọi version không lock
}
```

---

## So sánh lock types

| Lock | Dùng khi |
|------|----------|
| `std::lock_guard` | 1 mutex, simple (C++11) |
| `std::scoped_lock` | 1+ mutex, deadlock-free (C++17, prefer this) |
| `std::unique_lock` | Cần defer, try_lock, condition_variable |
| `std::shared_lock` | Read access với shared_mutex |

---

## Compile

```bash
g++ -std=c++17 -pthread -o program main.cpp
```
