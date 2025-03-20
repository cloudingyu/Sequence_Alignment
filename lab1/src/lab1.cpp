#include "repeat.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    string reference, query;

    ifstream refFile("reference.in");
    if(refFile.is_open()) {
        refFile >> reference;
        refFile.close();
    } else {
        cerr << "无法打开reference.in文件!" << endl;
        return 1;
    }
    
    ifstream queryFile("query.in");
    if(queryFile.is_open()) {
        queryFile >> query;
        queryFile.close();
    } else {
        cerr << "无法打开query.in文件!" << endl;
        return 1;
    }

    Repeat_Collection results;
    results.analyzeRepeats(reference, query);
    results.printResults();

    return 0;
}