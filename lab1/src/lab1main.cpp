#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>  // 用于计时
#include <iomanip> // 用于格式化输出

using namespace std;
using namespace std::chrono;

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

    cout << "参考序列长度: " << reference.length() << endl;
    cout << "查询序列长度: " << query.length() << endl;

    // 定义并初始化重复序列处理器
    Repeater results;
    results.reference = reference;
    results.query = query;
    results.kmer_size = 1;

    // 开始计时 - 预处理阶段
    auto start_prep = high_resolution_clock::now();
    
    results.prepAnalyze();
    
    // 结束计时 - 预处理阶段
    auto end_prep = high_resolution_clock::now();
    auto duration_prep = duration_cast<milliseconds>(end_prep - start_prep);
    
    // 开始计时 - 路径分析阶段
    auto start_route = high_resolution_clock::now();
    
    results.analyzeRoute();
    
    // 结束计时 - 路径分析阶段
    auto end_route = high_resolution_clock::now();
    auto duration_route = duration_cast<milliseconds>(end_route - start_route);
    
    // 开始计时 - 重复序列分析阶段
    auto start_repeats = high_resolution_clock::now();
    
    results.analyzeRepeats();
    
    // 结束计时 - 重复序列分析阶段
    auto end_repeats = high_resolution_clock::now();
    auto duration_repeats = duration_cast<milliseconds>(end_repeats - start_repeats);
    
    // 总时间
    auto total_duration = duration_cast<milliseconds>(end_repeats - start_prep);
    
    // 打印结果
    results.printResults();
    
    // 打印性能数据
    cout << "\n========== 性能统计 ==========\n";
    cout << "预处理时间: " << duration_prep.count() << " 毫秒\n";
    cout << "路径分析时间: " << duration_route.count() << " 毫秒\n";
    cout << "重复序列分析时间: " << duration_repeats.count() << " 毫秒\n";
    cout << "总执行时间: " << total_duration.count() << " 毫秒\n";
    cout << "==============================\n";

    return 0;
}