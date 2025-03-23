#include "repeat.h"
#include"stringProcess.h"
#include <iostream>
#include <iomanip>
using namespace std;

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
    cout<<"Helloworld!"<<endl;
    return;
}
