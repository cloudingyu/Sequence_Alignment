#include "sequenceProcess.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Anchor {
    int query_start, query_end;
    int ref_start, ref_end;
    int score;
    
    Anchor(int qs, int qe, int rs, int re, int s) : 
        query_start(qs), query_end(qe), ref_start(rs), ref_end(re), score(s) {}
};

struct GraphNode {
    int anchor_id;
    int max_score;
    int prev_id;
    
    GraphNode(int id) : anchor_id(id), max_score(0), prev_id(-1) {}
};

// 计算字符串的滚动哈希值
uint64_t rollingHash(const string& str, int start, int len) {
    const uint64_t base = 257;
    const uint64_t mod = 1000000007;
    uint64_t hash_val = 0;
    uint64_t power = 1;
    
    for (int i = 0; i < len && start + i < str.length(); i++) {
        hash_val = (hash_val + (str[start + i] - 'A' + 1) * power) % mod;
        power = (power * base) % mod;
    }
    return hash_val;
}

// 查找锚点（完全匹配的片段）
vector<Anchor> findAnchors(const string& reference, const string& query, int min_length = 15) {
    vector<Anchor> anchors;
    unordered_map<uint64_t, vector<int>> ref_hash_map;
    
    // 为reference创建哈希映射
    for (int i = 0; i <= (int)reference.length() - min_length; i++) {
        uint64_t hash_val = rollingHash(reference, i, min_length);
        ref_hash_map[hash_val].push_back(i);
    }
    
    // 在query中查找匹配
    for (int i = 0; i <= (int)query.length() - min_length; i++) {
        uint64_t hash_val = rollingHash(query, i, min_length);
        
        if (ref_hash_map.find(hash_val) != ref_hash_map.end()) {
            for (int ref_pos : ref_hash_map[hash_val]) {
                // 验证哈希匹配并扩展
                int len = 0;
                while (i + len < query.length() && 
                       ref_pos + len < reference.length() &&
                       query[i + len] == reference[ref_pos + len]) {
                    len++;
                }
                
                if (len >= min_length) {
                    anchors.push_back(Anchor(i, i + len - 1, ref_pos, ref_pos + len - 1, len));
                }
            }
        }
    }
    
    // 处理反向互补匹配
    string rev_comp_query = query;
    for (char& c : rev_comp_query) {
        switch (c) {
            case 'A': c = 'T'; break;
            case 'T': c = 'A'; break;
            case 'C': c = 'G'; break;
            case 'G': c = 'C'; break;
        }
    }
    reverse(rev_comp_query.begin(), rev_comp_query.end());
    
    // 为反向互补序列创建哈希映射
    unordered_map<uint64_t, vector<int>> rev_hash_map;
    for (int i = 0; i <= (int)rev_comp_query.length() - min_length; i++) {
        uint64_t hash_val = rollingHash(rev_comp_query, i, min_length);
        rev_hash_map[hash_val].push_back(i);
    }
    
    // 在reference中查找反向互补匹配
    for (int i = 0; i <= (int)reference.length() - min_length; i++) {
        uint64_t hash_val = rollingHash(reference, i, min_length);
        
        if (rev_hash_map.find(hash_val) != rev_hash_map.end()) {
            for (int rev_pos : rev_hash_map[hash_val]) {
                int len = 0;
                while (i + len < reference.length() && 
                       rev_pos + len < rev_comp_query.length() &&
                       reference[i + len] == rev_comp_query[rev_pos + len]) {
                    len++;
                }
                
                if (len >= min_length) {
                    // 将反向互补的位置转换回原始query坐标
                    int query_end_pos = query.length() - 1 - rev_pos;
                    int query_start_pos = query_end_pos - len + 1;
                    anchors.push_back(Anchor(query_start_pos, query_end_pos, i, i + len - 1, len));
                }
            }
        }
    }
    
    return anchors;
}

// 检查两个锚点是否可以链接
bool canChain(const Anchor& a1, const Anchor& a2) {
    // 确保query和reference的顺序一致
    return a1.query_end < a2.query_start && a1.ref_end < a2.ref_start;
}

// 计算匹配质量分数
double calculateMatchQuality(const Anchor& anchor, const string& reference, const string& query) {
    // 基础分数：匹配长度
    double base_score = anchor.score;
    
    // 长度奖励：较长的匹配获得额外奖励
    double length_bonus = anchor.score >= 3 ? anchor.score * 0.1 : 0;
    
    // 位置权重：序列中间位置的匹配更重要
    int query_mid = query.length() / 2;
    int anchor_mid = (anchor.query_start + anchor.query_end) / 2;
    double position_weight = 1.0 - abs(anchor_mid - query_mid) / (double)query_mid * 0.2;
    
    return (base_score + length_bonus) * position_weight;
}

// 计算两个锚点之间的链接分数
int calculateChainScore(const Anchor& a1, const Anchor& a2, const string& reference, const string& query) {
    int query_gap = a2.query_start - a1.query_end - 1;
    int ref_gap = a2.ref_start - a1.ref_end - 1;
    
    // 基础分数
    double base_score = calculateMatchQuality(a2, reference, query);
    
    // 间隙长度惩罚
    int gap_diff = abs(query_gap - ref_gap);
    double gap_penalty = 0;
    
    if (gap_diff == 0) {
        // 完美匹配，无惩罚
        gap_penalty = 0;
    } else if (gap_diff <= 5) {
        // 小间隙差异，轻微惩罚
        gap_penalty = gap_diff * 1.5;
    } else if (gap_diff <= 20) {
        // 中等间隙差异，中等惩罚
        gap_penalty = 5 * 1.5 + (gap_diff - 5) * 3;
    } else {
        // 大间隙差异，重惩罚
        gap_penalty = 5 * 1.5 + 15 * 3 + (gap_diff - 20) * 5;
    }
    
    // 距离惩罚：锚点间距离过远会被惩罚
    int query_distance = a2.query_start - a1.query_end;
    int ref_distance = a2.ref_start - a1.ref_end;
    double distance_penalty = 0;
    
    if (query_distance > 1000 || ref_distance > 1000) {
        distance_penalty = min(query_distance, ref_distance) * 0.01;
    }
    
    // 重叠惩罚：如果锚点重叠，给予重惩罚
    double overlap_penalty = 0;
    if (a1.query_end >= a2.query_start || a1.ref_end >= a2.ref_start) {
        overlap_penalty = 1000; // 重惩罚重叠
    }
    
    // 方向一致性奖励
    double direction_bonus = 0;
    if (query_gap >= 0 && ref_gap >= 0) {
        direction_bonus = min(a1.score, a2.score) * 0.05;
    }
    
    return (int)(base_score - gap_penalty - distance_penalty - overlap_penalty + direction_bonus);
}

// 动态规划找到最优锚点链
vector<Anchor> findOptimalChain(vector<Anchor>& anchors, const string& reference, const string& query) {
    if (anchors.empty()) return anchors;
    
    // 按query位置排序
    sort(anchors.begin(), anchors.end(), 
         [](const Anchor& a, const Anchor& b) {
             return a.query_start < b.query_start;
         });
    
    int n = anchors.size();
    vector<GraphNode> nodes;
    for (int i = 0; i < n; i++) {
        nodes.push_back(GraphNode(i));
        nodes[i].max_score = anchors[i].score;
    }
    
    // 动态规划构建最优链
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (canChain(anchors[j], anchors[i])) {
                int chain_score = nodes[j].max_score + calculateChainScore(anchors[j], anchors[i], reference, query);
                if (chain_score > nodes[i].max_score) {
                    nodes[i].max_score = chain_score;
                    nodes[i].prev_id = j;
                }
            }
        }
    }
    
    // 找到最高分的终点
    int best_end = 0;
    for (int i = 1; i < n; i++) {
        if (nodes[i].max_score > nodes[best_end].max_score) {
            best_end = i;
        }
    }
    
    // 回溯构建最优链
    vector<Anchor> optimal_chain;
    int current = best_end;
    while (current != -1) {
        optimal_chain.push_back(anchors[current]);
        current = nodes[current].prev_id;
    }
    
    reverse(optimal_chain.begin(), optimal_chain.end());
    return optimal_chain;
}

// 填充锚点之间的间隙
vector<Anchor> fillGaps(const vector<Anchor>& chain, const string& reference, const string& query) {
    vector<Anchor> result = chain;
    
    // 在锚点之间添加较小的匹配
    for (int i = 0; i < (int)chain.size() - 1; i++) {
        int query_gap_start = chain[i].query_end + 1;
        int query_gap_end = chain[i + 1].query_start - 1;
        int ref_gap_start = chain[i].ref_end + 1;
        int ref_gap_end = chain[i + 1].ref_start - 1;
        
        if (query_gap_start <= query_gap_end && ref_gap_start <= ref_gap_end) {
            // 尝试在间隙中找到更小的匹配
            int gap_query_len = query_gap_end - query_gap_start + 1;
            int gap_ref_len = ref_gap_end - ref_gap_start + 1;
            
            if (gap_query_len > 0 && gap_ref_len > 0) {
                // 简单的匹配策略：如果间隙大小相似，直接匹配
                if (abs(gap_query_len - gap_ref_len) <= max(gap_query_len, gap_ref_len) / 3) {
                    int match_len = min(gap_query_len, gap_ref_len);
                    result.push_back(Anchor(query_gap_start, query_gap_start + match_len - 1,
                                          ref_gap_start, ref_gap_start + match_len - 1, match_len));
                }
            }
        }
    }
    
    // 重新排序
    sort(result.begin(), result.end(), 
         [](const Anchor& a, const Anchor& b) {
             return a.query_start < b.query_start;
         });
    
    return result;
}

// 多层次查找锚点
vector<Anchor> findAnchorsMultiLevel(const string& reference, const string& query) {
    vector<Anchor> all_anchors;
    
    // 第一层：长匹配（高质量锚点）
    vector<Anchor> long_anchors = findAnchors(reference, query, 25);
    all_anchors.insert(all_anchors.end(), long_anchors.begin(), long_anchors.end());
    
    // 第二层：中等长度匹配
    vector<Anchor> medium_anchors = findAnchors(reference, query, 15);
    all_anchors.insert(all_anchors.end(), medium_anchors.begin(), medium_anchors.end());
    
    // 第三层：短匹配（填补空隙）
    if (all_anchors.size() < 10) { // 只有在锚点不足时才使用短匹配
        vector<Anchor> short_anchors = findAnchors(reference, query, 8);
        all_anchors.insert(all_anchors.end(), short_anchors.begin(), short_anchors.end());
    }
    
    // 去重和过滤重叠锚点
    sort(all_anchors.begin(), all_anchors.end(), 
         [](const Anchor& a, const Anchor& b) {
             if (a.query_start != b.query_start) return a.query_start < b.query_start;
             return a.score > b.score; // 相同位置选择分数更高的
         });
    
    vector<Anchor> filtered_anchors;
    for (const auto& anchor : all_anchors) {
        bool should_add = true;
        for (const auto& existing : filtered_anchors) {
            // 检查是否重叠过多
            int query_overlap = min(anchor.query_end, existing.query_end) - max(anchor.query_start, existing.query_start) + 1;
            int ref_overlap = min(anchor.ref_end, existing.ref_end) - max(anchor.ref_start, existing.ref_start) + 1;
            
            if (query_overlap > anchor.score * 0.7 || ref_overlap > anchor.score * 0.7) {
                should_add = false;
                break;
            }
        }
        if (should_add) {
            filtered_anchors.push_back(anchor);
        }
    }
    
    return filtered_anchors;
}

string sequenceAlignment(string reference, string query)
{
    // 使用多层次查找锚点
    vector<Anchor> anchors = findAnchorsMultiLevel(reference, query);
    
    // 找到最优锚点链
    vector<Anchor> optimal_chain = findOptimalChain(anchors, reference, query);
    
    // 填充间隙
    vector<Anchor> final_matches = fillGaps(optimal_chain, reference, query);
    
    // 格式化输出
    stringstream ss;
    ss << "[";
    for (int i = 0; i < final_matches.size(); i++) {
        if (i > 0) ss << ", ";
        ss << "(" << final_matches[i].query_start << ", " << final_matches[i].query_end 
           << ", " << final_matches[i].ref_start << ", " << final_matches[i].ref_end << ")";
    }
    ss << "]";
    
    return ss.str();
}