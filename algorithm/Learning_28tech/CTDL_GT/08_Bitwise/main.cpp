#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// "Bitwise" nghĩa là làm việc trực tiếp trên từng bit của số nguyên, dùng các toán tử: 
// & (AND): Cả 2 đều là 1 -> 1, còn lại -> 0
// | (OR): Cả 2 đều là 0 -> 0, còn lại -> 1
// ^ (XOR): 0 vs 1 -> 1, còn lại -> 0 (tức bit khác nhau -> 1, giống nhau -> 0)
// ~ (NOT): Đảo bit ~X
// << (dịch trái): X << 3 -> Dịch bit của X sang trái 3 nhịp
// >> (dịch phải): X >> 3 -> Dịch bit của X sang phải 3 nhịp
// __builtin_popcount(x): Đếm bit '1' trong số nguyên x

// Bitmask là một giá trị nhị phân (chuỗi bit) dùng để bật, tắt, hoặc kiểm tra những bit cụ thể trong một giá trị khác,
//          thường được kết hợp với toán tử bitwise.
int main(){
    unsigned int a, b, c, t;
    a = 13;                 // 1101
    b = 8;                  // 8 = 2^3 == 1000 == (1 << 3) == (0001 << 3)
    c = 7;                  // 0111
    t = a & b;         // 1000 -> Đây cũng là cách để kiểm tra xem bit thứ 3 của 'a' có được bật hay không (if (a & b))
    t = b | c;         // 1111
    t = a ^ c;         // 1010
    t = a << 2;     // 110100
    t = b >> 2;     // 10 == (1000 >> 2)
    int no = __builtin_popcount(a ^ c);     // 2

    // -- Input 1: (cho biết các bit position được bật, bitPos bắt đầu từ 0 -> ..)
    // 4
    // 1 3 4 5
    int n;
    cin >> n;
    int mask = 0;
    for (int i = 0; i < n; i++) {
        int bitPos;
        cin >> bitPos;
        mask |= (1 << bitPos);      // bật bit ở vị trí bitPos
    }

    cout << mask << endl;           // in giá trị int của bitmask: 00111010 = 58 
                                    // (được bật các bit thứ 1, 3, 4, 5 từ phải sang trái, bắt đầu từ bit thứ 0)

    // -- Input 2: (cho luông mảng nhị phân)
    vector<int> bits = {0, 0, 1, 0, 0, 1, 1};   // MSB-first (Most Significant Bit trước) -> '0010011'
    int m = bits.size();
    mask = 0;
    for (int i = 0; i < m; i++) {
        if (bits[i] == 1) 
            mask |= (1 << (m - 1 - i));       // Cách 1: Dễ hiểu hơn
                   
    }
    mask = 0;
    for (int i = 0; i < m; i++) {
        mask = (mask << 1) | bits[i];         // Cách 2: Advance -> rất hay
    }


    return 0;
}