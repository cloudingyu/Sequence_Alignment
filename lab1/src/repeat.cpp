#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// 记录重复信息
void Repeat_Segment::recordSegment(string const &seq, int loc, int len, int cnt, bool rev)
{
    sequence = seq;
    location = loc;
    length = len;
    repetitionCount = cnt;
    isReversed = rev;
}

// 按照规定格式输出所有重复序列信息
void Repeat_Collection::printResults()
{
    cout << setw(5) << "No." << " | Repetition location(ref) | Sequence length | Repetition times | Reverse" << endl;

    for (int i = 0; i < segments.size(); i++)
    {
        cout << setw(4) << i + 1 << setw(15);
        cout << segments[i].location << setw(25);
        cout << segments[i].length << setw(18);
        cout << segments[i].repetitionCount << setw(17);
        if (segments[i].isReversed)
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
}

// 分析重复序列
void Repeat_Collection::analyzeRepeats(const string &reference, const string &query)
{
    vector<vector<long long>> refeHashTable = rollingHash(reference);
    vector<vector<long long>> querHashTable = rollingHash(query);
    int refeLength = reference.length();
    int querLength = query.length();

    int head_quer = 0, tail_refe = 0, head_refe = 0;

    // 寻找最长匹配前缀
    while (reference[head_refe] == query[head_quer])
    {
        head_refe++;
        head_quer++;
    }

    while (head_quer < query.length())
    {
        // 记录当前重复匹配开始的位置
        int pointer = head_quer;

        // 开始正向和反向匹配
        while (1)
        {
            // 判断当前最长正向匹配
            int max_len = -1;
            for (int len = 0; tail_refe < head_refe - len && head_quer + len + 1 < refeLength; len++)
                if (refeHashTable[head_refe - len][head_refe - 1] == querHashTable[head_quer][head_quer + len + 1])
                    if (subStr(reference, head_refe - len, head_refe - 1) == subStr(query, head_quer, head_quer + len + 1))
                        max_len = max(max_len, len);
            if (max_len != -1)
            {
                int cnt = 1;
                while (refeHashTable[head_refe - max_len][head_refe - 1] == querHashTable[head_quer + cnt * max_len][head_quer + max_len + 1 + cnt * max_len])
                {
                    if (subStr(reference, head_refe - max_len, head_refe - 1) != subStr(query, head_quer + cnt * max_len, head_quer + max_len + 1 + cnt * max_len))
                        break;
                    cnt++;
                }
                Repeat_Segment segment(subStr(reference, head_refe - max_len, head_refe - 1), head_refe - max_len, max_len, cnt, false);
                segments.push_back(segment);
                head_quer = head_quer + cnt * max_len + 1;
                continue;
            }

            // 判断当前最长反向匹配
            /*int max_len_rev = -1;

            if (max_len_rev != -1)
            {

            }*/
            break;
        }
        tail_refe = head_refe - 1;
        head_quer = pointer;
        while (reference[head_refe] == query[head_quer] && head_refe < refeLength && head_quer < querLength)
        {
            head_refe++;
            head_quer++;
        }
    }
    return;
}