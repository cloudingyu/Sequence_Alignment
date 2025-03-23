#include "stringProcess.h"
#include <iostream>
#include <vector>
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

// 判断 text的[textBegin,textEnd]是否包含 pattern的[patternBegin,patternEnd],返回在 text中出现的初始位置
vector<int> subString(const string &text, const string &pattern, int textBegin, int textEnd, int patternBegin, int patternEnd)
{
    vector<int> result;
    if (textEnd == 0)
        textEnd = text.length() - 1;
    if (patternEnd == 0)
        patternEnd = pattern.length() - 1;
    int textLength = textEnd - textBegin + 1;
    int patternLength = patternEnd - patternBegin + 1;

    vector<int> next(patternLength, 0);

    for (int i = 1; i < patternLength; i++)
    {
        int j = next[i - 1];
        while (j > 0 && pattern[patternBegin + i] != pattern[patternBegin + j])
            j = next[j - 1];
        if (pattern[patternBegin + i] == pattern[patternBegin + j])
            j++;
        next[i] = j;
    }


    int j = 0;
    for (int i = textBegin; i <= textEnd; i++)
    {
        while (j > 0 && text[i] != pattern[patternBegin + j])
            j = next[j - 1];
        if (text[i] == pattern[patternBegin + j])
            j++;
        if (j == patternLength)
        {
            result.push_back(i - patternLength + 1);
            j = next[j - 1];
        }
    }
    return result;
}