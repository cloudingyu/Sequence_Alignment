#include "stringProcess.h"
#include <iostream>
using namespace std;

// 反转字符串
string reverse(const string &str)
{
    string result = str;
    int length = str.length();
    for (int i = 0; i < length / 2; i++)
    {
        char temp = result[i];
        result[i] = result[length - 1 - i];
        result[length - 1 - i] = temp;
    }
    return result;
}

// 碱基互补配对反转字符串
string reverseComplement(const string &str)
{
    string result = str;
    int length = str.length();

    for (int i = 0; i < length; i++)
        switch (result[i])
        {
        case 'A':
            result[i] = 'T';
            break;
        case 'T':
            result[i] = 'A';
            break;
        case 'C':
            result[i] = 'G';
            break;
        case 'G':
            result[i] = 'C';
            break;
        }
    return reverse(result);
}

// 判断str1的[l1,r1]是否包含str2的[l2,r2]
int subString(const string &str1, int l1, int r1, const string &str2, int l2, int r2)
{

    return 0;
}