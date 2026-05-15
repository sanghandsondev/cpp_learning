/**
 * ============================================================
 * BÀI 12: SUBARRAY SUM EQUALS K
 * ============================================================
 * 
 * LeetCode #560: Subarray Sum Equals K
 * 
 * ĐỀ BÀI:
 * Cho mảng nums và số k, đếm số mảng con liên tiếp có tổng = k.
 * 
 * VÍ DỤ:
 * Input:  nums = [1, 1, 1], k = 2
 * Output: 2  (mảng con [1,1] xuất hiện 2 lần: index 0-1 và 1-2)
 * 
 * Ý TƯỞNG (Prefix Sum + Hash Map):
 * - prefix[i] = nums[0] + nums[1] + ... + nums[i-1]
 * - Tổng mảng con từ i đến j = prefix[j+1] - prefix[i]
 * - Nếu prefix[j+1] - prefix[i] = k → prefix[i] = prefix[j+1] - k
 * - Dùng hash map đếm: có bao nhiêu prefix[i] = prefix[j+1] - k
 */

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// ============================================================
// Cách 1: Brute Force O(n²)
// ============================================================

int subarraySum_brute(vector<int>& nums, int k) {
    int count = 0;
    int n = nums.size();

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += nums[j];
            if (sum == k) count++;
        }
    }
    return count;
}

// ============================================================
// Cách 2: Prefix Sum + Hash Map O(n) ⭐
// ============================================================

int subarraySum_hashmap(vector<int>& nums, int k) {
    // prefixCount[sum] = số lần prefix sum = sum đã xuất hiện
    unordered_map<int, int> prefixCount;
    prefixCount[0] = 1; // prefix sum = 0 xuất hiện 1 lần (mảng rỗng)

    int count = 0;
    int currentSum = 0;

    for (int num : nums) {
        currentSum += num;

        // Nếu có prefix sum = currentSum - k → tồn tại mảng con tổng = k
        if (prefixCount.count(currentSum - k)) {
            count += prefixCount[currentSum - k];
        }

        // Lưu prefix sum hiện tại
        prefixCount[currentSum]++;
    }

    return count;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 12: SUBARRAY SUM EQUALS K" << endl;
    cout << "========================================\n" << endl;

    // Test case 1
    vector<int> nums1 = {1, 1, 1};
    int k1 = 2;
    cout << "--- Test 1: nums=[1,1,1], k=2 ---" << endl;
    cout << "  Brute force: " << subarraySum_brute(nums1, k1) << endl;
    cout << "  Hash map:    " << subarraySum_hashmap(nums1, k1) << endl;

    // Test case 2
    vector<int> nums2 = {1, 2, 3};
    int k2 = 3;
    cout << "\n--- Test 2: nums=[1,2,3], k=3 ---" << endl;
    cout << "  Kết quả: " << subarraySum_hashmap(nums2, k2) << endl;
    cout << "  (mảng con: [1,2] và [3])" << endl;

    // Test case 3: Có số âm
    vector<int> nums3 = {1, -1, 0};
    int k3 = 0;
    cout << "\n--- Test 3: nums=[1,-1,0], k=0 ---" << endl;
    cout << "  Kết quả: " << subarraySum_hashmap(nums3, k3) << endl;
    cout << "  (mảng con: [1,-1], [-1,0], [1,-1,0], [0])" << endl;

    // Giải thích từng bước
    cout << "\n========================================" << endl;
    cout << "  GIẢI THÍCH TỪNG BƯỚC" << endl;
    cout << "========================================" << endl;
    cout << R"(
  nums = [1, 2, 3], k = 3
  
  Prefix sum: [0, 1, 3, 6]
              (0 là prefix sum ban đầu)
  
  Mảng con tổng=3 khi: prefix[j] - prefix[i] = 3
  
  Bước qua từng phần tử:
  
  prefixCount = {0: 1}
  
  num=1: currentSum=1
         Tìm 1-3=-2 trong map? KHÔNG
         prefixCount = {0:1, 1:1}
  
  num=2: currentSum=3
         Tìm 3-3=0 trong map? CÓ! count += 1  (mảng con [1,2])
         prefixCount = {0:1, 1:1, 3:1}
  
  num=3: currentSum=6
         Tìm 6-3=3 trong map? CÓ! count += 1  (mảng con [3])
         prefixCount = {0:1, 1:1, 3:1, 6:1}
  
  → count = 2 ✓
  
  ⏱️ Thời gian: O(n)   (duyệt mảng 1 lần)
  💾 Bộ nhớ:   O(n)   (lưu prefix sum)
  
  💡 Pattern "Prefix Sum + Hash Map" rất hay gặp:
  - Subarray sum = k
  - Subarray sum divisible by k
  - Subarray with equal 0s and 1s
  - Contiguous array
)" << endl;

    return 0;
}
