/*
Observer Pattern (Mẫu quan sát)

Định nghĩa:
- Thiết lập quan hệ một-nhiều giữa Subject và các Observer.
- Khi Subject thay đổi trạng thái, toàn bộ Observer được thông báo tự động.

Khi nào dùng:
- Có dữ liệu trung tâm thay đổi liên tục (nhiệt độ động cơ, mức pin...)
  và nhiều màn hình/hệ thống cần cập nhật.

Ví dụ này:
- EngineSensor là Subject, lưu nhiệt độ động cơ.
- DashboardDisplay và MobileAppDisplay là Observer.
- Khi nhiệt độ đổi, các observer nhận thông báo ngay.
*/

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(int engineTemp) = 0;
};

class EngineSensor {
private:
    std::vector<std::shared_ptr<IObserver>> observers;
    int temperature = 0;

public:
    void attach(const std::shared_ptr<IObserver>& observer) {
        observers.push_back(observer);
    }

    void detach(const std::shared_ptr<IObserver>& observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end());
    }

    void setTemperature(int newTemp) {
        temperature = newTemp;
        notify();
    }

private:
    void notify() {
        for (const auto& observer : observers) {
            observer->update(temperature);
        }
    }
};

class DashboardDisplay : public IObserver {
public:
    void update(int engineTemp) override {
        std::cout << "[Dashboard] Nhiệt độ động cơ: " << engineTemp << "°C\n";
    }
};

class MobileAppDisplay : public IObserver {
public:
    void update(int engineTemp) override {
        std::cout << "[Mobile App] Cập nhật nhiệt độ: " << engineTemp << "°C\n";
    }
};

int main() {
    EngineSensor sensor;

    std::shared_ptr<IObserver> dashboard = std::make_shared<DashboardDisplay>();
    std::shared_ptr<IObserver> mobileApp = std::make_shared<MobileAppDisplay>();

    sensor.attach(dashboard);
    sensor.attach(mobileApp);

    sensor.setTemperature(90);
    sensor.setTemperature(105);

    // Gỡ app mobile, chỉ dashboard tiếp tục nhận thông báo.
    sensor.detach(mobileApp);
    sensor.setTemperature(98);

    return 0;
}
