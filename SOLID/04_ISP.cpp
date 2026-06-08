/*
ISP - Interface Segregation Principle (Nguyên tắc phân tách interface)

Định nghĩa:
- Client không nên bị ép phụ thuộc vào những method mà nó không sử dụng.
- Thay vì tạo một interface rất lớn, hãy tách thành các interface nhỏ theo nhu cầu thực tế.

Trong ví dụ này:
- IDrivable chỉ chứa hành vi lái xe.
- ILoggable chỉ chứa hành vi ghi log.
- HyundaiCar chỉ cần lái xe nên chỉ triển khai IDrivable.
- ToyotaCar cần cả lái và log nên triển khai cả hai interface.

Lợi ích:
- Tránh class phải cài đặt các method "thừa".
- Dễ mở rộng theo đúng nhu cầu từng đối tượng.
*/

#include <iostream>
#include <string>

class IDrivable {
public:
    virtual ~IDrivable() = default;
    // Interface nhỏ, chỉ dành cho chức năng di chuyển.
    virtual void drive() const = 0;
};

class ILoggable {
public:
    virtual ~ILoggable() = default;
    // Interface nhỏ, chỉ dành cho chức năng ghi log.
    virtual void logStatus() const = 0;
};

class ToyotaCar : public IDrivable, public ILoggable {
public:
    void drive() const override {
        std::cout << "Toyota is driving" << '\n';
    }

    void logStatus() const override {
        std::cout << "[LOG] Toyota status is OK" << '\n';
    }
};

class HyundaiCar : public IDrivable {
public:
    void drive() const override {
        std::cout << "Hyundai is driving" << '\n';
    }
};

void startTrip(const IDrivable& drivable) {
    // Hàm này chỉ cần khả năng lái, không quan tâm log.
    drivable.drive();
}

void writeLog(const ILoggable& loggable) {
    // Hàm này chỉ cần khả năng ghi log.
    loggable.logStatus();
}

int main() {
    ToyotaCar toyota;
    HyundaiCar hyundai;

    startTrip(toyota);
    startTrip(hyundai);

    writeLog(toyota);
    return 0;
}
