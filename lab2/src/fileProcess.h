#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <iostream>
#include <string>
using namespace std;

string readFile(const string& filename);

void writeFile(const string& filename, const string& content);
#endif