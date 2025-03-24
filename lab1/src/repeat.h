#ifndef REPEAT_H
#define REPEAT_H

#include <iostream>
#include <vector>
using namespace std;

// 单个重复序列信息
class Repeat_Segment
{
public:
    string sequence;
    int location;
    int length;
    int repetitionCount;
    bool isReversed;

public:
    // 默认构造函数
    Repeat_Segment() : sequence(""), location(0), length(0), repetitionCount(0), isReversed(false) {}
    
    // 带参构造函数
    Repeat_Segment(const string &seq, int loc, int len, int cnt, bool rev) : 
        sequence(seq), location(loc), length(len), repetitionCount(cnt), isReversed(rev) {}
        
    // 记录重复信息
    void recordSegment(string const &seq, int loc, int len, int cnt, bool rev);

};

// 重复序列信息集合
class Repeat_Collection
{
public:
    vector<Repeat_Segment> segments;

public:
    // 按照规定格式输出所有重复序列信息
    void printResults();

    // 分析重复序列
    void analyzeRepeats(const string &reference, const string &query);
};

#endif