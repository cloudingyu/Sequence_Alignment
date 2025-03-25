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
void Repeat_Collection::analyzeRepeats(const string &reference, const string &query, int kmer_size)
{
    // 记录哈希简化后的长度
    int refeLength = reference.length() - kmer_size + 1;
    int querLength = query.length() - kmer_size + 1;

    // 计算 reference 串和 query 串的哈希值
    vector<long long> refeHash = rollingHash(reference, kmer_size);
    vector<long long> querHash = rollingHash(query, kmer_size);

    // 计算 reference 串的反向互补哈希值
    // 若 querHashTable[i] == refeRevHashTable[j]
    // 则 query[i..(i + kmer_size -1)] 与 reference[j..(j + kmer_size - 1)] 的反向互补
    vector<long long> refeRevHash = rollingRevHash(reference, kmer_size);

    
    
    

    return;
}