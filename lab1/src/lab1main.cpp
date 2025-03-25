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

    // 主逻辑
    Repeat_Collection results;

    results.analyzeRepeats(reference, query, 3);
    
    results.printResults();

    return 0;
}