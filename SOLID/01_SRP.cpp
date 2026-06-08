/*
SRP - Single Responsibility Principle (Nguyên tắc đơn trách nhiệm)

Định nghĩa:
- Mỗi lớp chỉ nên có một trách nhiệm chính, và do đó chỉ có một lý do để thay đổi.
- Khi một lớp vừa xử lý nghiệp vụ, vừa ghi log, vừa lưu file... thì lớp đó đang ôm nhiều trách nhiệm.

Trong ví dụ này:
- Vehicle chỉ quản lý dữ liệu xe (hãng, mẫu xe) và cung cấp thông tin xe.
- VehicleLogger chỉ lo ghi log thông tin xe ra màn hình.

Lợi ích:
- Dễ bảo trì: đổi format log thì sửa VehicleLogger, không đụng vào Vehicle.
- Dễ kiểm thử: test nghiệp vụ của Vehicle tách biệt với test log.
*/

#include <iostream>
#include <string>

class Vehicle {
private:
    std::string brand;
    std::string model;

public:
    Vehicle(const std::string& brand, const std::string& model)
        : brand(brand), model(model) {}

    std::string getInfo() const {
        return brand + " " + model;
    }
};

class VehicleLogger {
public:
    void logVehicle(const Vehicle& vehicle) const {
        // VehicleLogger chỉ thực hiện đúng một việc: ghi log.
        std::cout << "[LOG] Vehicle: " << vehicle.getInfo() << '\n';
    }
};

int main() {
    // Vehicle chỉ chứa và cung cấp thông tin xe.
    Vehicle toyota("Toyota", "Corolla");
    VehicleLogger logger;

    // Khi cần log, ta ủy quyền cho VehicleLogger.
    logger.logVehicle(toyota);
    return 0;
}
