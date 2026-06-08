/*
Strategy Pattern (Mẫu chiến lược)

Định nghĩa:
- Đóng gói các thuật toán/chiến lược vào các lớp riêng.
- Có thể hoán đổi chiến lược tại runtime mà không sửa code client.

Khi nào dùng:
- Có nhiều cách xử lý cho cùng một bài toán.
- Muốn tránh if-else lớn theo từng chế độ.

Ví dụ này:
- Bài toán: tính mức tiêu thụ nhiên liệu giả lập theo chế độ lái.
- Có 3 chiến lược: Eco, Normal, Sport.
- CarContext dùng strategy hiện tại để tính kết quả.
*/

#include <iostream>
#include <memory>
#include <string>

class DrivingStrategy {
public:
    virtual ~DrivingStrategy() = default;
    virtual double fuelPer100Km() const = 0;
    virtual std::string name() const = 0;
};

class EcoStrategy : public DrivingStrategy {
public:
    double fuelPer100Km() const override { return 5.2; }
    std::string name() const override { return "Eco"; }
};

class NormalStrategy : public DrivingStrategy {
public:
    double fuelPer100Km() const override { return 6.4; }
    std::string name() const override { return "Normal"; }
};

class SportStrategy : public DrivingStrategy {
public:
    double fuelPer100Km() const override { return 8.1; }
    std::string name() const override { return "Sport"; }
};

class CarContext {
private:
    std::shared_ptr<DrivingStrategy> strategy;

public:
    explicit CarContext(std::shared_ptr<DrivingStrategy> strategy)
        : strategy(std::move(strategy)) {}

    void setStrategy(std::shared_ptr<DrivingStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void showFuelInfo(const std::string& carName) const {
        std::cout << carName << " | Chế độ " << strategy->name()
                  << " | Tiêu thụ: " << strategy->fuelPer100Km() << " L/100km\n";
    }
};

int main() {
    CarContext mazda(std::make_shared<NormalStrategy>());

    mazda.showFuelInfo("Mazda 3");

    // Đổi chiến lược tại runtime.
    mazda.setStrategy(std::make_shared<EcoStrategy>());
    mazda.showFuelInfo("Mazda 3");

    mazda.setStrategy(std::make_shared<SportStrategy>());
    mazda.showFuelInfo("Mazda 3");

    return 0;
}
