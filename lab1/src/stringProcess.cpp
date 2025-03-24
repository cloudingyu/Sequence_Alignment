#include "stringProcess.h"
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

// 返回子串
string subStr(const string &str, int l, int r)
{
    string result = str.substr(l, r - l + 1);
    return result;
}

// 反转字符串
string reverseStr(const string &str)
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
string complementStr(const string &str)
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
    return result;
}

// 判断 pattern 是否为 text 的子串,返回在 text 中出现的初始位置
vector<int> containStr(const string &text, const string &pattern)
{
    vector<int> result;

    vector<int> next(pattern.length(), 0);

    for (int i = 1; i < pattern.length(); i++)
    {
        int j = next[i - 1];
        while (j > 0 && pattern[i] != pattern[j])
            j = next[j - 1];
        if (pattern[i] == pattern[j])
            j++;
        next[i] = j;
    }

    int j = 0;
    for (int i = 0; i < text.length(); i++)
    {
        while (j > 0 && text[i] != pattern[j])
            j = next[j - 1];
        if (text[i] == pattern[j])
            j++;
        if (j == pattern.length())
        {
            result.push_back(i - pattern.length() + 1);
            j = next[j - 1];
        }
    }
    return result;
}

// 滚动哈希实现哈希匹配, result[i][j](i<=j) 表示 str[i..j] 的哈希值
vector<vector<long long>> rollingHash(const string &str, int BASE, int MOD)
{
    int length = str.length();

    // 创建一个 length × length 的二维数组，初始值为0
    vector<vector<long long>> result(length + 5, vector<long long>(length + 5, 0));

    // 预处理 BASE 的 n 次幂
    vector<long long> BASE_pow(length, 1);
    for (int i = 1; i < length; i++)
        BASE_pow[i] = (BASE_pow[i - 1] * BASE) % MOD;

    // 计算所有单字符的哈希值
    for (int i = 0; i < length; i++)
        result[i][i] = (str[i] - 'A' + 1);

    for (int len = 2; len <= length; len++)
        for (int i = 0; i <= length - len; i++)
        {
            int j = i + len - 1;
            result[i][j] = (result[i][j - 1] * BASE + (str[j] - 'A' + 1)) % MOD;
        }

    return result;
}