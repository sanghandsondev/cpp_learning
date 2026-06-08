# Synchronization Primitives: latch, barrier, semaphore (C++20)

## Tổng quan

C++20 thêm 3 primitives mới cho thread synchronization:
- `std::latch`: one-shot countdown barrier (dùng 1 lần)
- `std::barrier`: reusable barrier (dùng nhiều lần)
- `std::counting_semaphore`: control concurrent access count

**Tại sao dùng?**
- Nhẹ hơn condition_variable + mutex cho các patterns phổ biến
- Self-documenting: tên rõ ràng hơn raw CV
- Đúng pattern, đúng tool → ít bug hơn
- Thread pool, pipeline, phased computation

---

## std::latch - One-shot countdown

### "Wait for all workers to finish" pattern

```cpp
#include <iostream>
#include <latch>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

// Use case: đợi tất cả subsystems init xong trước khi start
void initSubsystem(const std::string& name, std::latch& ready) {
    // Simulate initialization
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
    std::cout << name << " initialized\n";
    ready.count_down(); // Giảm counter đi 1
}

int main() {
    constexpr int NUM_SUBSYSTEMS = 4;
    std::latch initLatch(NUM_SUBSYSTEMS); // Counter bắt đầu = 4

    // Start init threads
    std::jthread t1(initSubsystem, "CAN driver", std::ref(initLatch));
    std::jthread t2(initSubsystem, "GPS module", std::ref(initLatch));
    std::jthread t3(initSubsystem, "Display", std::ref(initLatch));
    std::jthread t4(initSubsystem, "Logger", std::ref(initLatch));

    std::cout << "Waiting for all subsystems...\n";
    initLatch.wait(); // Block cho đến counter == 0
    std::cout << "All subsystems ready! Starting main loop.\n";

    return 0;
}
```

### Producer signals multiple consumers

```cpp
#include <iostream>
#include <latch>
#include <thread>
#include <vector>
#include <string>

int main() {
    std::latch dataReady(1); // Chỉ cần 1 signal

    std::string sharedData;

    // Consumers: đợi data
    std::vector<std::jthread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back([&dataReady, &sharedData, i]() {
            std::cout << "Consumer " << i << " waiting...\n";
            dataReady.wait(); // Block cho đến latch == 0
            std::cout << "Consumer " << i << " got: " << sharedData << '\n';
        });
    }

    // Producer: prepare data then signal
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    sharedData = "CAN frame 0x100: DEADBEEF";
    std::cout << "Producer: data ready, signaling...\n";
    dataReady.count_down(); // Wake up ALL consumers!

    return 0;
}
```

---

## std::barrier - Reusable synchronization point

### Phased computation (nhiều phases, sync giữa mỗi phase)

```cpp
#include <iostream>
#include <barrier>
#include <thread>
#include <vector>
#include <mutex>
#include <string>

std::mutex printMtx;

void safePrint(const std::string& msg) {
    std::lock_guard lock(printMtx);
    std::cout << msg << '\n';
}

int main() {
    constexpr int NUM_WORKERS = 4;
    constexpr int NUM_PHASES = 3;

    // Completion function: chạy 1 lần khi tất cả threads đến barrier
    int phase = 0;
    auto onPhaseComplete = [&phase]() noexcept {
        ++phase;
        std::cout << "--- Phase " << phase << " complete ---\n";
    };

    std::barrier sync_point(NUM_WORKERS, onPhaseComplete);

    std::vector<std::jthread> workers;
    for (int id = 0; id < NUM_WORKERS; ++id) {
        workers.emplace_back([&sync_point, id]() {
            for (int p = 0; p < NUM_PHASES; ++p) {
                // Do work for this phase
                safePrint("Worker " + std::to_string(id) + " phase " + std::to_string(p));
                std::this_thread::sleep_for(std::chrono::milliseconds(50 + id * 30));

                // Synchronize: đợi tất cả workers xong phase này
                sync_point.arrive_and_wait();
                // Tất cả workers đến đây trước khi bất kỳ ai đi tiếp
            }
        });
    }

    return 0;
}
```

### Pipeline pattern: read → process → write (sync mỗi iteration)

```cpp
#include <iostream>
#include <barrier>
#include <thread>
#include <vector>
#include <cstdint>

struct FrameData {
    uint32_t id = 0;
    double value = 0.0;
    bool processed = false;
};

int main() {
    constexpr int ITERATIONS = 5;
    FrameData shared_frame;

    // 3 stages: read, process, write
    std::barrier pipeline(3);

    std::jthread reader([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            // Stage 1: Read
            shared_frame.id = 0x100 + i;
            shared_frame.value = i * 1.5;
            shared_frame.processed = false;
            std::cout << "Read frame 0x" << std::hex << shared_frame.id << std::dec << '\n';

            pipeline.arrive_and_wait(); // Sync: read done
            pipeline.arrive_and_wait(); // Sync: wait for process + write
        }
    });

    std::jthread processor([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            pipeline.arrive_and_wait(); // Wait for read

            // Stage 2: Process
            shared_frame.value *= 2.0;
            shared_frame.processed = true;
            std::cout << "Processed: value=" << shared_frame.value << '\n';

            pipeline.arrive_and_wait(); // Sync: process done
        }
    });

    std::jthread writer([&]() {
        for (int i = 0; i < ITERATIONS; ++i) {
            pipeline.arrive_and_wait(); // Wait for read
            pipeline.arrive_and_wait(); // Wait for process

            // Stage 3: Write
            std::cout << "Written frame 0x" << std::hex << shared_frame.id
                      << " value=" << std::dec << shared_frame.value << "\n\n";
        }
    });

    return 0;
}
```

---

## std::counting_semaphore - Limit concurrent access

### Connection pool (giới hạn N concurrent connections)

```cpp
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex printMtx;

class ConnectionPool {
    // Semaphore: max 3 concurrent connections
    std::counting_semaphore<3> m_slots{3};
    std::mutex m_mutex;
    int m_activeConnections = 0;

public:
    void useConnection(int taskId) {
        {
            std::lock_guard lock(printMtx);
            std::cout << "Task " << taskId << " waiting for connection...\n";
        }

        m_slots.acquire(); // Block nếu đã đủ 3 connections

        {
            std::lock_guard lock(m_mutex);
            ++m_activeConnections;
            std::lock_guard plock(printMtx);
            std::cout << "Task " << taskId << " got connection (active: "
                      << m_activeConnections << ")\n";
        }

        // Simulate using connection
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        {
            std::lock_guard lock(m_mutex);
            --m_activeConnections;
            std::lock_guard plock(printMtx);
            std::cout << "Task " << taskId << " released connection (active: "
                      << m_activeConnections << ")\n";
        }

        m_slots.release(); // Return slot
    }
};

int main() {
    ConnectionPool pool;

    // 10 tasks nhưng chỉ 3 chạy đồng thời
    std::vector<std::jthread> tasks;
    for (int i = 0; i < 10; ++i) {
        tasks.emplace_back([&pool, i]() {
            pool.useConnection(i);
        });
    }

    return 0;
}
```

### Binary semaphore (lightweight mutex / signaling)

```cpp
#include <iostream>
#include <semaphore>
#include <thread>
#include <queue>
#include <string>

// binary_semaphore = counting_semaphore<1>
// Nhẹ hơn condition_variable cho simple signaling

class SimpleChannel {
    std::queue<std::string> m_queue;
    std::binary_semaphore m_hasData{0};  // Starts at 0: no data
    std::mutex m_mutex;

public:
    void send(std::string msg) {
        {
            std::lock_guard lock(m_mutex);
            m_queue.push(std::move(msg));
        }
        m_hasData.release(); // Signal: data available (counter → 1)
    }

    std::string receive() {
        m_hasData.acquire(); // Block until data available (counter → 0)
        std::lock_guard lock(m_mutex);
        auto msg = std::move(m_queue.front());
        m_queue.pop();
        return msg;
    }
};

int main() {
    SimpleChannel channel;

    std::jthread consumer([&channel]() {
        for (int i = 0; i < 5; ++i) {
            auto msg = channel.receive(); // Blocks until message arrives
            std::cout << "Received: " << msg << '\n';
        }
    });

    std::jthread producer([&channel]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            channel.send("Message #" + std::to_string(i));
        }
    });

    return 0;
}
```

### Rate limiter (throttle requests)

```cpp
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex printMtx;

class RateLimiter {
    std::counting_semaphore<10> m_tokens; // Max 10 requests

    // Refill thread adds tokens periodically
    std::jthread m_refiller;

public:
    RateLimiter(int maxBurst, std::chrono::milliseconds refillInterval)
        : m_tokens(maxBurst)
    {
        m_refiller = std::jthread([this, maxBurst, refillInterval](std::stop_token token) {
            while (!token.stop_requested()) {
                std::this_thread::sleep_for(refillInterval);
                // Try to add a token (up to max)
                m_tokens.release();
            }
        });
    }

    bool tryRequest(std::chrono::milliseconds timeout) {
        return m_tokens.try_acquire_for(timeout);
    }

    void request() {
        m_tokens.acquire(); // Block until token available
    }
};

int main() {
    // Max 3 burst, refill 1 token every 200ms
    RateLimiter limiter(3, std::chrono::milliseconds(200));

    std::vector<std::jthread> clients;
    for (int i = 0; i < 8; ++i) {
        clients.emplace_back([&limiter, i]() {
            auto start = std::chrono::steady_clock::now();
            limiter.request();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start);

            std::lock_guard lock(printMtx);
            std::cout << "Client " << i << " served after " << elapsed.count() << "ms\n";
        });
    }

    return 0;
}
```

---

## Lỗi thường gặp

### 1. Latch count sai

```cpp
std::latch l(5); // Expect 5 count_downs

// Chỉ có 4 threads → l.wait() sẽ BLOCK MÃI MÃI!
// ĐÚNG: count phải CHÍNH XÁC = số lần count_down()
```

### 2. Barrier: số threads phải consistent

```cpp
std::barrier b(4); // Expect 4 threads

// Nếu 1 thread crash/exit sớm → 3 remaining threads block mãi tại barrier!
// Dùng arrive_and_drop() nếu thread muốn rời barrier group
void earlyExit(std::barrier<>& b) {
    // Tôi không participate nữa
    b.arrive_and_drop(); // Giảm expected count
}
```

### 3. Semaphore overflow

```cpp
std::counting_semaphore<5> sem(5);

// SAI: release thêm vượt quá max → undefined behavior!
sem.release(); // 6 > max(5) → UB!

// ĐÚNG: chỉ release sau acquire
sem.acquire();
// ... use resource ...
sem.release(); // OK: trả lại slot đã acquire
```

---

## So sánh primitives

| Primitive | Reusable | Use case |
|-----------|----------|----------|
| `latch` | ❌ One-shot | Wait for N events, then proceed |
| `barrier` | ✅ Multi-phase | Sync threads between phases |
| `semaphore` | ✅ | Limit N concurrent accesses |
| `mutex` | ✅ | Exclusive access (semaphore<1>) |
| `condition_variable` | ✅ | Complex wait conditions |

---

## Compile

```bash
g++ -std=c++20 -pthread -o program main.cpp
```
