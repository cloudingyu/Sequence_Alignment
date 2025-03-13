#include <iostream>
#include <vector>
#include <map>

// ...existing code...
// 注意：对文件中"计算机科学与技术"和"计算机科学与分析"路径保持一致

// 新增函数：分析给定字符串并输出重复子串结果
void analyzeString(const string &S, const string &title) {
    // 重置全局状态
    st.clear();
    sz = 0;
    last = 0;
    // 初始化 suffix automaton
    sa_init(S.size());
    for (int i = 0; i < (int)S.size(); i++) {
        sa_extend(S[i], i);
    }
    vector<int> order(sz);
    for (int i = 0; i < sz; i++) order[i] = i;
    sort(order.begin(), order.end(), cmp_by_len);
    for (int i = 0; i < sz; i++) {
        int s = order[i];
        if (st[s].link != -1)
            st[st[s].link].occ += st[s].occ;
    }
    
    // 收集候选重复子串，用 map 去重
    map<string, Candidate> candMap;
    for (int i = 0; i < sz; i++) {
        if (st[i].occ >= 2 && st[i].len > 10) {
            int end_pos = st[i].first_pos;
            int start_pos = end_pos - st[i].len + 1;
            string sub = S.substr(start_pos, st[i].len);
            if (candMap.find(sub) == candMap.end()) {
                Candidate cand;
                cand.s = sub;
                cand.count = 0;
                candMap[sub] = cand;
            }
            for (int pos = 0; pos <= (int)S.size() - st[i].len; pos++) {
                if (S.substr(pos, st[i].len) == sub) {
                    pair<int,int> pr = make_pair(pos, pos + st[i].len - 1);
                    if(find(candMap[sub].positions.begin(), candMap[sub].positions.end(), pr) == candMap[sub].positions.end())
                        candMap[sub].positions.push_back(pr);
                }
            }
            candMap[sub].count = candMap[sub].positions.size();
        }
    }
    
    vector<Candidate> candidates;
    for (map<string, Candidate>::iterator it = candMap.begin(); it != candMap.end(); ++it) {
        candidates.push_back(it->second);
    }
    
    // 过滤：如果一个候选子串是另一个更长候选子串的子串，则舍去之
    vector<bool> covered(candidates.size(), false);
    for (size_t i = 0; i < candidates.size(); i++) {
        for (size_t j = 0; j < candidates.size(); j++) {
            if (i == j) continue;
            if (candidates[j].s.size() > candidates[i].s.size()) {
                if (candidates[j].s.find(candidates[i].s) != string::npos) {
                    covered[i] = true;
                    break;
                }
            }
        }
    }
    
    // 对每个候选的出现位置进行排序
    for (size_t i = 0; i < candidates.size(); i++) {
        sort(candidates[i].positions.begin(), candidates[i].positions.end(), cmpPositions);
    }
    
    // 收集未被覆盖的候选子串索引，并按第一次出现位置排序
    vector<int> uncoveredIndices;
    for (size_t i = 0; i < candidates.size(); i++) {
        if (!covered[i])
            uncoveredIndices.push_back(i);
    }
    struct FirstOccurrenceComparator {
        const vector<Candidate>& candidates;
        FirstOccurrenceComparator(const vector<Candidate>& c) : candidates(c) {}
        bool operator()(int a, int b) const {
            return candidates[a].positions.front().first < candidates[b].positions.front().first;
        }
    } comp(candidates);
    sort(uncoveredIndices.begin(), uncoveredIndices.end(), comp);
    
    cout << "======= " << title << " 分析结果 =======" << endl;
    bool found = false;
    for (size_t k = 0; k < uncoveredIndices.size(); k++) {
        int i = uncoveredIndices[k];
        cout << "重复子串: " << candidates[i].s << endl;
        cout << "重复次数: " << candidates[i].count << endl;
        cout << "所有出现位置: ";
        for (vector<pair<int,int> >::const_iterator it = candidates[i].positions.begin();
             it != candidates[i].positions.end(); ++it) {
            cout << "[起始:" << it->first << ", 结束:" << it->second << "] ";
        }
        cout << endl;
        found = true;
    }
    if (!found)
        cout << "未找到满足条件的重复子串" << endl;
    cout << endl;
}

int main()
{
    // 分析 Reference
    analyzeString(Reference, "Reference");
    
    // 为分析 Query 重新初始化全局状态
    // 清空 st 及重置 sz, last
    st.clear();
    sz = 0;
    last = 0;
    // 分析 Query
    analyzeString(Query, "Query");
    
    return 0;
}
// ...existing code...
