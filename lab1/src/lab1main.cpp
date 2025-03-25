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

    freopen("output.out","w",stdout);
    // 定义并初始化重复序列处理器
    Repeater results;
    results.reference = reference;
    results.query = query;
    results.kmer_size = 1;

    results.prepAnalyze();

    results.analyzeRoute();

    results.analyzeRepeats();
    
    results.printResults();

    return 0;
}