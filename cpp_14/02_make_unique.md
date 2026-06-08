# std::make_unique (C++14)

## Tổng quan

`std::make_unique` tạo `std::unique_ptr` một cách an toàn và ngắn gọn.

**Tại sao dùng?**
- Tránh memory leak trong trường hợp exception
- Code ngắn gọn hơn so với `new` + `unique_ptr`
- Không cần viết type 2 lần
- Tuân thủ nguyên tắc "no raw new/delete"

> Ref: https://www.learncpp.com/cpp-tutorial/stdunique_ptr/

---

## Cách sử dụng

### Cơ bản

```cpp
#include <iostream>
#include <memory>
#include <string>

class Sensor {
public:
    Sensor(int id, std::string name)
        : m_id(id), m_name(std::move(name)) {
        std::cout << "Sensor " << m_name << " created\n";
    }

    ~Sensor() {
        std::cout << "Sensor " << m_name << " destroyed\n";
    }

    void read() const {
        std::cout << "[" << m_id << "] " << m_name << " reading data...\n";
    }

private:
    int m_id;
    std::string m_name;
};

int main() {
    // C++11: phải viết type 2 lần, không an toàn với exception
    // std::unique_ptr<Sensor> s1(new Sensor(1, "Temperature"));

    // C++14: ngắn gọn, an toàn
    auto s1 = std::make_unique<Sensor>(1, "Temperature");
    auto s2 = std::make_unique<Sensor>(2, "Pressure");

    s1->read();
    s2->read();

    // Tự động giải phóng khi ra khỏi scope
    return 0;
}
```

### Tạo mảng unique_ptr

```cpp
#include <iostream>
#include <memory>

int main() {
    // Tạo mảng 10 phần tử, tự zero-initialized
    auto arr = std::make_unique<int[]>(10);

    for (int i = 0; i < 10; ++i) {
        arr[i] = i * i;
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << arr[i] << ' '; // 0 1 4 9 16 25 36 49 64 81
    }
    std::cout << '\n';

    return 0;
}
```

### Factory pattern với make_unique

```cpp
#include <iostream>
#include <memory>
#include <string>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
    double m_radius;
public:
    explicit Circle(double radius) : m_radius(radius) {}
    void draw() const override {
        std::cout << "Drawing circle, radius=" << m_radius << '\n';
    }
};

class Rectangle : public Shape {
    double m_w, m_h;
public:
    Rectangle(double w, double h) : m_w(w), m_h(h) {}
    void draw() const override {
        std::cout << "Drawing rectangle " << m_w << "x" << m_h << '\n';
    }
};

// Factory function trả về unique_ptr
std::unique_ptr<Shape> createShape(const std::string& type) {
    if (type == "circle") {
        return std::make_unique<Circle>(5.0);
    } else if (type == "rect") {
        return std::make_unique<Rectangle>(3.0, 4.0);
    }
    return nullptr;
}

int main() {
    auto shape1 = createShape("circle");
    auto shape2 = createShape("rect");

    if (shape1) shape1->draw();
    if (shape2) shape2->draw();

    // Transfer ownership
    auto shape3 = std::move(shape1);
    // shape1 giờ là nullptr
    if (!shape1) std::cout << "shape1 is now empty\n";
    if (shape3) shape3->draw();

    return 0;
}
```

### Dùng trong container

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Task {
    std::string name;
    int priority;

    Task(std::string n, int p) : name(std::move(n)), priority(p) {}

    void execute() const {
        std::cout << "Executing: " << name << " (priority " << priority << ")\n";
    }
};

int main() {
    std::vector<std::unique_ptr<Task>> tasks;

    // emplace_back không cần std::move
    tasks.push_back(std::make_unique<Task>("Send CAN frame", 1));
    tasks.push_back(std::make_unique<Task>("Log data", 3));
    tasks.push_back(std::make_unique<Task>("Update display", 2));

    for (const auto& task : tasks) {
        task->execute();
    }

    return 0;
}
```

---

## Tại sao make_unique an toàn hơn new?

```cpp
// Trường hợp NGUY HIỂM với C++11:
void process(std::unique_ptr<A> a, std::unique_ptr<B> b);

// Nếu compiler thực thi theo thứ tự:
// 1. new A()
// 2. new B()  ← throw exception ở đây
// 3. unique_ptr<A>(...)  ← chưa được gọi → memory leak!
process(std::unique_ptr<A>(new A()), std::unique_ptr<B>(new B()));

// AN TOÀN với make_unique:
process(std::make_unique<A>(), std::make_unique<B>());
// Mỗi make_unique hoàn thành nguyên tử → không leak
```

---

## Lỗi thường gặp

### 1. Cố copy unique_ptr

```cpp
auto p1 = std::make_unique<int>(42);
// auto p2 = p1;  // COMPILE ERROR: unique_ptr không copy được

auto p2 = std::move(p1); // OK: transfer ownership
```

### 2. Dùng make_unique với custom deleter

```cpp
// make_unique KHÔNG hỗ trợ custom deleter
// Phải dùng constructor trực tiếp:
auto file_ptr = std::unique_ptr<FILE, decltype(&fclose)>(
    fopen("test.txt", "r"), &fclose
);
```

### 3. Trả về unique_ptr từ function → không cần std::move

```cpp
std::unique_ptr<int> create() {
    auto p = std::make_unique<int>(100);
    return p;  // OK: compiler tự move (NRVO hoặc implicit move)
    // return std::move(p); // KHÔNG cần, thậm chí có thể ngăn NRVO
}
```

---

## So sánh nhanh

| Cách tạo | An toàn exception? | Ngắn gọn? |
|-----------|-------------------|-----------|
| `new T(...)` | ❌ | ❌ |
| `unique_ptr<T>(new T(...))` | ❌ (trong biểu thức phức tạp) | ❌ |
| `make_unique<T>(...)` | ✅ | ✅ |

---

## Compile

```bash
g++ -std=c++14 -o program main.cpp
```
