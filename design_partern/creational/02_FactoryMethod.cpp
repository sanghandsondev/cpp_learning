/*
Factory Method Pattern (Mẫu phương thức nhà máy)

Định nghĩa:
- Định nghĩa interface để tạo object, nhưng để lớp con quyết định tạo object cụ thể nào.
- Tách phần tạo đối tượng ra khỏi phần sử dụng đối tượng.

Khi nào dùng:
- Muốn mở rộng thêm loại object mới mà không sửa code client cũ.
- Giảm phụ thuộc trực tiếp vào class cụ thể.

Ví dụ này:
- CarFactory là interface tạo xe.
- ToyotaFactory/HyundaiFactory/MazdaFactory tạo từng loại xe cụ thể.
- Hàm client chỉ làm việc với CarFactory và Car.
*/

#include <iostream>
#include <memory>
#include <string>

class Car {
public:
    virtual ~Car() = default;
    virtual std::string getModel() const = 0;
};

class ToyotaCar : public Car {
public:
    std::string getModel() const override { return "Toyota Corolla"; }
};

class HyundaiCar : public Car {
public:
    std::string getModel() const override { return "Hyundai Elantra"; }
};

class MazdaCar : public Car {
public:
    std::string getModel() const override { return "Mazda 3"; }
};

class CarFactory {
public:
    virtual ~CarFactory() = default;
    virtual std::unique_ptr<Car> createCar() const = 0;
};

class ToyotaFactory : public CarFactory {
public:
    std::unique_ptr<Car> createCar() const override {
        return std::make_unique<ToyotaCar>();
    }
};

class HyundaiFactory : public CarFactory {
public:
    std::unique_ptr<Car> createCar() const override {
        return std::make_unique<HyundaiCar>();
    }
};

class MazdaFactory : public CarFactory {
public:
    std::unique_ptr<Car> createCar() const override {
        return std::make_unique<MazdaCar>();
    }
};

void deliverCar(const CarFactory& factory) {
    // Client không cần biết đang new loại xe nào.
    std::unique_ptr<Car> car = factory.createCar();
    std::cout << "Giao xe: " << car->getModel() << '\n';
}

int main() {
    ToyotaFactory toyotaFactory;
    HyundaiFactory hyundaiFactory;
    MazdaFactory mazdaFactory;

    deliverCar(toyotaFactory);
    deliverCar(hyundaiFactory);
    deliverCar(mazdaFactory);

    return 0;
}
