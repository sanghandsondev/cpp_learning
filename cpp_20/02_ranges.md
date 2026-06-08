# Ranges (C++20)

## Tổng quan

Ranges là abstraction layer trên containers: cho phép **chain operations** (filter, transform, take...) kiểu pipeline mà vẫn efficient nhờ lazy evaluation.

**Tại sao dùng?**
- Code declarative, dễ đọc hơn raw loops
- Lazy evaluation: chỉ tính toán khi iterate
- Composable: pipe (`|`) nhiều operations
- Range algorithms nhận container trực tiếp (không cần begin/end)

---

## Cách sử dụng

### Range algorithms (thay thế begin/end)

```cpp
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <string>

int main() {
    std::vector<int> nums = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // C++17: std::sort(nums.begin(), nums.end());
    // C++20: truyền range trực tiếp
    std::ranges::sort(nums);

    // Find
    auto it = std::ranges::find(nums, 7);
    if (it != nums.end()) {
        std::cout << "Found: " << *it << '\n';
    }

    // Min/Max
    auto [minIt, maxIt] = std::ranges::minmax_element(nums);
    std::cout << "Min: " << *minIt << " Max: " << *maxIt << '\n';

    // Count
    std::vector<int> data = {1, 2, 2, 3, 2, 4, 2};
    std::cout << "Count of 2: " << std::ranges::count(data, 2) << '\n';

    return 0;
}
```

### Views: lazy pipeline với pipe operator

```cpp
#include <iostream>
#include <vector>
#include <ranges>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Filter + Transform: KHÔNG tạo vector trung gian
    auto result = nums
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });

    std::cout << "Even squares: ";
    for (int val : result) {
        std::cout << val << ' '; // 4 16 36 64 100
    }
    std::cout << '\n';

    // Take / Drop
    auto first3 = nums | std::views::take(3);
    auto skip5 = nums | std::views::drop(5);

    std::cout << "First 3: ";
    for (int v : first3) std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "After 5: ";
    for (int v : skip5) std::cout << v << ' ';
    std::cout << '\n';

    return 0;
}
```

### Real-world: data processing pipeline

```cpp
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <string>

struct SensorReading {
    int sensorId;
    double value;
    bool valid;
};

int main() {
    std::vector<SensorReading> readings = {
        {1, 25.5, true}, {2, -1.0, false}, {3, 30.2, true},
        {4, 0.0, false}, {5, 22.1, true},  {6, 28.7, true},
        {7, -5.0, false},{8, 26.3, true},
    };

    // Pipeline: valid → values > 25 → take 3
    auto highTemps = readings
        | std::views::filter([](const auto& r) { return r.valid; })
        | std::views::transform([](const auto& r) { return r.value; })
        | std::views::filter([](double v) { return v > 25.0; })
        | std::views::take(3);

    std::cout << "High temps: ";
    for (double t : highTemps) {
        std::cout << t << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

### Projections (thay custom comparators)

```cpp
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <string>

struct Employee {
    std::string name;
    int age;
    double salary;
};

int main() {
    std::vector<Employee> team = {
        {"Alice", 30, 75000}, {"Bob", 25, 65000},
        {"Charlie", 35, 85000}, {"Diana", 28, 70000},
    };

    // Sort by salary (projection thay vì custom comparator dài)
    std::ranges::sort(team, std::ranges::less{}, &Employee::salary);

    for (const auto& e : team) {
        std::cout << e.name << ": $" << e.salary << '\n';
    }

    // Find by name
    auto it = std::ranges::find(team, "Charlie", &Employee::name);
    if (it != team.end()) {
        std::cout << "Found: " << it->name << " age " << it->age << '\n';
    }

    return 0;
}
```

### iota, reverse, keys/values

```cpp
#include <iostream>
#include <ranges>
#include <vector>
#include <map>
#include <string>

int main() {
    // Generate sequence
    for (int i : std::views::iota(1, 11)) {
        std::cout << i << ' '; // 1 2 3 ... 10
    }
    std::cout << '\n';

    // Reverse
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (int i : v | std::views::reverse) {
        std::cout << i << ' '; // 5 4 3 2 1
    }
    std::cout << '\n';

    // Map keys/values
    std::map<std::string, int> scores = {{"A", 90}, {"B", 80}, {"C", 70}};
    for (const auto& key : scores | std::views::keys) {
        std::cout << key << ' ';
    }
    std::cout << '\n';

    return 0;
}
```

---

## Lỗi thường gặp

### 1. View là lazy → dangling nếu source bị destroy

```cpp
// SAI: vector tạm destroy, view dangling
auto bad_view() {
    std::vector<int> temp = {1, 2, 3};
    return temp | std::views::take(2); // DANGLING!
}

// ĐÚNG: source phải outlive view
std::vector<int> data = {1, 2, 3};
auto view = data | std::views::take(2); // OK
```

### 2. Data race khi modify source

```cpp
std::vector<int> v = {1, 2, 3};
auto view = v | std::views::filter([](int n) { return n > 1; });

v.push_back(4); // Có thể invalidate view!
// Dùng view sau modify → undefined behavior
```

### 3. filter_view không phải random_access → không sort được

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto filtered = v | std::views::filter([](int n) { return n > 2; });
// std::ranges::sort(filtered); // Error: filter_view chỉ bidirectional

// Collect vào vector trước
std::vector<int> collected(filtered.begin(), filtered.end());
std::ranges::sort(collected); // OK
```

---

## Compile

```bash
g++ -std=c++20 -o program main.cpp
```
