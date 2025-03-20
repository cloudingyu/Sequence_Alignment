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
};

// 重复序列信息集合
class Repeat_Collection
{
public:
    vector<Repeat_Segment> segments;
    int count = 0;

public:
    // 按照规定格式输出所有重复序列信息
    void printResults();
    // 分析重复序列
    void analyzeRepeats(const string &reference, const string &query);
};

#endif