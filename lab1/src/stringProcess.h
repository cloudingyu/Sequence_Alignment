#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H
#include <iostream>
using namespace std;

// 返回反转后的字符串
string reverse(const string &str);

// 返回碱基互补配对后反转过的字符串
string reverseComplement(const string &str);

// 判断str1的[l1,r1]是否包含str2的[l2,r2]
// 如果str1的子串包含str2的子串，返回str2子串在str1子串中的起始位置
// 如果不包含，返回-1
int subString(const string &str1, int l1, int r1, const string &str2, int l2, int r2);

#endif
