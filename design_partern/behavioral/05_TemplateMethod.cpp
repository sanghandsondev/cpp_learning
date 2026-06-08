/*
Template Method Pattern (Mẫu phương thức khuôn mẫu)

Định nghĩa:
- Định nghĩa "khung" của một thuật toán trong lớp cha.
- Một số bước cố định dùng chung, một số bước trì hoãn cho lớp con cài đặt.

Khi nào dùng:
- Nhiều quy trình có cùng khung xử lý nhưng khác chi tiết từng bước.

Ví dụ này:
- Quy trình bảo dưỡng xe có 4 bước chính:
  1) checkIn, 2) inspect, 3) repair, 4) washAndDeliver.
- Lớp cha VehicleMaintenance định nghĩa thứ tự chạy cố định.
- ToyotaMaintenance và MazdaMaintenance tùy biến bước inspect/repair.
*/

#include <iostream>
#include <string>

class VehicleMaintenance {
public:
    virtual ~VehicleMaintenance() = default;

    // Template Method: giữ cố định thứ tự các bước.
    void process() {
        checkIn();
        inspect();
        repair();
        washAndDeliver();
    }

protected:
    void checkIn() {
        std::cout << "[Bước 1] Tiếp nhận xe tại xưởng.\n";
    }

    virtual void inspect() = 0;
    virtual void repair() = 0;

    void washAndDeliver() {
        std::cout << "[Bước 4] Rửa xe và bàn giao cho khách.\n";
    }
};

class ToyotaMaintenance : public VehicleMaintenance {
protected:
    void inspect() override {
        std::cout << "[Bước 2] Toyota: Kiểm tra phanh và dầu máy.\n";
    }

    void repair() override {
        std::cout << "[Bước 3] Toyota: Thay dầu và cân chỉnh phanh.\n";
    }
};

class MazdaMaintenance : public VehicleMaintenance {
protected:
    void inspect() override {
        std::cout << "[Bước 2] Mazda: Kiểm tra hệ thống lái G-Vectoring.\n";
    }

    void repair() override {
        std::cout << "[Bước 3] Mazda: Cập nhật ECU và bảo dưỡng định kỳ.\n";
    }
};

int main() {
    std::cout << "=== Quy trình cho Toyota ===\n";
    ToyotaMaintenance toyota;
    toyota.process();

    std::cout << "\n=== Quy trình cho Mazda ===\n";
    MazdaMaintenance mazda;
    mazda.process();

    return 0;
}
