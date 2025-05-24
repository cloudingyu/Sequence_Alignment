#include "sequenceProcess.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Anchor
{
    int query_start, query_end;
    int ref_start, ref_end;
    int score;
    bool ifReverse;

    Anchor(int qs, int qe, int rs, int re, int s, bool ir) : query_start(qs), query_end(qe), ref_start(rs), ref_end(re), score(s), ifReverse(ir) {}
};

struct GraphNode
{
    int anchor_id;
    int max_score;
    int prev_id;

    GraphNode(int id) : anchor_id(id), max_score(0), prev_id(-1) {}
};

vector<long long> rollingHash(const string &str, int len)
{
    const int base = 5;
    const long long MOD = 1000000007;
    int n = str.length();
    vector<long long> hashes(n - len + 1, 0);

    long long hash_val = 0;

    for (int i = 0; i < len; i++)
    {
        hash_val = (hash_val * base + (str[i] - 'A' + 1)) % MOD;
    }

    hashes[0] = hash_val;

    long long max_power = 1;

    for (int i = 0; i < len - 1; i++)
    {
        max_power = (max_power * base) % MOD;
    }

    for (int i = 1; i < n - len + 1; i++)
    {
        hash_val = ((hash_val - ((str[i - 1] - 'A' + 1) * max_power) % MOD + MOD) % MOD) * base % MOD;
        hash_val = (hash_val + (str[i + len - 1] - 'A' + 1)) % MOD;
        hashes[i] = hash_val;
    }

    return hashes;
}

string getReverseComplement(const string &dna)
{
    string rev_comp = dna;
    for (char &c : rev_comp)
    {
        switch (c)
        {
        case 'A':
            c = 'T';
            break;
        case 'T':
            c = 'A';
            break;
        case 'C':
            c = 'G';
            break;
        case 'G':
            c = 'C';
            break;
        }
    }
    reverse(rev_comp.begin(), rev_comp.end());
    return rev_comp;
}

double calculateMatchQuality(const Anchor &anchor, const string &reference, const string &query)
{
    double base_score = anchor.score;

    double length_bonus = anchor.score >= 3 ? anchor.score * 0.1 : 0;

    int query_mid = query.length() / 2;
    int anchor_mid = (anchor.query_start + anchor.query_end) / 2;
    double position_weight = 1.0 - abs(anchor_mid - query_mid) / (double)query_mid * 0.2;

    if (anchor.ifReverse)
    {
        position_weight *= 0.5; // 反向互补匹配的权重降低
    }

    return (base_score + length_bonus) * position_weight;
}

vector<Anchor> findAnchors(const string &reference, const string &query, int min_length)
{
    vector<Anchor> anchors;

    vector<long long> ref_hashes = rollingHash(reference, min_length);
    unordered_map<long long, vector<int>> ref_hash_map;

    for (int i = 0; i < ref_hashes.size(); i++)
    {
        ref_hash_map[ref_hashes[i]].push_back(i);
    }

    vector<long long> query_hashes = rollingHash(query, min_length);
    
    auto hasOverlap = [&anchors](int q_start, int q_end, int r_start, int r_end) -> bool {
        for (const auto& anchor : anchors) {
            bool q_overlap = max(q_start, anchor.query_start) <= min(q_end, anchor.query_end);
            bool r_overlap = max(r_start, anchor.ref_start) <= min(r_end, anchor.ref_end);
            
            if (q_overlap && r_overlap) {
                return true;
            }
        }
        return false;
    };

    for (int i = 0; i < query_hashes.size(); i++)
    {
        long long hash_val = query_hashes[i];

        if (ref_hash_map.find(hash_val) != ref_hash_map.end())
        {
            for (int ref_pos : ref_hash_map[hash_val])
            {
                int len = 0;
                while (i + len < query.length() && ref_pos + len < reference.length() && 
                       query[i + len] == reference[ref_pos + len])
                {
                    len++;
                }
                
                if (len < min_length) {
                    continue;
                }
                
                if (hasOverlap(i, i + len - 1, ref_pos, ref_pos + len - 1)) {
                    continue;
                }

                anchors.push_back(Anchor(i, i + len - 1, ref_pos, ref_pos + len - 1, len, false));
                anchors.back().score = calculateMatchQuality(anchors.back(), reference, query);
            }
        }
    }

    string rev_comp_query = getReverseComplement(query);
    vector<long long> rev_hashes = rollingHash(rev_comp_query, min_length);

    unordered_map<long long, vector<int>> rev_hash_map;
    for (int i = 0; i < rev_hashes.size(); i++)
    {
        rev_hash_map[rev_hashes[i]].push_back(i);
    }

    for (int i = 0; i < ref_hashes.size(); i++)
    {
        long long hash_val = ref_hashes[i];

        if (rev_hash_map.find(hash_val) != rev_hash_map.end())
        {
            for (int rev_pos : rev_hash_map[hash_val])
            {
                int len = 0;
                while (i + len < reference.length() && rev_pos + len < rev_comp_query.length() && 
                       reference[i + len] == rev_comp_query[rev_pos + len])
                {
                    len++;
                }

                if (len < min_length) {
                    continue;
                }

                int query_end_pos = query.length() - 1 - rev_pos;
                int query_start_pos = query_end_pos - len + 1;
                
                if (hasOverlap(query_start_pos, query_end_pos, i, i + len - 1)) {
                    continue;
                }
                
                anchors.push_back(Anchor(query_start_pos, query_end_pos, i, i + len - 1, len, true));
                anchors.back().score = calculateMatchQuality(anchors.back(), reference, query);
            }
        }
    }

    sort(anchors.begin(), anchors.end(),
         [](const Anchor &a, const Anchor &b)
         {
             return a.query_start < b.query_start;
         });

    return anchors;
}

bool canChain(const Anchor &a1, const Anchor &a2)
{
    return a1.query_end < a2.query_start;
}

int calculateChainScore(const Anchor &a1, const Anchor &a2, const string &reference, const string &query)
{
    int query_gap = a2.query_start - a1.query_end - 1;
    int ref_gap = a2.ref_start - a1.ref_end - 1;

    double base_score = a2.score;

    int gap_diff = abs(query_gap - ref_gap);
    double gap_penalty = 0;

    if (gap_diff == 0)
    {
        gap_penalty = 0;
    }
    else if (gap_diff <= 5)
    {
        gap_penalty = gap_diff * 1.5;
    }
    else if (gap_diff <= 20)
    {
        gap_penalty = 5 * 1.5 + (gap_diff - 5) * 3;
    }
    else
    {
        gap_penalty = 5 * 1.5 + 15 * 3 + (gap_diff - 20) * 5;
    }

    double direction_bonus = 0;
    if (query_gap >= 0 && ref_gap >= 0)
    {
        direction_bonus = min(a1.score, a2.score) * 0.05;
    }

    return (int)(base_score - gap_penalty + direction_bonus);
}

vector<Anchor> findOptimalChain(vector<Anchor> &anchors, const string &reference, const string &query)
{
    if (anchors.empty())
        return anchors;

    // 首先按照query_start排序锚点
    sort(anchors.begin(), anchors.end(),
         [](const Anchor &a, const Anchor &b)
         {
             return a.query_start < b.query_start;
         });

    int n = anchors.size();
    vector<int> dp(n); // dp[i]表示以第i个锚点为结尾的最高分数
    vector<int> prev(n, -1); // 记录前驱节点

    // 初始化DP数组
    for (int i = 0; i < n; i++) {
        dp[i] = anchors[i].score; // 每个锚点至少贡献自身的分数
    }

    // 自底向上填充DP表
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            // 确保锚点j可以链接到锚点i（query序列上不重叠）
            if (anchors[j].query_end < anchors[i].query_start) {
                int chain_score = dp[j] + calculateChainScore(anchors[j], anchors[i], reference, query);
                if (chain_score > dp[i]) {
                    dp[i] = chain_score;
                    prev[i] = j;
                }
            }
        }
    }

    // 找到得分最高的终点
    int best_end = 0;
    for (int i = 1; i < n; i++) {
        if (dp[i] > dp[best_end]) {
            best_end = i;
        }
    }

    // 回溯构建最优链
    vector<Anchor> optimal_chain;
    int current = best_end;
    while (current != -1) {
        optimal_chain.push_back(anchors[current]);
        current = prev[current];
    }

    // 由于是从终点回溯，需要反转结果
    reverse(optimal_chain.begin(), optimal_chain.end());
    return optimal_chain;
}

string sequenceAlignment(string reference, string query)
{
    cout << "ref_length: " << reference.length() << ", query_length: " << query.length() << endl;

    vector<Anchor> anchors = findAnchors(reference, query, 30);

    cout << "anchors_count = " << anchors.size() << endl;

    for (int i = 0; i < anchors.size(); i++)
    {
        cout << "anchor No." << i << " (query: " << anchors[i].query_start << "-" << anchors[i].query_end
             << ", ref: " << anchors[i].ref_start << "-" << anchors[i].ref_end
             << ", score: " << anchors[i].score << ", ifReverse: " << (anchors[i].ifReverse ? "True" : "False") << ")" << endl;
    }

    vector<Anchor> optimal_chain = findOptimalChain(anchors, reference, query);

    stringstream ss;
    ss << "[";
    for (int i = 0; i < optimal_chain.size(); i++)
    {
        if (i > 0)
            ss << ", ";
        ss << "((" << optimal_chain[i].query_start << ", " << optimal_chain[i].query_end
           << "),( " << optimal_chain[i].ref_start << ", " << optimal_chain[i].ref_end << "))";
    }
    ss << "]";

    return ss.str();
}