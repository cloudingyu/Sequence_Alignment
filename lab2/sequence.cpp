#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <cmath>
#include <set>

using namespace std;

// 定义锚点结构体
struct Anchor {
    int query_start;
    int ref_start;
    int length;
    bool is_reverse;
    
    Anchor(int qs, int rs, int len, bool rev = false) : 
        query_start(qs), ref_start(rs), length(len), is_reverse(rev) {}
    
    bool operator<(const Anchor& other) const {
        return query_start < other.query_start;
    }
};

// 定义构图后的节点结构体
struct Node {
    int query_start;
    int query_end;
    int ref_start;
    int ref_end;
    bool is_reverse;
    int score;
    int prev; // 前驱节点在向量中的索引
    
    Node(int qs, int qe, int rs, int re, bool rev, int s, int p = -1) : 
        query_start(qs), query_end(qe), ref_start(rs), ref_end(re), 
        is_reverse(rev), score(s), prev(p) {}
};

// 获取反向互补序列
string reverse_complement(const string& seq) {
    string result(seq.length(), ' ');
    for (int i = 0; i < seq.length(); i++) {
        char c = seq[seq.length() - 1 - i];
        switch (c) {
            case 'A': result[i] = 'T'; break;
            case 'T': result[i] = 'A'; break;
            case 'G': result[i] = 'C'; break;
            case 'C': result[i] = 'G'; break;
            default: result[i] = 'N';
        }
    }
    return result;
}

// 计算两个序列的编辑距离（简化版）
int edit_distance(const string& s1, const string& s2, int max_dist) {
    if (s1.length() != s2.length()) return max_dist + 1;
    
    int dist = 0;
    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] != s2[i]) dist++;
        if (dist > max_dist) return max_dist + 1;
    }
    return dist;
}

// 扩展锚点以获取更长匹配
void extend_anchor(const string& query, const string& reference, 
                   int& q_start, int& r_start, int& length, bool is_reverse) {
    // 允许的最大错配数
    const int max_mismatch_ratio = 10; // 每10个碱基允许1个错配
    int mismatch_count = 0;
    
    // 向左扩展
    int left_extend = 0;
    while (q_start - left_extend - 1 >= 0 && r_start - left_extend - 1 >= 0) {
        char q_char;
        if (is_reverse) {
            int rev_pos = q_start + length + left_extend;
            if (rev_pos >= query.length()) break;
            
            switch(query[rev_pos]) {
                case 'A': q_char = 'T'; break;
                case 'T': q_char = 'A'; break;
                case 'G': q_char = 'C'; break;
                case 'C': q_char = 'G'; break;
                default: q_char = 'N';
            }
        } else {
            q_char = query[q_start - left_extend - 1];
        }
        
        char r_char = reference[r_start - left_extend - 1];
        
        if (q_char != r_char) {
            mismatch_count++;
            if (mismatch_count > (left_extend + 1) / max_mismatch_ratio + 1) break;
        }
        left_extend++;
    }
    
    // 向右扩展
    int right_extend = 0;
    mismatch_count = 0;
    
    while (q_start + length + right_extend < query.length() && 
           r_start + length + right_extend < reference.length()) {
        char q_char;
        if (is_reverse) {
            int rev_pos = q_start - right_extend - 1;
            if (rev_pos < 0) break;
            
            switch(query[rev_pos]) {
                case 'A': q_char = 'T'; break;
                case 'T': q_char = 'A'; break;
                case 'G': q_char = 'C'; break;
                case 'C': q_char = 'G'; break;
                default: q_char = 'N';
            }
        } else {
            q_char = query[q_start + length + right_extend];
        }
        
        char r_char = reference[r_start + length + right_extend];
        
        if (q_char != r_char) {
            mismatch_count++;
            if (mismatch_count > (right_extend + 1) / max_mismatch_ratio + 1) break;
        }
        right_extend++;
    }
    
    // 更新锚点信息
    if (is_reverse) {
        q_start -= right_extend;
        length += left_extend + right_extend;
    } else {
        q_start -= left_extend;
        r_start -= left_extend;
        length += left_extend + right_extend;
    }
}

// 在query和reference序列中寻找匹配的k-mers作为锚点
vector<Anchor> find_anchors(const string& query, const string& reference, int k, int step) {
    unordered_map<string, vector<int>> kmer_positions;
    
    // 构建reference中k-mers的哈希表
    for (int i = 0; i <= reference.length() - k; i += step) {
        string kmer = reference.substr(i, k);
        kmer_positions[kmer].push_back(i);
    }
    
    // 搜索query中的k-mers
    vector<Anchor> anchors;
    string rc_query = reverse_complement(query);
    
    for (int i = 0; i <= query.length() - k; i += step) {
        // 正向匹配
        string kmer = query.substr(i, k);
        auto it = kmer_positions.find(kmer);
        if (it != kmer_positions.end()) {
            for (int pos : it->second) {
                anchors.emplace_back(i, pos, k, false);
            }
        }
        
        // 反向互补匹配
        kmer = rc_query.substr(query.length() - i - k, k);
        it = kmer_positions.find(kmer);
        if (it != kmer_positions.end()) {
            for (int pos : it->second) {
                anchors.emplace_back(i, pos, k, true);
            }
        }
    }
    
    return anchors;
}

// 合并重叠或非常接近的锚点
vector<Anchor> merge_anchors(vector<Anchor>& anchors, int max_gap) {
    if (anchors.empty()) return {};
    
    // 按query位置排序，并按照ref位置二次排序
    sort(anchors.begin(), anchors.end(), [](const Anchor& a, const Anchor& b) {
        if (a.query_start == b.query_start)
            return a.ref_start < b.ref_start;
        return a.query_start < b.query_start;
    });
    
    // 过滤掉过短的锚点，避免噪声
    vector<Anchor> filtered_anchors;
    for (const auto& anchor : anchors) {
        if (anchor.length >= 10) {
            filtered_anchors.push_back(anchor);
        }
    }
    
    if (filtered_anchors.empty()) return anchors;
    
    vector<Anchor> merged;
    Anchor current = filtered_anchors[0];
    
    for (int i = 1; i < filtered_anchors.size(); i++) {
        const Anchor& next = filtered_anchors[i];
        
        // 检查是否可以合并（在query和reference上都接近，且方向相同）
        bool can_merge = false;
        
        if (next.is_reverse == current.is_reverse) {
            int query_gap = next.query_start - (current.query_start + current.length);
            
            if (query_gap <= max_gap && query_gap >= -5) { // 允许少量重叠或间隙
                if (!current.is_reverse) {
                    int ref_gap = next.ref_start - (current.ref_start + current.length);
                    if (abs(ref_gap) <= max_gap + 5) { // 在reference上允许更大的变异
                        can_merge = true;
                    }
                } else {
                    // 反向互补情况下，ref位置是反向的
                    int ref_gap = current.ref_start - (next.ref_start + next.length);
                    if (abs(ref_gap) <= max_gap + 5) {
                        can_merge = true;
                    }
                }
            }
        }
        
        if (can_merge) {
            // 更新当前锚点长度
            if (!current.is_reverse) {
                int new_end = max(current.query_start + current.length, 
                                 next.query_start + next.length);
                current.length = new_end - current.query_start;
            } else {
                // 反向互补情况需要特殊处理
                int new_query_end = max(current.query_start + current.length, 
                                       next.query_start + next.length);
                current.length = new_query_end - current.query_start;
            }
        } else {
            merged.push_back(current);
            current = next;
        }
    }
    
    merged.push_back(current);
    
    // 二次过滤，只保留较长的锚点
    vector<Anchor> final_anchors;
    for (const auto& anchor : merged) {
        if (anchor.length >= 20) { // 增加长度阈值
            final_anchors.push_back(anchor);
        }
    }
    
    return final_anchors.empty() ? merged : final_anchors;
}

// 构建图，寻找最优路径
vector<tuple<int, int, int, int>> find_optimal_chain(vector<Anchor>& anchors, 
                                    const string& query, const string& reference) {
    if (anchors.empty()) return {};
    
    vector<Node> nodes;
    
    // 创建节点
    for (const Anchor& a : anchors) {
        int q_start = a.query_start;
        int r_start = a.ref_start;
        int q_end = q_start + a.length;
        int r_end;
        
        if (a.is_reverse) {
            // 对于反向互补，r_end < r_start
            r_end = r_start;
            r_start = r_end - a.length;
        } else {
            r_end = r_start + a.length;
        }
        
        // 计算节点得分 - 根据长度和匹配质量
        int score = a.length;
        
        // 添加节点
        nodes.emplace_back(q_start, q_end, r_start, r_end, a.is_reverse, score);
    }
    
    // 按照query起始位置排序
    sort(nodes.begin(), nodes.end(), 
         [](const Node& a, const Node& b) { return a.query_start < b.query_start; });
    
    // 动态规划：寻找得分最高的路径
    vector<int> dp(nodes.size());
    vector<int> prev(nodes.size(), -1);
    
    // 构建邻接表以加速搜索
    vector<vector<int>> adj_list(nodes.size());
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < i; j++) {
            // 节点j完全在节点i之前
            if (nodes[j].query_end <= nodes[i].query_start) {
                adj_list[i].push_back(j);
            }
        }
    }
    
    // 动态规划计算
    for (int i = 0; i < nodes.size(); i++) {
        dp[i] = nodes[i].score;
        
        // 对于每个节点，考虑之前的所有可能的前驱节点
        for (int j : adj_list[i]) {
            // 检查方向一致性和间隙大小
            bool compatible = true;
            int gap_size = nodes[i].query_start - nodes[j].query_end;
            
            // 正向匹配的连接性检查
            if (!nodes[j].is_reverse && !nodes[i].is_reverse) {
                int ref_gap = nodes[i].ref_start - nodes[j].ref_end;
                
                // 如果参考序列上的间隙与查询序列上的间隙差距过大，可能表示结构变异
                if (abs(ref_gap - gap_size) > 30) {
                    compatible = false;
                }
            } 
            // 反向匹配的连接性检查
            else if (nodes[j].is_reverse && nodes[i].is_reverse) {
                int ref_gap = nodes[j].ref_start - nodes[i].ref_end;
                
                // 反向互补情况下的间隙检查
                if (abs(ref_gap - gap_size) > 30) {
                    compatible = false;
                }
            }
            // 混合方向的匹配，允许但得分降低
            else {
                // 方向发生变化时，降低连接分数
                compatible = true;
            }
            
            // 计算新的得分
            int chain_bonus = min(20, gap_size <= 10 ? 10 : 0); // 小间隙奖励
            int new_score = dp[j] + nodes[i].score + (compatible ? chain_bonus : -5);
            
            if (new_score > dp[i]) {
                dp[i] = new_score;
                prev[i] = j;
            }
        }
    }
    
    // 找出得分最高的节点
    int best_idx = 0;
    for (int i = 1; i < nodes.size(); i++) {
        if (dp[i] > dp[best_idx]) {
            best_idx = i;
        }
    }
    
    // 回溯构建最优链
    vector<tuple<int, int, int, int>> best_chain;
    for (int i = best_idx; i != -1; i = prev[i]) {
        best_chain.emplace_back(nodes[i].query_start, nodes[i].query_end, 
                               nodes[i].ref_start, nodes[i].ref_end);
        
        // 保存节点的前驱信息，用于后续处理
        nodes[i].prev = prev[i];
    }
    reverse(best_chain.begin(), best_chain.end());
    
    return best_chain;
}

// 生成最终输出的匹配元组
vector<tuple<int, int, int, int>> generate_tuples(
    const vector<Anchor>& anchors, const string& query, const string& reference) {
    
    vector<tuple<int, int, int, int>> result;
    
    if (anchors.empty()) return result;
    
    // 分组处理正向和反向匹配
    vector<Anchor> forward_anchors, reverse_anchors;
    for (const auto& anchor : anchors) {
        if (anchor.is_reverse) {
            reverse_anchors.push_back(anchor);
        } else {
            forward_anchors.push_back(anchor);
        }
    }
    
    // 处理正向匹配
    if (!forward_anchors.empty()) {
        // 按query_start排序
        sort(forward_anchors.begin(), forward_anchors.end());
        
        // 合并连续的锚点
        vector<tuple<int, int, int, int>> forward_matches;
        Anchor current = forward_anchors[0];
        int q_end = current.query_start + current.length;
        int r_end = current.ref_start + current.length;
        
        for (int i = 1; i < forward_anchors.size(); i++) {
            const Anchor& next = forward_anchors[i];
            
            // 允许更大的间隙来处理复杂变异
            if (next.query_start <= q_end + 20 && 
                abs(next.ref_start - r_end) <= 30) { // 增加容错空间
                
                // 合并锚点，同时处理可能的插入/删除
                q_end = max(q_end, next.query_start + next.length);
                r_end = max(r_end, next.ref_start + next.length);
            } else {
                // 无法合并，添加当前匹配并重新开始
                forward_matches.emplace_back(
                    current.query_start, q_end, 
                    current.ref_start, r_end
                );
                
                current = next;
                q_end = current.query_start + current.length;
                r_end = current.ref_start + current.length;
            }
        }
        
        // 添加最后一个匹配
        forward_matches.emplace_back(
            current.query_start, q_end, 
            current.ref_start, r_end
        );
        
        // 过滤过短的匹配
        for (auto& match : forward_matches) {
            int q_start = get<0>(match);
            int q_end = get<1>(match);
            int r_start = get<2>(match);
            int r_end = get<3>(match);
            
            if (q_end - q_start >= 30) {  // 最小匹配长度阈值
                result.emplace_back(q_start, q_end, r_start, r_end);
            }
        }
    }
    
    // 处理反向匹配
    if (!reverse_anchors.empty()) {
        // 按query_start排序
        sort(reverse_anchors.begin(), reverse_anchors.end());
        
        // 合并连续的锚点
        vector<tuple<int, int, int, int>> reverse_matches;
        Anchor current = reverse_anchors[0];
        int q_end = current.query_start + current.length;
        int r_start = current.ref_start; // 反向互补下r_start是起点
        
        for (int i = 1; i < reverse_anchors.size(); i++) {
            const Anchor& next = reverse_anchors[i];
            
            // 允许更大的间隙来处理复杂变异
            if (next.query_start <= q_end + 20 &&
                abs(r_start - (next.ref_start + next.length)) <= 30) { // 反向互补情况
                
                // 合并锚点
                q_end = max(q_end, next.query_start + next.length);
                r_start = min(r_start, next.ref_start);
            } else {
                // 无法合并，添加当前匹配并重新开始
                reverse_matches.emplace_back(
                    current.query_start, q_end, 
                    current.ref_start + current.length, current.ref_start // 反向顺序：end在前，start在后
                );
                
                current = next;
                q_end = current.query_start + current.length;
                r_start = current.ref_start;
            }
        }
        
        // 添加最后一个匹配
        reverse_matches.emplace_back(
            current.query_start, q_end, 
            current.ref_start + current.length, current.ref_start // 反向顺序：end在前，start在后
        );
        
        // 过滤过短的匹配
        for (auto& match : reverse_matches) {
            int q_start = get<0>(match);
            int q_end = get<1>(match);
            int r_end = get<2>(match);
            int r_start = get<3>(match);
            
            if (q_end - q_start >= 30) {  // 最小匹配长度阈值
                // 在result中保持统一格式：(q_start, q_end, r_start, r_end)
                result.emplace_back(q_start, q_end, r_start, r_end);
            }
        }
    }
    
    // 按query_start排序
    sort(result.begin(), result.end(), 
         [](const auto& a, const auto& b) {
             return get<0>(a) < get<0>(b);
         });
    
    // 去除重叠区域，优先保留更长的匹配
    vector<tuple<int, int, int, int>> final_result;
    if (!result.empty()) {
        final_result.push_back(result[0]);
        
        for (int i = 1; i < result.size(); i++) {
            auto& last = final_result.back();
            auto& current = result[i];
            
            int last_q_end = get<1>(last);
            int curr_q_start = get<0>(current);
            
            // 如果当前匹配与前一个匹配有重叠
            if (curr_q_start < last_q_end) {
                int last_len = get<1>(last) - get<0>(last);
                int curr_len = get<1>(current) - get<0>(current);
                
                // 如果当前匹配比上一个更长，替换上一个
                if (curr_len > last_len) {
                    final_result.back() = current;
                }
                // 否则保留上一个
            } else {
                final_result.push_back(current);
            }
        }
    }
    
    return final_result;
}

// 优化的比对算法
vector<tuple<int, int, int, int>> align_sequences(
    const string& query, const string& reference) {
    
    // 根据序列长度动态调整参数
    int k, step, max_gap;
    
    // 对较长序列使用更大的k-mer进行处理，提高效率
    if (query.length() > 10000 || reference.length() > 10000) {
        k = 20;           // 较大的k-mer长度，提高特异性
        step = 3;         // 较大的步长，提高效率
        max_gap = 15;     // 较大的合并间隙，处理大范围变异
    } else {
        k = 15;           // 标准k-mer长度
        step = 1;         // 更小的步长，提高敏感性
        max_gap = 5;      // 标准合并间隙
    }
    
    // 查找锚点
    vector<Anchor> anchors = find_anchors(query, reference, k, step);
    
    // 如果锚点过少，尝试更小的k-mer
    if (anchors.size() < 10 && k > 10) {
        k -= 5;
        step = 1;
        anchors = find_anchors(query, reference, k, step);
    }
    
    // 如果仍然找不到足够的锚点，可能是序列差异太大，或者需要特殊处理
    if (anchors.size() < 5) {
        // 对于极端情况，尝试识别重复区域
        vector<tuple<int, int, int, int>> special_results;
        
        // 1. 处理大规模重复：分段查找，每段处理50个碱基
        int segment_size = 50;
        for (int i = 0; i < query.length(); i += segment_size / 2) { // 50%的重叠
            int end = min(i + segment_size, (int)query.length());
            string query_segment = query.substr(i, end - i);
            
            for (int j = 0; j < reference.length(); j += segment_size / 4) { // 75%的重叠，更密集地查找
                int ref_end = min(j + segment_size * 2, (int)reference.length()); // 查找范围扩大
                string ref_segment = reference.substr(j, ref_end - j);
                
                // 简单的相似度检查
                int match_count = 0;
                int min_len = min(query_segment.length(), ref_segment.length());
                for (int p = 0; p < min_len; p++) {
                    if (query_segment[p] == ref_segment[p]) match_count++;
                }
                
                if ((double)match_count / min_len > 0.8) { // 80%相似度阈值
                    special_results.emplace_back(i, end, j, j + min_len);
                }
                
                // 检查反向互补
                string rc_query_segment = reverse_complement(query_segment);
                match_count = 0;
                for (int p = 0; p < min(rc_query_segment.length(), ref_segment.length()); p++) {
                    if (rc_query_segment[p] == ref_segment[p]) match_count++;
                }
                
                if ((double)match_count / min_len > 0.8) {
                    special_results.emplace_back(i, end, j + min_len, j); // 注意反向互补的ref起止位置
                }
            }
        }
        
        if (!special_results.empty()) {
            return special_results;
        }
    }
    
    // 扩展并合并锚点
    for (auto& anchor : anchors) {
        extend_anchor(query, reference, anchor.query_start, anchor.ref_start, 
                     anchor.length, anchor.is_reverse);
    }
    
    vector<Anchor> merged_anchors = merge_anchors(anchors, max_gap);
    
    // 获取最优链并生成比对元组
    if (!merged_anchors.empty()) {
        auto chain_results = find_optimal_chain(merged_anchors, query, reference);
        
        // 如果链结果不佳，尝试直接生成元组
        if (chain_results.empty() || 
            (chain_results.size() == 1 && 
             get<1>(chain_results[0]) - get<0>(chain_results[0]) < 100)) {
            
            auto tuple_results = generate_tuples(merged_anchors, query, reference);
            return tuple_results.empty() ? chain_results : tuple_results;
        }
        return chain_results;
    } else {
        return generate_tuples(merged_anchors, query, reference);
    }
}

// 处理特殊情况：完全匹配场景和特殊变异
vector<tuple<int, int, int, int>> handle_exact_match(
    const string& query, const string& reference) {
    
    vector<tuple<int, int, int, int>> result;
    
    // 1. 处理完全匹配情况
    if (query.length() == reference.length()) {
        int max_dist = query.length() / 10;  // 允许10%的差异
        if (edit_distance(query, reference, max_dist) <= max_dist) {
            result.emplace_back(0, query.length(), 0, reference.length());
            return result;
        }
        
        // 检查反向互补匹配
        string rc_query = reverse_complement(query);
        if (edit_distance(rc_query, reference, max_dist) <= max_dist) {
            result.emplace_back(0, query.length(), reference.length(), 0);
            return result;
        }
    }
    
    // 2. 处理大范围的完全匹配段
    if (query.length() > 1000 && reference.length() > 1000) {
        // 寻找可能的大块匹配区域
        int k = 20; // 较大的k-mer
        unordered_map<string, int> kmer_positions;
        
        // 统计reference中的k-mers及其位置
        for (int i = 0; i <= reference.length() - k; i += 5) { // 五步一查，提高效率
            string kmer = reference.substr(i, k);
            kmer_positions[kmer] = i; // 仅保留一个位置，避免重复
        }
        
        // 在query中寻找匹配的k-mers
        vector<pair<int, int>> potential_matches; // (query_pos, ref_pos)
        for (int i = 0; i <= query.length() - k; i += 5) {
            string kmer = query.substr(i, k);
            if (kmer_positions.count(kmer)) {
                potential_matches.emplace_back(i, kmer_positions[kmer]);
            }
        }
        
        // 分析可能的大块匹配
        if (potential_matches.size() > 10) { // 至少有10个匹配点
            sort(potential_matches.begin(), potential_matches.end());
            
            // 找到最密集的匹配区域
            int best_count = 0;
            int best_q_start = 0, best_r_start = 0;
            
            for (int i = 0; i < potential_matches.size(); i++) {
                int q_start = potential_matches[i].first;
                int r_start = potential_matches[i].second;
                
                int count = 1;
                int last_q_pos = q_start;
                int last_r_pos = r_start;
                
                for (int j = i + 1; j < potential_matches.size(); j++) {
                    int q_pos = potential_matches[j].first;
                    int r_pos = potential_matches[j].second;
                    
                    int q_diff = q_pos - last_q_pos;
                    int r_diff = r_pos - last_r_pos;
                    
                    if (abs(q_diff - r_diff) < 10) { // 间隔变化不大
                        count++;
                        last_q_pos = q_pos;
                        last_r_pos = r_pos;
                    }
                }
                
                if (count > best_count) {
                    best_count = count;
                    best_q_start = q_start;
                    best_r_start = r_start;
                }
            }
            
            if (best_count >= 5) { // 至少有5个连续匹配点
                int best_q_end = best_q_start + 1000; // 假设匹配区域为1000bp
                int best_r_end = best_r_start + 1000;
                
                best_q_end = min(best_q_end, (int)query.length());
                best_r_end = min(best_r_end, (int)reference.length());
                
                // 验证这个区域的匹配质量
                string q_seg = query.substr(best_q_start, best_q_end - best_q_start);
                string r_seg = reference.substr(best_r_start, best_r_end - best_r_start);
                
                int max_dist = q_seg.length() / 10;  // 允许10%的差异
                if (edit_distance(q_seg, r_seg, max_dist) <= max_dist) {
                    result.emplace_back(best_q_start, best_q_end, best_r_start, best_r_end);
                }
            }
        }
    }
    
    return result;
}

// 主函数
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <query_file> <ref_file> <output_file>" << endl;
        return 1;
    }
    
    // 读取输入文件
    ifstream query_file(argv[1]);
    ifstream ref_file(argv[2]);
    
    if (!query_file || !ref_file) {
        cerr << "Failed to open input files!" << endl;
        return 1;
    }
    
    string query, reference;
    getline(query_file, query);
    getline(ref_file, reference);
    
    query_file.close();
    ref_file.close();
    
    cout << "Query length: " << query.length() << endl;
    cout << "Reference length: " << reference.length() << endl;
    
    // 执行序列比对
    vector<tuple<int, int, int, int>> alignments;
    
    // 先尝试处理特殊情况：完全匹配
    alignments = handle_exact_match(query, reference);
    
    // 如果没有找到完全匹配，执行常规比对
    if (alignments.empty()) {
        alignments = align_sequences(query, reference);
    }
    
    // 写入结果
    ofstream output_file(argv[3]);
    if (!output_file) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }
    
    output_file << "[";
    for (int i = 0; i < alignments.size(); i++) {
        if (i > 0) output_file << ", ";
        output_file << "(" << get<0>(alignments[i]) << ", " 
                   << get<1>(alignments[i]) << ", " 
                   << get<2>(alignments[i]) << ", " 
                   << get<3>(alignments[i]) << ")";
    }
    output_file << "]";
    
    output_file.close();
    cout << "Alignment completed successfully." << endl;
    
    return 0;
}