# std::jthread & std::stop_token (C++20)

## Tổng quan

- `std::jthread`: **auto-joining thread** - tự join khi destructor gọi (không crash nếu quên join)
- `std::stop_token` / `std::stop_source`: cooperative cancellation mechanism

**Tại sao dùng?**
- `std::thread` crash (terminate) nếu quên join/detach → jthread fix điều này
- Cancellation: dừng thread một cách graceful thay vì dùng atomic bool tự viết
- Production code: stop_token tích hợp sẵn, không cần homebrew solutions
- Exception-safe: jthread join ngay cả khi exception xảy ra

---

## Cách sử dụng

### jthread cơ bản (auto-join)

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void worker(int id) {
    std::cout << "Worker " << id << " started\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Worker " << id << " done\n";
}

int main() {
    {
        // std::thread: PHẢI join hoặc detach, nếu không → std::terminate()
        // std::thread t(worker, 1);
        // Quên join → CRASH!

        // std::jthread: tự join khi ra khỏi scope
        std::jthread t1(worker, 1);
        std::jthread t2(worker, 2);

        std::cout << "Main doing work...\n";
        // t1, t2 tự join ở đây (destructor)
    }
    std::cout << "All workers joined automatically\n";

    // Exception-safe!
    try {
        std::jthread t3(worker, 3);
        throw std::runtime_error("oops");
        // t3 vẫn được join trước khi exception propagate
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    return 0;
}
```

### stop_token: cooperative cancellation

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// === CŨ (C++11): tự quản lý cancellation ===
void old_worker(std::atomic<bool>& shouldStop) {
    while (!shouldStop.load()) {
        std::cout << "Old: working...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Old: stopped\n";
}

// === MỚI (C++20): stop_token built-in ===
void new_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        std::cout << "New: working...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "New: stopped gracefully\n";
}

int main() {
    // C++20: jthread tự truyền stop_token nếu function nhận nó
    std::jthread worker(new_worker);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Request stop (cooperative - thread check token trong loop)
    worker.request_stop();

    // jthread tự join ở đây
    std::cout << "Main: done\n";
    return 0;
}
```

### stop_callback: cleanup khi stop requested

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

class NetworkReceiver {
    int m_socketFd = 42; // simulated

public:
    void run(std::stop_token token) {
        // Register callback: khi stop requested → close socket để unblock recv()
        std::stop_callback cleanup(token, [this]() {
            std::cout << "Cleanup: closing socket " << m_socketFd << '\n';
            m_socketFd = -1;
            // Trong thực tế: shutdown(m_socketFd, SHUT_RDWR);
        });

        while (!token.stop_requested()) {
            // Simulate blocking recv
            std::cout << "Receiving on fd=" << m_socketFd << "...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            if (m_socketFd < 0) {
                std::cout << "Socket closed, exiting\n";
                break;
            }
        }

        std::cout << "Receiver: shutdown complete\n";
    }
};

int main() {
    NetworkReceiver receiver;
    std::jthread recvThread([&receiver](std::stop_token token) {
        receiver.run(token);
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Main: requesting stop...\n";
    recvThread.request_stop(); // Triggers stop_callback → closes socket

    // jthread auto-join
    return 0;
}
```

### Multiple workers with shared stop_source

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

std::mutex printMutex;

void periodicTask(std::stop_token token, int id, std::chrono::milliseconds interval) {
    int count = 0;
    while (!token.stop_requested()) {
        {
            std::lock_guard lock(printMutex);
            std::cout << "Task " << id << " tick #" << ++count << '\n';
        }
        // Sleep that can be interrupted by stop_request
        // (simplified: real code should use condition_variable_any)
        std::this_thread::sleep_for(interval);
    }
    std::lock_guard lock(printMutex);
    std::cout << "Task " << id << " stopped after " << count << " ticks\n";
}

int main() {
    // Shared stop_source: stop ALL workers at once
    std::stop_source stopSource;

    std::vector<std::jthread> workers;
    workers.emplace_back(periodicTask, stopSource.get_token(), 1, std::chrono::milliseconds(100));
    workers.emplace_back(periodicTask, stopSource.get_token(), 2, std::chrono::milliseconds(150));
    workers.emplace_back(periodicTask, stopSource.get_token(), 3, std::chrono::milliseconds(200));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "=== Stopping all workers ===\n";
    stopSource.request_stop(); // Stop tất cả!

    // jthreads auto-join
    std::cout << "All stopped\n";
    return 0;
}
```

### Real-world: CAN bus reader thread

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <vector>

struct CANFrame {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};

class CANReader {
    std::mutex m_mutex;
    std::condition_variable_any m_cv;
    std::queue<CANFrame> m_queue;
    static constexpr size_t MAX_QUEUE = 1000;

public:
    // Reader thread: nhận frames liên tục
    void readerLoop(std::stop_token token) {
        int frameCount = 0;
        while (!token.stop_requested()) {
            // Simulate receiving CAN frame
            CANFrame frame{};
            frame.id = 0x100 + (frameCount % 10);
            frame.dlc = 8;
            frame.data[0] = static_cast<uint8_t>(frameCount & 0xFF);

            {
                std::lock_guard lock(m_mutex);
                if (m_queue.size() < MAX_QUEUE) {
                    m_queue.push(frame);
                }
            }
            m_cv.notify_one();

            ++frameCount;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Reader: stopped after " << frameCount << " frames\n";
    }

    // Consumer: lấy frame từ queue (with cancellation support)
    bool getFrame(std::stop_token token, CANFrame& frame) {
        std::unique_lock lock(m_mutex);
        // Wait với stop_token: tự wake up khi stop requested!
        bool hasData = m_cv.wait(lock, token, [this]() {
            return !m_queue.empty();
        });

        if (!hasData) return false; // stop requested

        frame = m_queue.front();
        m_queue.pop();
        return true;
    }
};

int main() {
    CANReader reader;

    // Reader thread
    std::jthread readerThread([&reader](std::stop_token token) {
        reader.readerLoop(token);
    });

    // Consumer thread
    std::jthread consumerThread([&reader](std::stop_token token) {
        int processed = 0;
        CANFrame frame;
        while (reader.getFrame(token, frame)) {
            ++processed;
            if (processed % 20 == 0) {
                std::cout << "Processed " << processed << " frames, "
                          << "last ID=0x" << std::hex << frame.id << std::dec << '\n';
            }
        }
        std::cout << "Consumer: stopped after " << processed << " frames\n";
    });

    // Run for 1 second then stop everything
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "=== Shutting down ===\n";

    // Stop cả 2 threads
    readerThread.request_stop();
    consumerThread.request_stop();

    // Auto-join
    return 0;
}
```

---

## Lỗi thường gặp

### 1. stop_token là cooperative - thread PHẢI check

```cpp
// SAI: thread không bao giờ check token → không dừng được
void bad_worker(std::stop_token token) {
    while (true) { // Không check token!
        heavy_computation();
    }
}

// ĐÚNG: check token thường xuyên
void good_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        heavy_computation_chunk(); // Chia nhỏ work
    }
}
```

### 2. jthread destructor blocks (join)

```cpp
void potentially_slow() {
    std::jthread t([]() {
        std::this_thread::sleep_for(std::chrono::hours(1)); // 1 giờ!
    });
    // Destructor sẽ BLOCK 1 giờ chờ thread xong!

    // ĐÚNG: dùng stop_token + request_stop
    std::jthread t2([](std::stop_token token) {
        while (!token.stop_requested()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    // Destructor: request_stop() → thread dừng nhanh → join nhanh
}
```

### 3. stop_callback lifetime

```cpp
void worker(std::stop_token token) {
    int resource = 42;

    {
        // stop_callback tự unregister khi ra khỏi scope!
        std::stop_callback cb(token, [&resource]() {
            resource = -1;
        });
        // callback active ở đây
    }
    // callback ĐÃ bị unregister ở đây!
    // stop_request sẽ KHÔNG trigger callback nữa
}
```

---

## So sánh thread vs jthread

| Feature | std::thread | std::jthread |
|---------|-------------|--------------|
| Auto-join | ❌ (terminate nếu quên) | ✅ |
| Stop token | ❌ (tự code) | ✅ Built-in |
| Exception-safe | ❌ (phải try-catch + join) | ✅ |
| Overhead | None | Minimal (stop_source) |
| C++ version | C++11 | C++20 |

---

## Compile

```bash
g++ -std=c++20 -pthread -o program main.cpp
```
