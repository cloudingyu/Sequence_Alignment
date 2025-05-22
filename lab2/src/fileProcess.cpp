#include "fileProcess.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

string readFile(const string& filename)
{
    string result;
    ifstream Filee(filename);
    if (Filee.is_open())
    {
        getline(Filee, result);
        Filee.close();
    }
    else
    {
        cerr << "Error: Unable to read file: " << filename << endl;
        return "";
    }
    return result;
 }

void writeFile(const string& filename, const string& content)
{
    ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << content;
        outFile.close();
    }
    else
    {
        cerr << "Error: Unable to write to file: " << filename << endl;
    }
}