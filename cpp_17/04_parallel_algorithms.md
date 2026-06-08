# Parallel Algorithms (C++17)

## Tổng quan

C++17 thêm **execution policies** cho STL algorithms: chạy parallel trên nhiều cores mà KHÔNG cần viết thread code thủ công.

**Tại sao dùng?**
- Tận dụng multi-core CPU chỉ bằng 1 parameter thêm vào
- Không cần tự quản lý threads, task splitting, synchronization
- Sort, transform, reduce... tự động parallel
- Production-ready: tested, optimized

---

## Cách sử dụng

### Execution policies

```cpp
#include <iostream>
#include <algorithm>
#include <execution> // C++17 execution policies
#include <vector>
#include <numeric>
#include <chrono>

int main() {
    std::vector<int> data(10'000'000);
    std::iota(data.begin(), data.end(), 1); // 1, 2, 3, ..., 10M

    // === Sequential (mặc định, 1 thread) ===
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::seq, data.begin(), data.end(), std::greater<>());
    auto end = std::chrono::high_resolution_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // === Parallel (multi-thread) ===
    start = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par, data.begin(), data.end());
    end = std::chrono::high_resolution_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // === Parallel + vectorized (SIMD hints) ===
    start = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par_unseq, data.begin(), data.end(), std::greater<>());
    end = std::chrono::high_resolution_clock::now();
    auto par_unseq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Sequential:     " << seq_time.count() << " ms\n";
    std::cout << "Parallel:       " << par_time.count() << " ms\n";
    std::cout << "Par+Vectorized: " << par_unseq_time.count() << " ms\n";

    return 0;
}
```

### std::transform (parallel map)

```cpp
#include <iostream>
#include <algorithm>
#include <execution>
#include <vector>
#include <cmath>
#include <chrono>

struct SensorReading {
    double raw;
    double calibrated;
};

int main() {
    // Simulate 1M sensor readings
    std::vector<SensorReading> readings(1'000'000);
    for (size_t i = 0; i < readings.size(); ++i) {
        readings[i].raw = static_cast<double>(i) * 0.001;
    }

    // Parallel calibration: heavy computation per element
    std::for_each(std::execution::par, readings.begin(), readings.end(),
        [](SensorReading& r) {
            // Simulate complex calibration
            r.calibrated = std::sin(r.raw) * 100.0 + std::cos(r.raw * 2.0) * 50.0;
        }
    );

    // Parallel transform: input → output
    std::vector<double> rawValues(1'000'000);
    std::vector<double> results(1'000'000);
    std::iota(rawValues.begin(), rawValues.end(), 1.0);

    std::transform(std::execution::par, rawValues.begin(), rawValues.end(),
        results.begin(),
        [](double x) { return std::sqrt(x) + std::log(x + 1.0); }
    );

    std::cout << "First 5 results: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << results[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

### std::reduce (parallel sum/accumulate)

```cpp
#include <iostream>
#include <numeric>
#include <execution>
#include <vector>
#include <chrono>

int main() {
    std::vector<double> data(10'000'000);
    std::iota(data.begin(), data.end(), 1.0);

    // Sequential accumulate
    auto start = std::chrono::high_resolution_clock::now();
    double sum_seq = std::accumulate(data.begin(), data.end(), 0.0);
    auto end = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Parallel reduce (C++17) - associative + commutative operation
    start = std::chrono::high_resolution_clock::now();
    double sum_par = std::reduce(std::execution::par, data.begin(), data.end(), 0.0);
    end = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Sequential sum: " << sum_seq << " (" << t1.count() << " ms)\n";
    std::cout << "Parallel sum:   " << sum_par << " (" << t2.count() << " ms)\n";

    // transform_reduce: map + reduce trong 1 bước
    // Ví dụ: tính sum of squares
    double sumSquares = std::transform_reduce(
        std::execution::par,
        data.begin(), data.end(),
        0.0,                           // init
        std::plus<>(),                 // reduce operation
        [](double x) { return x * x; } // transform
    );
    std::cout << "Sum of squares: " << sumSquares << '\n';

    return 0;
}
```

### Parallel find, count, all_of

```cpp
#include <iostream>
#include <algorithm>
#include <execution>
#include <vector>
#include <string>

struct LogEntry {
    int timestamp;
    std::string level;
    std::string message;
};

int main() {
    // Simulate large log
    std::vector<LogEntry> logs(1'000'000);
    for (size_t i = 0; i < logs.size(); ++i) {
        logs[i].timestamp = static_cast<int>(i);
        logs[i].level = (i % 100 == 0) ? "ERROR" : "INFO";
        logs[i].message = "Event " + std::to_string(i);
    }

    // Parallel count errors
    auto errorCount = std::count_if(std::execution::par,
        logs.begin(), logs.end(),
        [](const LogEntry& entry) { return entry.level == "ERROR"; }
    );
    std::cout << "Errors: " << errorCount << '\n';

    // Parallel check: any errors?
    bool hasErrors = std::any_of(std::execution::par,
        logs.begin(), logs.end(),
        [](const LogEntry& entry) { return entry.level == "ERROR"; }
    );
    std::cout << "Has errors: " << std::boolalpha << hasErrors << '\n';

    // Parallel find first error
    auto it = std::find_if(std::execution::par,
        logs.begin(), logs.end(),
        [](const LogEntry& entry) { return entry.level == "ERROR"; }
    );
    if (it != logs.end()) {
        std::cout << "First error at timestamp: " << it->timestamp << '\n';
    }

    return 0;
}
```

### Real-world: parallel data processing pipeline

```cpp
#include <iostream>
#include <algorithm>
#include <execution>
#include <vector>
#include <numeric>
#include <cmath>

struct CANFrame {
    uint32_t id;
    uint8_t data[8];
    double timestamp;
    bool valid;
};

// Simulate processing pipeline
int main() {
    // Simulate 1M CAN frames
    std::vector<CANFrame> frames(1'000'000);
    for (size_t i = 0; i < frames.size(); ++i) {
        frames[i].id = static_cast<uint32_t>(i % 256);
        frames[i].timestamp = i * 0.001;
        frames[i].valid = (i % 10 != 7); // 90% valid
    }

    // Step 1: Parallel validation
    std::for_each(std::execution::par, frames.begin(), frames.end(),
        [](CANFrame& f) {
            // Validate checksum, DLC, etc.
            f.valid = f.valid && (f.id < 0x800);
        }
    );

    // Step 2: Parallel count valid frames per ID
    auto validCount = std::count_if(std::execution::par,
        frames.begin(), frames.end(),
        [](const CANFrame& f) { return f.valid; }
    );

    // Step 3: Parallel partition (valid first, invalid last)
    auto partition_point = std::partition(std::execution::par,
        frames.begin(), frames.end(),
        [](const CANFrame& f) { return f.valid; }
    );

    size_t validFrames = std::distance(frames.begin(), partition_point);
    std::cout << "Valid frames: " << validFrames << " / " << frames.size() << '\n';
    std::cout << "Valid count:  " << validCount << '\n';

    return 0;
}
```

---

## Lỗi thường gặp

### 1. Data race trong parallel lambda

```cpp
std::vector<int> data = {1, 2, 3, 4, 5};
int sum = 0;

// SAI: race condition! Nhiều threads modify sum đồng thời
std::for_each(std::execution::par, data.begin(), data.end(),
    [&sum](int x) { sum += x; }  // DATA RACE!
);

// ĐÚNG: dùng std::reduce hoặc atomic
#include <atomic>
std::atomic<int> safe_sum{0};
std::for_each(std::execution::par, data.begin(), data.end(),
    [&safe_sum](int x) { safe_sum += x; } // OK: atomic
);

// HOẶC dùng reduce (tốt nhất)
int result = std::reduce(std::execution::par, data.begin(), data.end(), 0);
```

### 2. reduce yêu cầu associative + commutative

```cpp
// std::accumulate: thứ tự cố định (left-to-right)
// std::reduce: thứ tự KHÔNG cố định (parallel)

// SAI: floating-point subtraction không associative
// (a - b) - c ≠ a - (b - c)
// double bad = std::reduce(std::execution::par, data.begin(), data.end(), 0.0,
//     std::minus<>()); // Kết quả KHÔNG deterministic!

// ĐÚNG: dùng operations associative + commutative
double good = std::reduce(std::execution::par, data.begin(), data.end(), 0.0,
    std::plus<>()); // OK: addition
```

### 3. Exception trong parallel = std::terminate

```cpp
// Nếu lambda throw exception trong parallel execution → std::terminate()!
// ĐÚNG: catch trong lambda
std::for_each(std::execution::par, data.begin(), data.end(),
    [](int& x) {
        try {
            // risky operation
            if (x < 0) throw std::runtime_error("negative");
            x = x * 2;
        } catch (...) {
            x = 0; // handle locally
        }
    }
);
```

### 4. Small data → parallel overhead > speedup

```cpp
std::vector<int> small(100); // Quá nhỏ!

// Parallel overhead (thread creation, sync) > computation time
// → CHẬM hơn sequential!
std::sort(std::execution::par, small.begin(), small.end());

// Rule of thumb: parallel chỉ nhanh hơn khi data > ~10,000 elements
// hoặc computation per element đủ heavy
```

---

## Execution policies

| Policy | Behavior |
|--------|----------|
| `std::execution::seq` | Sequential (1 thread, giống default) |
| `std::execution::par` | Parallel (multi-thread) |
| `std::execution::par_unseq` | Parallel + vectorized (SIMD, no mutex/locks in lambda!) |
| `std::execution::unseq` (C++20) | Vectorized only (single thread, SIMD) |

---

## Compile

```bash
# GCC: cần link TBB (Threading Building Blocks)
g++ -std=c++17 -ltbb -o program main.cpp

# Hoặc nếu dùng libstdc++ parallel mode:
g++ -std=c++17 -ltbb -pthread -o program main.cpp

# Clang cũng cần TBB
# MSVC: built-in support, không cần flag thêm
```

**Cài TBB:**
```bash
# Ubuntu/Debian
sudo apt install libtbb-dev

# Fedora
sudo dnf install tbb-devel
```
