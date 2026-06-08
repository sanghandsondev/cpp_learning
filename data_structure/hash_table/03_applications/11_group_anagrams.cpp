/**
 * ============================================================
 * BÀI 11: GROUP ANAGRAMS
 * ============================================================
 * 
 * LeetCode #49: Group Anagrams
 * 
 * ĐỀ BÀI:
 * Cho mảng string, nhóm các anagram lại với nhau.
 * Anagram = các chuỗi có cùng ký tự (khác thứ tự).
 * 
 * VÍ DỤ:
 * Input:  ["eat","tea","tan","ate","nat","bat"]
 * Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
 * 
 * Ý TƯỞNG:
 * - Sort mỗi string → anagram cho cùng kết quả
 * - Dùng string đã sort làm KEY trong hash map
 * - "eat" sort → "aet", "tea" sort → "aet" → cùng group!
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

// ============================================================
// Cách 1: Sort string làm key
// ============================================================
// Thời gian: O(n * k*log(k)) với n = số string, k = độ dài trung bình

vector<vector<string>> groupAnagrams_sort(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;

    for (const string& s : strs) {
        string key = s;
        sort(key.begin(), key.end()); // "eat" → "aet"
        groups[key].push_back(s);
    }

    vector<vector<string>> result;
    for (auto& [key, group] : groups) {
        result.push_back(group);
    }
    return result;
}

// ============================================================
// Cách 2: Frequency string làm key (nhanh hơn)
// ============================================================
// Thay vì sort → đếm tần suất ký tự → tạo key từ freq
// Thời gian: O(n * k) — nhanh hơn cách 1!

vector<vector<string>> groupAnagrams_freq(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;

    for (const string& s : strs) {
        // Đếm tần suất 26 ký tự
        int freq[26] = {0};
        for (char c : s) freq[c - 'a']++;

        // Tạo key: "1#0#0#...#0#1#0#..." (freq mỗi ký tự)
        string key = "";
        for (int i = 0; i < 26; i++) {
            key += to_string(freq[i]) + "#";
        }
        // "eat" → "1#0#0#0#1#0#0#0#0#0#0#0#0#0#0#0#0#0#0#1#0#0#0#0#0#0#"
        // "tea" → "1#0#0#0#1#0#0#0#0#0#0#0#0#0#0#0#0#0#0#1#0#0#0#0#0#0#"
        // → CÙNG KEY! ✓

        groups[key].push_back(s);
    }

    vector<vector<string>> result;
    for (auto& [key, group] : groups) {
        result.push_back(group);
    }
    return result;
}

void printGroups(const vector<vector<string>>& groups) {
    cout << "  [" << endl;
    for (const auto& group : groups) {
        cout << "    [";
        for (int i = 0; i < (int)group.size(); i++) {
            if (i > 0) cout << ", ";
            cout << "\"" << group[i] << "\"";
        }
        cout << "]" << endl;
    }
    cout << "  ]" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "  BÀI 11: GROUP ANAGRAMS" << endl;
    cout << "========================================\n" << endl;

    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};

    cout << "  Input: [\"eat\",\"tea\",\"tan\",\"ate\",\"nat\",\"bat\"]\n" << endl;

    cout << "--- Cách 1: Sort key ---" << endl;
    auto result1 = groupAnagrams_sort(strs);
    printGroups(result1);

    cout << "\n--- Cách 2: Frequency key ---" << endl;
    auto result2 = groupAnagrams_freq(strs);
    printGroups(result2);

    // Giải thích
    cout << "\n--- Quá trình (Cách 1) ---" << endl;
    cout << R"(
  "eat" → sort → "aet" → groups["aet"] = ["eat"]
  "tea" → sort → "aet" → groups["aet"] = ["eat", "tea"]
  "tan" → sort → "ant" → groups["ant"] = ["tan"]
  "ate" → sort → "aet" → groups["aet"] = ["eat", "tea", "ate"]
  "nat" → sort → "ant" → groups["ant"] = ["tan", "nat"]
  "bat" → sort → "abt" → groups["abt"] = ["bat"]
  
  Hash map cuối:
  "aet" → ["eat", "tea", "ate"]
  "ant" → ["tan", "nat"]
  "abt" → ["bat"]
)" << endl;

    // Test case 2
    cout << "--- Test case 2 ---" << endl;
    vector<string> strs2 = {"", ""};
    auto result3 = groupAnagrams_sort(strs2);
    cout << "  Input: [\"\", \"\"]" << endl;
    printGroups(result3);

    cout << R"(
  💡 Bài này minh họa pattern:
     "Tạo key đại diện cho nhóm → dùng hash map nhóm lại"
     
  Ứng dụng tương tự:
  - Nhóm số có cùng tổng chữ số
  - Nhóm string có cùng pattern (encode pattern)
  - Nhóm theo bất kỳ tiêu chí nào!
)" << endl;

    return 0;
}
