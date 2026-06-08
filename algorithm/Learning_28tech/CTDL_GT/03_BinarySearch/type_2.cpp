#include <iostream>
using namespace std;

// ĐIỀU KIỆN: Dãy tăng dần (không giảm)
// Điều kiện break vòng while là l > r, tức l nằm bên phải r ---> dựa vào đó để return giá trị min, max tùy yêu cầu bài toán.

// Type 2: Các phần tử trong mảng nơi tìm kiếm là giống nhau, và đc sắp xếp tăng dần
// Ví dụ: 1 2 3 4 5 5 5 5 5 5 6 7 8 9   

// Type 2.1: Tìm số 5 bên trái cùng (first to come) ---> lower_bound()
int binarySearch(int arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] < x) {           // Điều kiện biểu thức chắc chắn sẽ là >= x
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return l;
}
// Type 2.2: Tìm số 5 bên phải cùng (last to come) ----> upper_bound() - 1
int binarySearch2(int arr[], int size, int x) {
    int l = 0, r = size - 1;
    while (l <= r)
    {
        int m = l + (r-l)/2;
        if (arr[m] <= x) {           // Điều kiện biểu thức chắc chắn sẽ là <= x
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return r;
}

int main(){
    int a[13] = {1, 3, 4, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9};      
    cout << binarySearch(a, 13, 5) << endl;     // 1 3 4 (5) 5 5 ... => index = 3       >= 5
    cout << binarySearch(a, 13, 6) << endl;     // index = 9                            >= 6

    cout << binarySearch2(a, 13, 5) << endl;     // 1 3 4 5 5 5 5 5 (5) 6 ... => index = 8       <= 5

    return 0;
}