/*
DIP - Dependency Inversion Principle (Nguyên tắc đảo ngược phụ thuộc)

Định nghĩa:
- Module cấp cao không nên phụ thuộc trực tiếp vào module cấp thấp.
- Cả hai nên phụ thuộc vào abstraction (interface/abstract class).
- Chi tiết kỹ thuật (ví dụ ConsoleLogger, FileLogger) phải phụ thuộc vào abstraction,
  chứ không để nghiệp vụ phụ thuộc cứng vào chi tiết.

Trong ví dụ này:
- VehicleService là module nghiệp vụ cấp cao.
- ILogger là abstraction trung gian.
- ConsoleLogger là module cấp thấp triển khai ILogger.
- VehicleService nhận ILogger qua constructor (Dependency Injection),
  nên có thể thay logger khác mà không sửa code nghiệp vụ.

Lợi ích:
- Dễ thay thế thành phần kỹ thuật (console/file/database logger).
- Dễ test bằng mock logger.
*/

#include <iostream>
#include <memory>
#include <string>

class ILogger {
public:
    virtual ~ILogger() = default;
    // Hợp đồng log chung cho mọi loại logger.
    virtual void log(const std::string& message) const = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) const override {
        std::cout << "[Console] " << message << '\n';
    }
};

class VehicleService {
private:
    // Phụ thuộc abstraction thay vì phụ thuộc trực tiếp ConsoleLogger.
    std::shared_ptr<ILogger> logger;

public:
    explicit VehicleService(std::shared_ptr<ILogger> logger)
        : logger(std::move(logger)) {}

    void registerVehicle(const std::string& brand) const {
        logger->log("Register vehicle: " + brand);
    }
};

int main() {
    // Có thể đổi sang FileLogger/DatabaseLogger nếu tạo class tương ứng.
    std::shared_ptr<ILogger> logger = std::make_shared<ConsoleLogger>();
    VehicleService service(logger);

    service.registerVehicle("Toyota");
    service.registerVehicle("Hyundai");
    service.registerVehicle("Mazda");

    return 0;
}
