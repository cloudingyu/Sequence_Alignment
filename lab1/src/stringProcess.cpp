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

// 判断 pattern 是否为 text 的子串, 返回在 text 中出现的初始位置
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

// 滚动哈希实现哈希值计算, rollingHash[i][j] (i<=j) 表示 str[i..j] 的哈希值
vector<vector<int>> rollingHash(const string &str, int BASE, int MOD)
{
    int length = str.length();
    vector<vector<int>> result(length, vector<int>(length, 0));

    // 预处理 BASE 的 n 次幂，使用long long防溢出
    vector<long long> BASE_pow(length, 1);
    for (int i = 1; i < length; i++)
        BASE_pow[i] = (BASE_pow[i - 1] * BASE) % MOD;

    // 计算所有 str[0..i - 1] 的哈希值
    result[0][0] = str[0] - 'A' + 1;
    for (int i = 1; i < length; i++)
        result[0][i] = (int)(((long long)result[0][i - 1] * BASE + (str[i] - 'A' + 1)) % MOD);

    // 计算所有哈希值
    for(int len=1; len<length+1; len++)
        for(int i=1; i<length-len+1; i++) 
        {
            long long term1 = result[i-1][i+len-2];
            long long term2 = (BASE_pow[len-1] * (str[i-1] - 'A' + 1)) % MOD;
            long long diff = (term1 - term2 + MOD) % MOD;
            result[i][i+len-1] = (int)(((diff * BASE) % MOD + (str[i+len-1] - 'A' + 1)) % MOD);
        }
    
    return result;
}
