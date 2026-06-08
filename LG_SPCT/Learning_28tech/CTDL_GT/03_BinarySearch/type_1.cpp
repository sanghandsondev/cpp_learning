#include <iostream>
using namespace std;

// ĐIỀU KIỆN: Dãy tăng dần (không giảm)
// Điều kiện break vòng while là l > r, tức l nằm bên phải r ---> dựa vào đó để return giá trị min, max tùy yêu cầu bài toán.

// Type 1: Các phần tử trong mảng là không giống nhau, và đc sắp xếp tăng dần
// Type 1.1: Tìm đúng index phần tử có trong mảng
int binarySearch1(int arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] == x) {
            return m;
        } else if (arr[m] < x) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return -1;
}
// Type 1.2: Tìm index phần tử lớn nhất thỏa mãn điều kiện biểu thức EX : true true true <true> false false false ...
int binarySearch2(int arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] <= x) {      // giả sử điều kiện biểu thức EX: thỏa mãn <= giá trị x   ---->  check(m) == true  
            l = m + 1;                                                                  // --->  left = mid + 1
        } else {
            r = m - 1;
        }
    }
    return r;
}
// Type 1.3: Tìm index phần tử nhỏ nhất thỏa mãn điều kiện biểu thức EX : false false false false <true> true true ...
int binarySearch3(int arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] >= x) {      // giả sử điều kiện biểu thức EX: thỏa mãn >= giá trị x  ----> check(m) == true
            r = m - 1;                                                              // -----> right = mid - 1
        } else {
            l = m + 1;        
        }
    }
    return l;
}

int main(){
    // --- Yêu cầu mảng tăng dần (không giảm) ----
    int a[6] = {1, 3, 4, 5, 7, 9};      
    cout << binarySearch1(a, 6, 7) << endl;     // 1 3 4 5 (7) 9 => index = 4     == 7

    int b[11] = {1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 17};
    cout << binarySearch2(b, 11, 10) << endl;   // 1 2 4 5 7 8 (9) 11 => index = 6      <= 10

    int c[11] = {1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 17};
    cout << binarySearch3(b, 11, 13) << endl;   // 1 2 4 5 7 8 9 11 12 (14) => index = 8    >= 13
    return 0;
}