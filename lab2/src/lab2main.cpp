#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "fileProcess.h"
#include "sequenceProcess.h"
using namespace std;


int main()
{
    string reference1 = readFile("reference1.in");
    string query1 = readFile("query1.in");

    cout << "process test1" << endl;
    string result1 = sequenceAlignment(reference1, query1);

    string reference2 = readFile("reference2.in");
    string query2 = readFile("query2.in");

    cout << "process test2" << endl;
    string result2 = sequenceAlignment(reference2, query2);
    
    writeFile("../result/result1.out", result1);
    writeFile("../result/result2.out", result2);

    return 0;
}