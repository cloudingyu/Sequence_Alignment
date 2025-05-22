# 字符串处理工具 API 文档

## 概述

本模块提供了一系列用于DNA序列分析的字符串处理函数，包括子串提取、字符串反转、碱基互补配对、子串查找以及滚动哈希计算等功能。这些工具函数为重复序列分析器(`Repeater`)提供底层支持。

## 函数列表

### `string subStr(const string &str, int l, int r)`

从字符串中提取指定范围的子串。

#### 参数
- `str`: 源字符串
- `l`: 子串的起始位置（包含）
- `r`: 子串的结束位置（包含）

#### 返回值
- 提取的子串

#### 示例
```cpp
string sequence = "ACGTACGT";
string fragment = subStr(sequence, 2, 5); // 返回 "GTAC"
```

### `string reverseStr(const string &str)`

将字符串反转。

#### 参数
- `str`: 要反转的字符串

#### 返回值
- 反转后的字符串

#### 示例
```cpp
string sequence = "ACGTACGT";
string reversed = reverseStr(sequence); // 返回 "TGCATGCA"
```

### `string complementStr(const string &str)`

将DNA序列转换为其碱基互补配对形式。
- A 与 T 互补
- C 与 G 互补

#### 参数
- `str`: DNA序列字符串

#### 返回值
- 互补配对后的字符串

#### 示例
```cpp
string sequence = "ACGTACGT";
string complement = complementStr(sequence); // 返回 "TGCATGCA"
```

### `vector<int> containStr(const string &text, const string &pattern)`

使用KMP算法判断模式串是否为文本串的子串，并返回所有匹配位置。

#### 参数
- `text`: 待搜索的文本字符串
- `pattern`: 要查找的模式字符串

#### 返回值
- 包含所有匹配位置的整数向量，如果没有匹配则返回空向量

#### 示例
```cpp
string text = "ACGTACGTACGT";
string pattern = "CGTA";
vector<int> positions = containStr(text, pattern); // 返回 [2, 6]
```

### `vector<vector<int>> rollingHash(const string &str, int BASE = 5, int MOD = 1000000007)`

计算字符串的滚动哈希值，用于快速子串比较。

#### 参数
- `str`: 要计算哈希值的字符串
- `BASE`: 哈希函数的基数（默认为5）
- `MOD`: 哈希值的模数（默认为1000000007）

#### 返回值
- 二维整数向量，其中 `result[i][j]` 表示子串 `str[i..j]` 的哈希值

#### 算法说明
该函数使用滚动哈希技术，预计算从任意起点到任意终点的子串哈希值：
1. 先计算所有前缀哈希值 `result[0][i]`
2. 然后利用前缀哈希值计算任意子串哈希值 `result[i][j]`
3. 通过模运算防止整数溢出

#### 示例
```cpp
string sequence = "ACGTACGT";
vector<vector<int>> hashValues = rollingHash(sequence);
int hashOfSubstring = hashValues[2][5]; // 子串 "GTAC" 的哈希值
```

## 应用场景

这些函数主要用于：
1. 重复序列识别中的子串提取和比较
2. DNA序列的正向和反向互补配对分析
3. 序列匹配的高效实现
4. 大规模DNA序列数据的快速搜索

## 性能特性

- `subStr`、`reverseStr` 和 `complementStr`: O(n) 时间复杂度，其中 n 为字符串长度
- `containStr`: 平均 O(n+m) 时间复杂度，其中 n 为文本串长度，m 为模式串长度
- `rollingHash`: O(n²) 时间复杂度，其中 n 为字符串长度，但后续子串比较仅需 O(1) 时间
