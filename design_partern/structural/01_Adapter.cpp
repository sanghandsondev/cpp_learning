/*
Adapter Pattern (Mẫu bộ chuyển đổi)

Định nghĩa:
- Chuyển đổi interface của một lớp có sẵn (không tương thích)
  thành interface mà client mong muốn.
- Giúp tái sử dụng code cũ mà không cần sửa code cũ.

Khi nào dùng:
- Có thư viện cũ (legacy) nhưng hệ thống mới dùng interface khác.

Ví dụ này:
- Hệ thống mới cần interface IVehicleDiagnostic trả về chuỗi trạng thái.
- Thiết bị cũ LegacyOBDScanner chỉ trả về mã lỗi số nguyên.
- OBDAdapter bọc LegacyOBDScanner để chuyển mã số thành thông điệp dễ hiểu.
*/

#include <iostream>
#include <memory>
#include <string>

class IVehicleDiagnostic {
public:
    virtual ~IVehicleDiagnostic() = default;
    virtual std::string readStatus() const = 0;
};

class LegacyOBDScanner {
public:
    int getErrorCode() const {
        // Giả lập: 0 = OK, 101 = lỗi động cơ.
        return 101;
    }
};

class OBDAdapter : public IVehicleDiagnostic {
private:
    std::shared_ptr<LegacyOBDScanner> scanner;

public:
    explicit OBDAdapter(std::shared_ptr<LegacyOBDScanner> scanner)
        : scanner(std::move(scanner)) {}

    std::string readStatus() const override {
        int code = scanner->getErrorCode();
        if (code == 0) {
            return "Toyota: Hệ thống động cơ bình thường.";
        }
        if (code == 101) {
            return "Toyota: Cảnh báo lỗi động cơ (mã 101).";
        }
        return "Toyota: Lỗi chưa xác định (mã " + std::to_string(code) + ").";
    }
};

void printDiagnostic(const IVehicleDiagnostic& diagnostic) {
    std::cout << diagnostic.readStatus() << '\n';
}

int main() {
    std::shared_ptr<LegacyOBDScanner> legacyScanner =
        std::make_shared<LegacyOBDScanner>();

    OBDAdapter adapter(legacyScanner);
    printDiagnostic(adapter);

    return 0;
}
