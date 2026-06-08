/**
 * ============================================================
 * BÀI 13: LONGEST CONSECUTIVE SEQUENCE
 * ============================================================
 * 
 * LeetCode #128: Longest Consecutive Sequence
 * 
 * ĐỀ BÀI:
 * Cho mảng nums (không sắp xếp), tìm độ dài dãy số liên tiếp dài nhất.
 * Yêu cầu: O(n)
 * 
 * VÍ DỤ:
 * Input:  [100, 4, 200, 1, 3, 2]
 * Output: 4  (dãy liên tiếp: 1, 2, 3, 4)
 * 
 * Ý TƯỞNG:
 * 1. Bỏ tất cả số vào unordered_set
 * 2. Với mỗi số n, nếu (n-1) KHÔNG có trong set → n là đầu dãy
 * 3. Từ đầu dãy, đếm n+1, n+2, n+3... còn trong set bao lâu
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

// ============================================================
// Cách 1: Sort O(n log n)
// ============================================================

int longestConsecutive_sort(vector<int> nums) {
    if (nums.empty()) return 0;

    sort(nums.begin(), nums.end());

    int longest = 1, current = 1;
    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] == nums[i-1]) continue;       // Bỏ qua trùng lặp
        if (nums[i] == nums[i-1] + 1) {
            current++;
        } else {
            current = 1;
        }
        longest = max(longest, current);
    }
    return longest;
}

// ============================================================
// Cách 2: Hash Set O(n) ⭐
// ============================================================

int longestConsecutive_hashset(vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());

    int longest = 0;

    for (int num : numSet) {
        // Chỉ bắt đầu đếm nếu num là ĐẦU DÃY
        // (tức num-1 không tồn tại)
        if (!numSet.count(num - 1)) {
            int currentNum = num;
            int currentStreak = 1;

            // Đếm tiếp
            while (numSet.count(currentNum + 1)) {
                currentNum++;
                currentStreak++;
            }

            longest = max(longest, currentStreak);
        }
    }

    return longest;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 13: LONGEST CONSECUTIVE SEQUENCE" << endl;
    cout << "========================================\n" << endl;

    // Test 1
    vector<int> nums1 = {100, 4, 200, 1, 3, 2};
    cout << "--- Test 1: [100, 4, 200, 1, 3, 2] ---" << endl;
    cout << "  Sort:     " << longestConsecutive_sort(nums1) << endl;
    cout << "  Hash set: " << longestConsecutive_hashset(nums1) << endl;
    cout << "  (Dãy: 1→2→3→4, dài 4)\n" << endl;

    // Test 2
    vector<int> nums2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    cout << "--- Test 2: [0,3,7,2,5,8,4,6,0,1] ---" << endl;
    cout << "  Hash set: " << longestConsecutive_hashset(nums2) << endl;
    cout << "  (Dãy: 0→1→2→3→4→5→6→7→8, dài 9)\n" << endl;

    // Test 3
    vector<int> nums3 = {9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6};
    cout << "--- Test 3: [9,1,4,7,3,-1,0,5,8,-1,6] ---" << endl;
    cout << "  Hash set: " << longestConsecutive_hashset(nums3) << endl;
    cout << "  (Dãy: -1→0→1→...→9, dài 11)\n" << endl;

    // Giải thích
    cout << "========================================" << endl;
    cout << "  GIẢI THÍCH" << endl;
    cout << "========================================" << endl;
    cout << R"(
  nums = [100, 4, 200, 1, 3, 2]
  Set  = {100, 4, 200, 1, 3, 2}
  
  Duyệt từng số trong set:
  
  num=100: 99 có trong set? KHÔNG → 100 là đầu dãy
           101 có? KHÔNG → streak = 1
  
  num=4:   3 có trong set? CÓ → 4 KHÔNG phải đầu dãy → BỎ QUA
  
  num=200: 199 có? KHÔNG → đầu dãy → 201 có? KHÔNG → streak = 1
  
  num=1:   0 có? KHÔNG → 1 là đầu dãy! ⭐
           2 có? CÓ → streak = 2
           3 có? CÓ → streak = 3
           4 có? CÓ → streak = 4
           5 có? KHÔNG → streak = 4
  
  num=3:   2 có? CÓ → BỎ QUA
  num=2:   1 có? CÓ → BỎ QUA
  
  → longest = 4 ✓
  
  ⏱️ TẠI SAO LÀ O(n)?
  Mỗi số chỉ được "đếm" 1 lần (trong while loop).
  Vì chỉ bắt đầu đếm từ đầu dãy → mỗi số thuộc đúng 1 dãy.
  Tổng cộng: O(n) cho tất cả while loops.
  
  💡 Đây là bài mẫu tuyệt vời cho Hash Set:
  - Cần kiểm tra sự tồn tại O(1) → dùng set
  - Trick: Chỉ xử lý "đầu dãy" để tránh lặp
)" << endl;

    return 0;
}
