#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

// 测试用的序列数据
string Reference = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA";

string Query = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA";

// 后缀自动机状态结构
struct State {
    int len;
    int link;
    map<char, int> next;
    bool terminal;
    int position;  // 终止状态代表的子串在原串中的结束位置
    // 构造函数代替初始化列表
    State(int l, int lnk, map<char, int> nxt, bool term, int pos) 
        : len(l), link(lnk), next(nxt), terminal(term), position(pos) {}
    State() : len(0), link(-1), next(), terminal(false), position(-1) {}
};

// 匹配信息结构
struct MatchInfo {
    int ref_start;   // 参考序列中起始位置
    int ref_end;     // 参考序列中结束位置
    int query_start; // 查询序列中起始位置
    int query_end;   // 查询序列中结束位置
    int length;      // 匹配长度
    bool is_reversed;// 是否反转
    int repeat_count;// 重复次数
    // 构造函数初始化
    MatchInfo() : ref_start(0), ref_end(0), query_start(0), query_end(0), 
                  length(0), is_reversed(false), repeat_count(0) {}
};

// 全局变量
vector<State> st;
int sz, last;

// 反转DNA序列（返回反向互补序列）
string reverseComplement(const string& dna) {
    string result = dna;
    reverse(result.begin(), result.end());
    // 替换范围for循环
    for (size_t i = 0; i < result.size(); ++i) {
        char& c = result[i];
        switch(c) {
            case 'A': c = 'T'; break;
            case 'T': c = 'A'; break;
            case 'G': c = 'C'; break;
            case 'C': c = 'G'; break;
            default: break; // 保留其他字符不变
        }
    }
    return result;
}

// 后缀自动机初始化
void sa_init() {
    st.clear();
    // 创建并添加初始状态
    map<char, int> empty_map;
    State initial_state(0, -1, empty_map, false, -1);
    st.push_back(initial_state);
    sz = 1;
    last = 0;
}

// 扩展后缀自动机
void sa_extend(char c, int pos) {
    int cur = sz++;
    // 创建新状态
    map<char, int> empty_map;
    st.push_back(State(st[last].len + 1, -1, empty_map, true, pos));
    
    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            // 创建clone状态
            State clone_state(st[p].len + 1, st[q].link, st[q].next, false, st[q].position);
            st.push_back(clone_state);
            st[q].link = st[cur].link = clone;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
        }
    }
    last = cur;
}

// 构建字符串的后缀自动机
void buildSAM(const string& s) {
    sa_init();
    for (int i = 0; i < (int)s.length(); i++) {
        sa_extend(s[i], i);
    }
}

// 在后缀自动机上匹配字符串，返回最长匹配长度和结束位置
pair<int, int> matchString(const string& pattern, int start_pos = 0) {
    int state = 0; // 从初始状态开始
    int matched_len = 0;
    int max_len = 0;
    int max_pos = -1;
    
    for (int i = 0; i < (int)pattern.length(); i++) {
        char c = pattern[i];
        // 尝试进行状态转移
        while (state != -1 && !st[state].next.count(c)) {
            state = st[state].link;
            if (state == -1) {
                matched_len = 0;
                break;
            }
            matched_len = st[state].len;
        }
        
        if (state == -1) {
            state = 0;  // 重置为初始状态
            matched_len = 0;
            continue;
        }
        
        state = st[state].next[c];
        matched_len++;
        
        // 更新最长匹配
        if (matched_len > max_len) {
            max_len = matched_len;
            max_pos = i;
        }
    }
    
    // 返回最长匹配长度和在pattern中的结束位置
    return make_pair(max_len, max_pos);
}

// 查找Query中的所有重复片段，并确定它们在Reference中的位置和是否反转
vector<MatchInfo> findRepeatedSegments(const string& ref, const string& query, int min_length = 10) {
    vector<MatchInfo> results;
    
    cout << "开始分析重复片段..." << endl;
    cout << "Reference长度: " << ref.length() << ", Query长度: " << query.length() << endl;
    cout << "最小匹配长度: " << min_length << endl;
    
    // 构建Reference的后缀自动机
    buildSAM(ref);
    cout << "构建Reference的后缀自动机完成，状态数: " << st.size() << endl;
    
    // 将后缀自动机状态保存起来，用于匹配原串
    vector<State> forward_sam = st;
    
    // 构建Reference反转后的后缀自动机
    string rev_ref = reverseComplement(ref);
    buildSAM(rev_ref);
    cout << "构建反转Reference的后缀自动机完成，状态数: " << st.size() << endl;
    
    // 保存反转后缀自动机状态
    vector<State> reversed_sam = st;
    
    // 记录匹配到的子串，用于判断重复
    map<string, vector<pair<int, int> > > forward_segments; // 正向匹配子串
    map<string, vector<pair<int, int> > > reversed_segments; // 反转匹配子串
    
    // 使用滑动窗口检测Query中的片段
    int window_step = 5; // 窗口步长，可以调整以加快处理速度
    cout << "开始扫描Query序列..." << endl;
    
    int found_count = 0;
    for (int i = 0; i < (int)query.length() - min_length + 1; i += window_step) {
        // 正向匹配
        st = forward_sam;
        pair<int, int> match = matchString(query.substr(i));
        int f_len = match.first;
        
        if (f_len >= min_length) {
            string matched_segment = query.substr(i, f_len);
            size_t ref_pos = ref.find(matched_segment);
            
            if (ref_pos != string::npos) {
                pair<int, int> pos_pair;
                pos_pair.first = i;
                pos_pair.second = i + f_len - 1;
                forward_segments[matched_segment].push_back(pos_pair);
                found_count++;
                
                // 当收集到足够的匹配段时，输出一些信息
                if (found_count % 100 == 0) {
                    cout << "已找到 " << found_count << " 个匹配片段" << endl;
                }
            }
        }
        
        // 反向匹配
        st = reversed_sam;
        match = matchString(query.substr(i));
        int r_len = match.first;
        
        if (r_len >= min_length) {
            string matched_segment = query.substr(i, r_len);
            string rev_matched = reverseComplement(matched_segment);
            size_t rev_ref_pos = ref.find(rev_matched);
            
            if (rev_ref_pos != string::npos) {
                pair<int, int> pos_pair;
                pos_pair.first = i;
                pos_pair.second = i + r_len - 1;
                reversed_segments[matched_segment].push_back(pos_pair);
            }
        }
    }
    
    cout << "扫描完成，开始统计结果..." << endl;
    cout << "正向匹配子串数量: " << forward_segments.size() << endl;
    cout << "反转匹配子串数量: " << reversed_segments.size() << endl;
    
    // 收集并返回结果
    // 处理正向匹配的重复子串
    for (map<string, vector<pair<int, int> > >::iterator it = forward_segments.begin(); 
         it != forward_segments.end(); ++it) {
         
        if (it->second.size() >= 2) {  // 至少重复2次
            MatchInfo info;
            info.is_reversed = false;
            info.repeat_count = it->second.size();
            info.length = it->first.length();
            
            // 取第一个出现位置在Reference中查找
            info.query_start = it->second[0].first;
            info.query_end = it->second[0].second;
            
            size_t ref_pos = ref.find(it->first);
            if (ref_pos != string::npos) {
                info.ref_start = ref_pos;
                info.ref_end = ref_pos + info.length - 1;
                results.push_back(info);
            }
        }
    }
    
    // 处理反向匹配的重复子串
    for (map<string, vector<pair<int, int> > >::iterator it = reversed_segments.begin(); 
         it != reversed_segments.end(); ++it) {
         
        if (it->second.size() >= 2) {  // 至少重复2次
            MatchInfo info;
            info.is_reversed = true;
            info.repeat_count = it->second.size();
            info.length = it->first.length();
            
            // 取第一个出现位置
            info.query_start = it->second[0].first;
            info.query_end = it->second[0].second;
            
            string rev_matched = reverseComplement(it->first);
            size_t ref_pos = ref.find(rev_matched);
            if (ref_pos != string::npos) {
                info.ref_start = ref_pos;
                info.ref_end = ref_pos + info.length - 1;
                results.push_back(info);
            }
        }
    }
    
    cout << "总共找到 " << results.size() << " 个重复子串" << endl;
    return results;
}

// 输出结果
void printResults(const vector<MatchInfo>& results) {
    if (results.empty()) {
        cout << "未找到满足条件的重复子串！" << endl;
        return;
    }
    
    cout << "在Query中发现的重复子串：" << endl;
    cout << "-------------------------" << endl;
    
    for (size_t i = 0; i < results.size(); ++i) {
        const MatchInfo& info = results[i];
        cout << "重复子串 #" << (i+1) << ":" << endl;
        cout << "在Reference中的位置: [" << info.ref_start << ", " << info.ref_end << "]" << endl;
        cout << "在Query中的位置: [" << info.query_start << ", " << info.query_end << "]" << endl;
        cout << "重复长度: " << info.length << endl;
        cout << "重复次数: " << info.repeat_count << endl;
        cout << "是否反转: " << (info.is_reversed ? "是" : "否") << endl;
        cout << "-------------------------" << endl;
    }
}

int main() {
    cout << "程序开始执行..." << endl;
    
    // 尝试较低的匹配长度阈值，以增加找到匹配的可能性
    vector<MatchInfo> results = findRepeatedSegments(Reference, Query, 8);
    
    // 输出结果
    printResults(results);
    
    cout << "程序执行完毕" << endl;
    return 0;
}
