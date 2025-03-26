#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    string reference, query;

    // 读取reference.in和query.in文件
    ifstream refFile("reference.in");
    if (refFile.is_open()){getline(refFile, reference);refFile.close();}
    else{cerr << "Error to read reference.in" << endl;return 1;}
    ifstream queryFile("query.in");
    if (queryFile.is_open()){getline(queryFile, query);queryFile.close();}
    else{cerr << "Error to read query.in" << endl;return 1;}

    // 定义并初始化重复序列处理器
    Repeater results;
    results.reference = reference;
    results.query = query;
    results.kmer_size = 1;
    
    // 预处理函数
    results.prepAnalyze();

    // 分析最优路径
    results.analyzeRoute();
    
    // 提取连续子串
    results.analyzeSequence();

    // 绘制比对演示图像
    results.drawSequence();

    // 删去与原字符串相同的部分, 合并相同子串(模糊处理)
    results.eraseSequence();
    
    // 按照规定格式输出所有重复序列信息
    results.printResults();

    return 0;
}
