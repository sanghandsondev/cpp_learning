/*
OCP - Open/Closed Principle (Nguyên tắc đóng/mở)

Định nghĩa:
- Mở để mở rộng (Open for extension): hệ thống có thể thêm hành vi mới.
- Đóng để sửa đổi (Closed for modification): hạn chế sửa code cũ đã ổn định.

Trong ví dụ này:
- Car là abstraction (giao diện chung) cho mọi hãng xe.
- Showroom chỉ làm việc với Car, không phụ thuộc vào Toyota/Hyundai/Mazda cụ thể.
- Khi thêm hãng mới (ví dụ Kia), chỉ cần tạo class Kia kế thừa Car,
  không cần sửa logic trong Showroom.

Lợi ích:
- Giảm rủi ro làm hỏng chức năng cũ.
- Dễ mở rộng sản phẩm theo thời gian.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Car {
public:
    virtual ~Car() = default;
    // Mỗi hãng xe tự mô tả đặc điểm của mình.
    virtual std::string getDescription() const = 0;
};

class Toyota : public Car {
public:
    std::string getDescription() const override {
        return "Toyota - Reliable family car";
    }
};

class Hyundai : public Car {
public:
    std::string getDescription() const override {
        return "Hyundai - Value and technology";
    }
};

class Mazda : public Car {
public:
    std::string getDescription() const override {
        return "Mazda - Sporty design";
    }
};

class Showroom {
public:
    void showCars(const std::vector<std::shared_ptr<Car>>& cars) const {
        // Showroom chỉ quan tâm "đây là Car", không cần biết hãng cụ thể.
        for (const auto& car : cars) {
            std::cout << car->getDescription() << '\n';
        }
    }
};

int main() {
    std::vector<std::shared_ptr<Car>> cars;
    // Có thể thêm bao nhiêu hãng cũng được mà không sửa Showroom.
    cars.push_back(std::make_shared<Toyota>());
    cars.push_back(std::make_shared<Hyundai>());
    cars.push_back(std::make_shared<Mazda>());

    Showroom showroom;
    showroom.showCars(cars);
    return 0;
}
