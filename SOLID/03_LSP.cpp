/*
LSP - Liskov Substitution Principle (Nguyên tắc thay thế Liskov)

Định nghĩa:
- Nếu S là lớp con của T, thì đối tượng kiểu S phải thay thế được đối tượng kiểu T
    mà chương trình vẫn chạy đúng theo kỳ vọng.
- Nói ngắn gọn: dùng lớp con ở nơi dùng lớp cha mà không làm hỏng hành vi.

Trong ví dụ này:
- Vehicle là kiểu cha, định nghĩa hợp đồng chung: có thể startEngine() và có name().
- Toyota, Hyundai, Mazda là các lớp con, đều giữ đúng hợp đồng đó.
- Hàm testDrive nhận Vehicle&, vì vậy có thể truyền bất kỳ hãng nào mà không cần sửa hàm.

Lợi ích:
- Code phía client (testDrive) ổn định và tái sử dụng tốt.
- Tránh tình trạng lớp con phá vỡ kỳ vọng của lớp cha.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
public:
    virtual ~Vehicle() = default;
    // Hành vi mặc định, lớp con có thể override nhưng vẫn phải "khởi động được".
    virtual void startEngine() const {
        std::cout << "Vehicle engine started" << '\n';
    }
    virtual std::string name() const = 0;
};

class Toyota : public Vehicle {
public:
    void startEngine() const override {
        std::cout << "Toyota engine started smoothly" << '\n';
    }

    std::string name() const override {
        return "Toyota";
    }
};

class Hyundai : public Vehicle {
public:
    void startEngine() const override {
        std::cout << "Hyundai engine started" << '\n';
    }

    std::string name() const override {
        return "Hyundai";
    }
};

class Mazda : public Vehicle {
public:
    void startEngine() const override {
        std::cout << "Mazda engine started with sporty sound" << '\n';
    }

    std::string name() const override {
        return "Mazda";
    }
};

void testDrive(const Vehicle& vehicle) {
    // Chỉ phụ thuộc vào kiểu Vehicle: đây là điểm thể hiện LSP.
    std::cout << "Testing: " << vehicle.name() << '\n';
    vehicle.startEngine();
}

int main() {
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    vehicles.push_back(std::make_shared<Toyota>());
    vehicles.push_back(std::make_shared<Hyundai>());
    vehicles.push_back(std::make_shared<Mazda>());

    for (const auto& v : vehicles) {
        testDrive(*v);
    }

    return 0;
}
