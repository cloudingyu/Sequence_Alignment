#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "fileProcess.h"
using namespace std;

string sequenceAlignment(string reference, string query)
{
    return "[((0,0),(0,0))]";
}

int main()
{

    string reference1 = readFile("sequence/reference.in");
    string query1 = readFile("sequence/query.in");

    cout << "process test1" << endl;
    string result1 = sequenceAlignment(reference1, query1);

    string reference2 = readFile("sequence/reference2.in");
    string query2 = readFile("sequence/query2.in");

    cout << "process test2" << endl;
    string result2 = sequenceAlignment(reference2, query2);

    writeFile("result/result1.out", result1);
    writeFile("result/result2.out", result2);

    return 0;
}