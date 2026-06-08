/*
Singleton Pattern (Mẫu Singleton)

Định nghĩa:
- Đảm bảo một lớp chỉ có đúng một đối tượng duy nhất trong toàn bộ chương trình.
- Cung cấp điểm truy cập toàn cục tới đối tượng đó.

Khi nào dùng:
- Cần dùng chung tài nguyên: logger, config, connection manager...
- Muốn tránh tạo nhiều instance gây lệch trạng thái.

Ví dụ này:
- VehicleLogger là Singleton để toàn hệ thống dùng chung 1 logger.
- Toyota/Hyundai/Mazda đều ghi log qua cùng một instance.
*/

#include <iostream>
#include <string>

class VehicleLogger {
private:
    VehicleLogger() = default;

public:
    VehicleLogger(const VehicleLogger&) = delete;
    VehicleLogger& operator=(const VehicleLogger&) = delete;

    static VehicleLogger& getInstance() {
        // Meyers' Singleton: thread-safe từ C++11.
        static VehicleLogger instance;
        return instance;
    }

    void log(const std::string& message) {
        std::cout << "[VehicleLogger] " << message << '\n';
    }
};

int main() {
    VehicleLogger& loggerA = VehicleLogger::getInstance();
    VehicleLogger& loggerB = VehicleLogger::getInstance();

    loggerA.log("Toyota Corolla đã vào xưởng bảo dưỡng.");
    loggerB.log("Hyundai Elantra đã hoàn tất kiểm tra.");
    loggerA.log("Mazda 3 đang chờ thay dầu.");

    std::cout << "Cùng một instance? "
              << ((&loggerA == &loggerB) ? "Đúng" : "Sai") << '\n';

    return 0;
}
