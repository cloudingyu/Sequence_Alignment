#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H
#include <iostream>
#include <vector>
using namespace std;

// 返回反转后的字符串
string reverse(const string &str);

// 返回碱基互补配对后反转过的字符串
string reverseComplement(const string &str);

// 判断 text的[textBegin,textEnd]是否包含 pattern的[patternBegin,patternEnd],返回在 text中出现的初始位置
vector<int> subString(const string &text, const string &pattern, int textBegin=0, int textEnd=0, int patternBegin=0, int patternEnd=0);

#endif
