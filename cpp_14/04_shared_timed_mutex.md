# std::shared_timed_mutex - Reader-Writer Lock (C++14)

## Tổng quan

`std::shared_timed_mutex` cho phép **multiple readers** đọc đồng thời, nhưng **writer** phải exclusive. Pattern cực kỳ phổ biến khi data được đọc nhiều hơn ghi.

**Tại sao dùng?**
- Nhiều threads đọc cùng lúc → throughput cao hơn exclusive mutex
- Config/cache: đọc thường xuyên, ghi hiếm khi
- Sensor data: nhiều consumer đọc, 1 producer ghi
- Alternative: C++17 có `std::shared_mutex` (đơn giản hơn, không có timed)

---

## Cách sử dụng

### Cơ bản: multiple readers, single writer

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <string>
#include <chrono>

class SensorDataStore {
    mutable std::shared_timed_mutex m_mutex;
    double m_temperature = 0.0;
    double m_pressure = 0.0;
    int m_updateCount = 0;

public:
    // WRITER: exclusive lock - chỉ 1 thread ghi tại 1 thời điểm
    void update(double temp, double pressure) {
        std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
        m_temperature = temp;
        m_pressure = pressure;
        ++m_updateCount;
    }

    // READER: shared lock - nhiều threads đọc đồng thời
    std::pair<double, double> read() const {
        std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
        return {m_temperature, m_pressure};
    }

    int getUpdateCount() const {
        std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
        return m_updateCount;
    }
};

int main() {
    SensorDataStore store;

    // 1 writer thread
    std::thread writer([&store]() {
        for (int i = 0; i < 100; ++i) {
            store.update(20.0 + i * 0.1, 1013.0 + i * 0.5);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // 5 reader threads chạy đồng thời
    std::vector<std::thread> readers;
    for (int id = 0; id < 5; ++id) {
        readers.emplace_back([&store, id]() {
            for (int i = 0; i < 50; ++i) {
                auto [temp, pres] = store.read();
                if (i % 10 == 0) {
                    std::cout << "Reader " << id << ": T=" << temp
                              << " P=" << pres << '\n';
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        });
    }

    writer.join();
    for (auto& r : readers) r.join();

    std::cout << "Total updates: " << store.getUpdateCount() << '\n';
    return 0;
}
```

### Configuration cache pattern

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <map>
#include <string>
#include <optional>

class ConfigCache {
    mutable std::shared_timed_mutex m_mutex;
    std::map<std::string, std::string> m_config;

public:
    // Read (shared) - nhiều threads đọc cùng lúc
    std::optional<std::string> get(const std::string& key) const {
        std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
        auto it = m_config.find(key);
        if (it != m_config.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Write (exclusive) - chỉ 1 thread ghi
    void set(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
        m_config[key] = value;
    }

    // Bulk update (exclusive)
    void loadDefaults() {
        std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
        m_config["baudrate"] = "500000";
        m_config["interface"] = "can0";
        m_config["timeout_ms"] = "1000";
        m_config["log_level"] = "INFO";
    }

    size_t size() const {
        std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
        return m_config.size();
    }
};

int main() {
    ConfigCache config;
    config.loadDefaults();

    // Nhiều threads đọc config đồng thời
    std::vector<std::thread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([&config, i]() {
            for (int j = 0; j < 100; ++j) {
                auto val = config.get("baudrate");
                if (val && j % 25 == 0) {
                    std::cout << "Worker " << i << ": baudrate=" << *val << '\n';
                }
            }
        });
    }

    // 1 thread thỉnh thoảng update
    std::thread updater([&config]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        config.set("baudrate", "1000000");
        std::cout << "Config updated!\n";
    });

    for (auto& w : workers) w.join();
    updater.join();

    return 0;
}
```

### Timed lock: timeout thay vì block mãi

```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <chrono>

class TimedResource {
    std::shared_timed_mutex m_mutex;
    int m_value = 0;

public:
    // Try lock with timeout
    bool tryUpdate(int newValue, std::chrono::milliseconds timeout) {
        // Cố gắng lấy exclusive lock trong timeout
        if (m_mutex.try_lock_for(timeout)) {
            m_value = newValue;
            std::cout << "Updated to " << newValue << '\n';
            m_mutex.unlock();
            return true;
        }
        std::cout << "Timeout! Could not update\n";
        return false;
    }

    // Try shared lock with timeout
    std::optional<int> tryRead(std::chrono::milliseconds timeout) const {
        if (m_mutex.try_lock_shared_for(timeout)) {
            int val = m_value;
            m_mutex.unlock_shared();
            return val;
        }
        return std::nullopt; // timeout
    }
};

int main() {
    TimedResource resource;

    // Writer giữ lock lâu
    std::thread longWriter([&resource]() {
        // Simulate long write
        resource.tryUpdate(100, std::chrono::milliseconds(1000));
    });

    // Reader với timeout ngắn
    std::thread impatientReader([&resource]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        auto val = resource.tryRead(std::chrono::milliseconds(50));
        if (val) {
            std::cout << "Read: " << *val << '\n';
        } else {
            std::cout << "Reader timed out\n";
        }
    });

    longWriter.join();
    impatientReader.join();
    return 0;
}
```

---

## Lỗi thường gặp

### 1. Upgrade read lock → write lock (DEADLOCK!)

```cpp
// SAI: giữ shared lock rồi cố lấy unique lock = DEADLOCK
void deadlockExample(std::shared_timed_mutex& mtx) {
    std::shared_lock<std::shared_timed_mutex> readLock(mtx);
    // ... đọc data ...
    
    // Muốn ghi → CẦN unique lock nhưng ĐANG giữ shared lock!
    // std::unique_lock<std::shared_timed_mutex> writeLock(mtx); // DEADLOCK!
}

// ĐÚNG: release read lock trước, rồi lấy write lock
void correctUpgrade(std::shared_timed_mutex& mtx, int& data) {
    int localCopy;
    {
        std::shared_lock<std::shared_timed_mutex> readLock(mtx);
        localCopy = data;
    } // release read lock
    
    // Process localCopy...
    
    if (localCopy < 100) {
        std::unique_lock<std::shared_timed_mutex> writeLock(mtx);
        data = localCopy + 1; // check lại nếu cần (data có thể đã thay đổi)
    }
}
```

### 2. Dùng shared_lock cho write (silently broken)

```cpp
std::shared_timed_mutex mtx;
int shared_data = 0;

// SAI: shared_lock cho phép nhiều threads vào → race condition khi write!
void wrongWriter() {
    std::shared_lock<std::shared_timed_mutex> lock(mtx);
    shared_data++;  // DATA RACE! Nhiều threads có thể vào đây
}

// ĐÚNG: unique_lock cho write
void correctWriter() {
    std::unique_lock<std::shared_timed_mutex> lock(mtx);
    shared_data++;  // Safe: exclusive access
}
```

### 3. Lock quá lâu → starve writers

```cpp
// SAI: reader giữ lock lâu → writer bị starve
void slowReader(std::shared_timed_mutex& mtx) {
    std::shared_lock<std::shared_timed_mutex> lock(mtx);
    // Heavy processing TRONG lock → writer phải đợi rất lâu
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

// ĐÚNG: copy data ra, release lock, rồi process
void fastReader(std::shared_timed_mutex& mtx, const std::vector<int>& data) {
    std::vector<int> localCopy;
    {
        std::shared_lock<std::shared_timed_mutex> lock(mtx);
        localCopy = data; // Quick copy
    } // Release lock ngay
    
    // Heavy processing NGOÀI lock
    // ... process localCopy ...
}
```

---

## Performance: Khi nào dùng shared_mutex vs regular mutex?

| Tình huống | Dùng |
|------------|------|
| Read >> Write (90%+ read) | `shared_timed_mutex` |
| Read ≈ Write | `std::mutex` (simpler, less overhead) |
| Write-heavy | `std::mutex` |
| Cần timeout | `shared_timed_mutex` |
| C++17 available, không cần timeout | `std::shared_mutex` (lighter) |

---

## Compile

```bash
g++ -std=c++14 -pthread -o program main.cpp
```
