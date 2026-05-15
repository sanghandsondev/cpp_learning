/**
 * ============================================================
 * BÀI 10: FREQUENCY COUNT - Đếm tần suất phần tử
 * ============================================================
 * 
 * Đây là PATTERN phổ biến nhất khi dùng hash map!
 * Gặp rất nhiều trong thuật toán và phỏng vấn.
 * 
 * Pattern: for (auto x : arr) mp[x]++;
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

// ============================================================
// Bài 1: Tìm phần tử xuất hiện nhiều nhất (Majority Element)
// ============================================================
// LeetCode #169: Tìm phần tử xuất hiện > n/2 lần

int majorityElement(vector<int>& nums) {
    unordered_map<int, int> freq;
    int n = nums.size();

    for (int num : nums) {
        freq[num]++;
        if (freq[num] > n / 2) return num;
    }
    return -1;
}

// ============================================================
// Bài 2: Tìm phần tử đầu tiên không lặp lại
// ============================================================
// LeetCode #387: First Unique Character in a String

int firstUniqChar(string s) {
    unordered_map<char, int> freq;

    // Bước 1: Đếm tần suất
    for (char c : s) freq[c]++;

    // Bước 2: Tìm ký tự đầu tiên có freq = 1
    for (int i = 0; i < (int)s.size(); i++) {
        if (freq[s[i]] == 1) return i;
    }
    return -1;
}

// ============================================================
// Bài 3: Kiểm tra 2 chuỗi có phải anagram không
// ============================================================
// LeetCode #242: Valid Anagram
// "anagram" và "nagaram" → true (cùng tần suất ký tự)

bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;

    unordered_map<char, int> freq;

    // Đếm freq cho s (tăng) và t (giảm)
    for (int i = 0; i < (int)s.size(); i++) {
        freq[s[i]]++;
        freq[t[i]]--;
    }

    // Nếu tất cả freq = 0 → anagram
    for (auto& [ch, count] : freq) {
        if (count != 0) return false;
    }
    return true;
}

// ============================================================
// Bài 4: Top K phần tử xuất hiện nhiều nhất
// ============================================================
// LeetCode #347: Top K Frequent Elements

vector<int> topKFrequent(vector<int>& nums, int k) {
    // Bước 1: Đếm tần suất
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;

    // Bước 2: Bucket sort theo tần suất
    // bucket[i] = danh sách các số xuất hiện i lần
    int n = nums.size();
    vector<vector<int>> bucket(n + 1);
    for (auto& [num, count] : freq) {
        bucket[count].push_back(num);
    }

    // Bước 3: Lấy từ bucket lớn nhất
    vector<int> result;
    for (int i = n; i >= 0 && (int)result.size() < k; i--) {
        for (int num : bucket[i]) {
            result.push_back(num);
            if ((int)result.size() == k) break;
        }
    }
    return result;
}

// ============================================================
// Bài 5: Kiểm tra 2 mảng có "tương đương" không
// ============================================================
// Cùng phần tử, cùng tần suất (không cần cùng thứ tự)

bool areEquivalent(vector<int>& a, vector<int>& b) {
    if (a.size() != b.size()) return false;

    unordered_map<int, int> freq;
    for (int x : a) freq[x]++;
    for (int x : b) freq[x]--;

    for (auto& [key, count] : freq) {
        if (count != 0) return false;
    }
    return true;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 10: FREQUENCY COUNT" << endl;
    cout << "========================================\n" << endl;

    // Bài 1
    cout << "--- 1. Majority Element ---" << endl;
    vector<int> nums1 = {2, 2, 1, 1, 1, 2, 2};
    cout << "  [2,2,1,1,1,2,2] → Majority: " << majorityElement(nums1) << "\n" << endl;

    // Bài 2
    cout << "--- 2. First Unique Character ---" << endl;
    string s1 = "leetcode";
    string s2 = "aabb";
    cout << "  \"leetcode\" → index: " << firstUniqChar(s1) 
         << " (ký tự '" << s1[firstUniqChar(s1)] << "')" << endl;
    cout << "  \"aabb\"     → index: " << firstUniqChar(s2) << " (không có)\n" << endl;

    // Bài 3
    cout << "--- 3. Valid Anagram ---" << endl;
    cout << "  \"anagram\" vs \"nagaram\": " 
         << (isAnagram("anagram", "nagaram") ? "true" : "false") << endl;
    cout << "  \"rat\" vs \"car\": " 
         << (isAnagram("rat", "car") ? "true" : "false") << "\n" << endl;

    // Bài 4
    cout << "--- 4. Top K Frequent Elements ---" << endl;
    vector<int> nums4 = {1, 1, 1, 2, 2, 3};
    auto topK = topKFrequent(nums4, 2);
    cout << "  [1,1,1,2,2,3], k=2 → Top 2: ";
    for (int x : topK) cout << x << " ";
    cout << "\n" << endl;

    // Bài 5
    cout << "--- 5. Equivalent Arrays ---" << endl;
    vector<int> a = {1, 2, 3, 2};
    vector<int> b = {2, 1, 2, 3};
    vector<int> c = {1, 2, 2, 4};
    cout << "  [1,2,3,2] vs [2,1,2,3]: " 
         << (areEquivalent(a, b) ? "true" : "false") << endl;
    cout << "  [1,2,3,2] vs [1,2,2,4]: " 
         << (areEquivalent(a, c) ? "true" : "false") << endl;

    // Pattern tổng kết
    cout << "\n========================================" << endl;
    cout << "  PATTERN ĐẾMMẦN SUẤT" << endl;
    cout << "========================================" << endl;
    cout << R"(
  // Pattern cơ bản
  unordered_map<T, int> freq;
  for (auto x : data) freq[x]++;
  
  // Ứng dụng:
  // 1. Tìm majority element  → freq[x] > n/2
  // 2. Tìm unique element    → freq[x] == 1
  // 3. Kiểm tra anagram      → freq bằng nhau
  // 4. Top K frequent         → sort theo freq
  // 5. So sánh 2 collection  → freq++ rồi freq--
  
  ⏱️ Tất cả đều O(n) nhờ hash map!
)" << endl;

    return 0;
}
