/**
 * ============================================================
 * BÀI 9: TWO SUM - Bài toán kinh điển nhất dùng Hash Map
 * ============================================================
 * 
 * ĐỀ BÀI (LeetCode #1):
 * Cho mảng nums và số target.
 * Tìm 2 phần tử có tổng = target. Trả về index của chúng.
 * 
 * VÍ DỤ:
 * Input:  nums = [2, 7, 11, 15], target = 9
 * Output: [0, 1]  (vì nums[0] + nums[1] = 2 + 7 = 9)
 */

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// ============================================================
// Cách 1: Brute Force - O(n²)
// ============================================================
vector<int> twoSum_bruteforce(vector<int>& nums, int target) {
    for (int i = 0; i < (int)nums.size(); i++) {
        for (int j = i + 1; j < (int)nums.size(); j++) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }
    return {};
}

// ============================================================
// Cách 2: Hash Map - O(n) ⭐ (ĐÂY LÀ CÁCH TỐI ƯU)
// ============================================================
// 
// Ý TƯỞNG:
// Với mỗi số nums[i], ta cần tìm (target - nums[i]) đã xuất hiện chưa.
// Dùng hash map lưu: {giá trị → index}
// 
// VÍ DỤ: nums = [2, 7, 11, 15], target = 9
//   i=0: nums[0]=2, cần tìm 9-2=7. Map rỗng → chưa có. Lưu {2→0}
//   i=1: nums[1]=7, cần tìm 9-7=2. Map có 2→0 → TÌM THẤY! Return {0,1}

vector<int> twoSum_hashmap(vector<int>& nums, int target) {
    unordered_map<int, int> seen; // {value → index}

    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];

        // Kiểm tra complement đã xuất hiện chưa
        if (seen.count(complement)) {
            return {seen[complement], i};
        }

        // Lưu số hiện tại vào map
        seen[nums[i]] = i;
    }
    return {};
}

// ============================================================
// Biến thể: Two Sum trả về true/false (dùng unordered_set)
// ============================================================
bool twoSum_exists(vector<int>& nums, int target) {
    unordered_set<int> seen;

    for (int num : nums) {
        if (seen.count(target - num)) {
            return true;
        }
        seen.insert(num);
    }
    return false;
}

// ============================================================
// Biến thể: Đếm số cặp có tổng = target
// ============================================================
int countPairs(vector<int>& nums, int target) {
    unordered_map<int, int> freq;
    int count = 0;

    for (int num : nums) {
        int complement = target - num;
        if (freq.count(complement)) {
            count += freq[complement];
        }
        freq[num]++;
    }
    return count;
}

void printResult(const vector<int>& result) {
    if (result.empty()) {
        cout << "Không tìm thấy";
    } else {
        cout << "[" << result[0] << ", " << result[1] << "]";
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 9: TWO SUM" << endl;
    cout << "========================================\n" << endl;

    // Test case 1
    vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;

    cout << "  nums = [2, 7, 11, 15], target = 9" << endl;
    cout << "  Brute force: ";
    printResult(twoSum_bruteforce(nums1, target1));
    cout << endl;
    cout << "  Hash map:    ";
    printResult(twoSum_hashmap(nums1, target1));
    cout << endl;

    // Test case 2
    vector<int> nums2 = {3, 2, 4};
    int target2 = 6;

    cout << "\n  nums = [3, 2, 4], target = 6" << endl;
    cout << "  Hash map: ";
    printResult(twoSum_hashmap(nums2, target2));
    cout << endl;

    // Test case 3: Đếm cặp
    vector<int> nums3 = {1, 5, 7, -1, 5};
    int target3 = 6;
    cout << "\n  nums = [1, 5, 7, -1, 5], target = 6" << endl;
    cout << "  Số cặp có tổng = 6: " << countPairs(nums3, target3) << endl;

    // Giải thích chi tiết
    cout << "\n\n========================================" << endl;
    cout << "  GIẢI THÍCH TỪNG BƯỚC" << endl;
    cout << "========================================" << endl;
    cout << R"(
  nums = [2, 7, 11, 15], target = 9
  
  Map: {}
  
  i=0: num=2, complement=9-2=7
       Map có 7? KHÔNG → Lưu {2:0}
       Map: {2:0}
  
  i=1: num=7, complement=9-7=2  
       Map có 2? CÓ! → index của 2 là 0
       → Return [0, 1] ✓
  
  Độ phức tạp:
  - Thời gian: O(n) — duyệt mảng 1 lần
  - Bộ nhớ:    O(n) — lưu tối đa n phần tử trong map
  
  So sánh:
  - Brute force: O(n²) thời gian, O(1) bộ nhớ
  - Hash map:    O(n) thời gian,  O(n) bộ nhớ
  → Đánh đổi bộ nhớ lấy tốc độ! (Space-Time tradeoff)
)" << endl;

    return 0;
}
