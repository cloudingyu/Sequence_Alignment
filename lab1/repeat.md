# 重复序列分析器 API 文档

## 概述

该程序用于识别和分析DNA序列中的重复片段。通过比较参考序列(reference)和查询序列(query)，程序能够找出相似或重复的DNA片段，包括正向和反向重复序列，并提供重复位置、长度和重复次数等信息。

## 类结构

### `Repeat_Segment` 类

表示单个重复序列片段的信息。

#### 属性

- `string sequence`：重复序列的碱基序列
- `int location`：重复序列在参考序列中的起始位置
- `int length`：重复序列的长度
- `int repetitionCount`：重复次数
- `bool isReversed`：是否为反向重复序列

#### 构造函数

- `Repeat_Segment()`：默认构造函数，初始化所有属性为默认值
- `Repeat_Segment(const string &seq, int loc, int len, int cnt, bool rev)`：带参构造函数，用于创建完整的重复序列对象

### `Repeater` 类

重复序列分析器的主要类，处理序列分析和匹配。

#### 属性

- `vector<Repeat_Segment> segments`：存储所有找到的重复序列片段
- `string reference`：参考序列
- `string query`：查询序列
- `int refeLength`：参考序列长度
- `int querLength`：查询序列长度
- `vector<vector<int>> refeHash`：参考序列的哈希值
- `vector<vector<int>> refeRevHash`：反向参考序列的哈希值
- `vector<vector<int>> querHash`：查询序列的哈希值
- `int kmer_size`：比对的最小碱基对长度，默认为1

#### 辅助数据结构

- `struct Align_Point`：记录比对信息
  - `double maxScore`：最大得分
  - `int prevIndex`：前置节点索引
  - `int continuousCount`：连续匹配计数

- `struct Max_Point`：记录最大得分信息
  - `double maxScore`：最大得分
  - `int maxScoreIndex`：最大得分对应的索引
  - `int prevIndex`：前置节点索引

#### 方法

- `void prepAnalyze()`：预处理函数，初始化数据结构
- `bool isEqual(int i, int j)`：检查query[i]与reference[j]是否匹配（正向）
- `bool isMatch(int i, int j)`：检查query[i]与reference[j]是否匹配（反向）
- `void analyzeRoute()`：动态规划分析最优匹配路径
- `void analyzeRepeats()`：分析重复序列片段
- `void printResults()`：按照规定格式输出所有重复序列信息

## 使用示例

```cpp
#include "repeat.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    Repeater analyzer;
    
    // 设置参考序列和查询序列
    analyzer.reference = "ACGTACGTACGT";
    analyzer.query = "ACGTACGT";
    
    // 执行分析流程
    analyzer.prepAnalyze();     // 预处理
    analyzer.analyzeRoute();    // 分析最优路径
    analyzer.analyzeRepeats();  // 分析重复序列
    
    // 输出结果
    analyzer.printResults();
    
    return 0;
}
```

## 分析算法说明

程序使用动态规划算法来识别重复序列。分析过程包括：

1. 预处理阶段：计算序列哈希值，初始化数据结构
2. 匹配分析阶段：对每个位置计算最佳匹配得分，考虑正向和反向匹配
3. 路径回溯阶段：从最优得分开始回溯，识别连续的重复片段
4. 结果整合阶段：合并相似的重复片段，计算重复次数

## 参数设置

算法使用以下参数来调整匹配策略：

- `maintainScore = 50`：保持正向匹配的得分
- `newScore = -10`：开始新序列的惩罚得分
- `maintainRevScore = 49`：保持反向匹配的得分
- `newRevScore = -10`：开始新反向序列的惩罚得分
- `continuousScore = 1`：连续匹配的额外奖励得分

通过调整这些参数，可以控制算法对不同类型匹配的偏好程度。
