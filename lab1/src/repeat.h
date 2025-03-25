#ifndef REPEAT_H
#define REPEAT_H

#include <iostream>
#include <vector>
using namespace std;

const int MIN_INF = -2147483648;

// 单个重复序列信息
class Repeat_Segment
{
public:
    // 重复序列
    string sequence;

    // 重复序列在 reference 的起始位置
    int location;

    // 重复序列长度
    int length;

    // 重复次数
    int repetitionCount;

    // 是否反向
    bool isReversed;

public:
    // 默认构造函数
    Repeat_Segment() : sequence(""), location(0), length(0), repetitionCount(0), isReversed(false) {}
    
    // 带参构造函数
    Repeat_Segment(const string &seq, int loc, int len, int cnt, bool rev) : 
        sequence(seq), location(loc), length(len), repetitionCount(cnt), isReversed(rev) {}
};

// 重复序列处理器
class Repeater
{
public:
    // 所有重复序列信息集合
    vector<Repeat_Segment> segments;

    // 参考序列和查询序列
    string reference;
    string query;

    // 参考序列和查询序列的长度
    int refeLength;
    int querLength;

    // 参考序列和查询序列的哈希值
    vector<vector<int>> refeHash;
    vector<vector<int>> refeRevHash;
    vector<vector<int>> querHash;

    // 比对的最小碱基对长度
    int kmer_size=1;

    // 记录 query 第 i 个碱基对应 reference 第 j个碱基的最大得分和前置节点
    struct Align_Point
    {
        double maxScore;
        int prevIndex;
        int continuousCount;
    };
    vector<vector<Align_Point>> Align;

    // 记录 query 第 i 个碱基对所对应最大得分, 
    // 对应的 reference 位置
    // 前置节点对应 reference 位置
    struct Max_Point
    {
        double maxScore;
        int maxScoreIndex;
        int prevIndex;
    };
    vector<Max_Point> querAlign;


public:
    // 预处理函数
    void prepAnalyze();

    // 比对函数
    bool isEqual(int i,int j);

    // 比对函数
    bool isMatch(int i,int j);

    // 分析最优路径
    void analyzeRoute();

    // 分析重复序列
    void analyzeRepeats();

    // 按照规定格式输出所有重复序列信息
    void printResults();
};

#endif