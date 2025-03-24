#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H

#include <iostream>
#include <vector>
using namespace std;

// 返回子串
string subStr(const string &str, int l, int r);

// 返回反转后的字符串
string reverseStr(const string &str);

// 返回碱基互补配对后的字符串
string complementStr(const string &str);

// 判断 pattern 是否为 text 的子串,返回在 text 中出现的初始位置
vector<int> containStr(const string &text, const string &pattern);

#endif
