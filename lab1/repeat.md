# 重复序列分析器 API 文档

## 概述

该程序用于识别和分析DNA序列中的重复片段。通过比较参考序列(reference)和查询序列(query)，程序能够找出相似或重复的DNA片段，包括正向和反向重复序列，并提供重复位置、长度和重复次数等信息，并且可以生成序列比对的可视化图像。

## 类结构

### `Repeat_Segment` 类

表示单个重复序列片段的信息。

#### 属性

- `string sequence`：重复序列的碱基序列
- `int location`：重复序列在参考序列中的起始位置
- `int length`：重复序列的长度
- `int repetitionCount`：重复次数
- `bool isReversed`：是否为反向重复序列
- `int begin`：序列在查询序列中的起始位置
- `int endd`：序列在查询序列中的结束位置

#### 构造函数

- `Repeat_Segment()`：默认构造函数，初始化所有属性为默认值
- `Repeat_Segment(const string &seq, int loc, int len, int cnt, bool rev, int beg, int end)`：带参构造函数，用于创建完整的重复序列对象

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
- `vector<vector<int>> Route`：记录最优选择路径

#### 辅助数据结构

- `struct Align_Point`：记录比对信息
  - `double maxScore`：最大得分
  - `int prevIndex`：前置节点索引
  - `int continuousCount`：连续匹配计数

- `struct Max_Point`：记录最大得分信息
  - `double maxScore`：最大得分
  - `int maxScoreIndex`：最大得分对应的索引
  - `int prevIndex`：前置节点索引

- `vector<vector<Align_Point>> Align`：存储每个查询位置与参考位置的匹配信息
- `vector<Max_Point> querAlign`：存储每个查询位置的最优匹配信息
- `vector<Max_Point> pointRoute`：存储回溯路径信息

#### 方法

- `void prepAnalyze()`：预处理函数，初始化数据结构和计算哈希值
- `bool isEqual(int i, int j)`：检查query[i]与reference[j]是否匹配（正向）
- `bool isMatch(int i, int j)`：检查query[i]与reference[j]是否匹配（反向）
- `void analyzeRoute()`：动态规划分析最优匹配路径
- `void analyzeSequence()`：提取连续子串，形成重复片段
- `void eraseSequence()`：删去与原字符串相同的部分，合并相同子串(模糊处理)
- `void drawSequence()`：绘制比对演示图像，输出Python脚本
- `void printResults()`：按照规定格式输出所有重复序列信息

## 使用示例

```cpp
#include "repeat.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string reference, query;

    // 读取序列数据
    ifstream refFile("reference.in");
    if (refFile.is_open()) {
        getline(refFile, reference);
        refFile.close();
    }
    
    ifstream queryFile("query.in");
    if (queryFile.is_open()) {
        getline(queryFile, query);
        queryFile.close();
    }
    
    // 定义并初始化重复序列处理器
    Repeater results;
    results.reference = reference;
    results.query = query;
    results.kmer_size = 1;
    
    // 执行分析流程
    results.prepAnalyze();      // 预处理
    results.analyzeRoute();     // 分析最优路径
    results.analyzeSequence();  // 提取连续子串
    results.drawSequence();     // 绘制比对演示图像
    results.eraseSequence();    // 模糊处理
    results.printResults();     // 输出结果
    
    return 0;
}
```

## 分析算法说明

程序使用动态规划算法来识别重复序列。分析过程包括：

1. 预处理阶段：计算序列哈希值，初始化数据结构
2. 匹配分析阶段：对每个位置计算最佳匹配得分，考虑正向和反向匹配
   - 对每一个查询序列位置i，枚举参考序列所有位置j
   - 计算正向匹配和反向匹配的得分，选择最优方案
3. 路径回溯阶段：从最优得分开始回溯，识别连续的重复片段
4. 结果整合阶段：合并相似的重复片段，计算重复次数
5. 可视化阶段：生成序列比对的可视化图像，直观展示匹配结果

## 参数设置

算法使用以下参数来调整匹配策略：

- `maintainScore = 50`：保持正向匹配的得分
- `newScore = -10`：开始新序列的惩罚得分
- `maintainRevScore = 49`：保持反向匹配的得分（略低于正向匹配，优先选择正向）
- `newRevScore = -10`：开始新反向序列的惩罚得分
- `continuousScore = 1`：连续匹配的额外奖励得分，鼓励更长的连续匹配

通过调整这些参数，可以控制算法对不同类型匹配的偏好程度。

## 输出结果

程序会生成两种类型的输出：

1. 控制台输出：以表格形式显示所有识别到的重复序列片段信息，包括位置、长度、重复次数和是否为反向重复
2. 可视化图像：生成名为`alignment_path.png`的序列比对可视化图像，其中：
   - 蓝色线段：表示连续匹配的序列片段
   - 红色线段：表示不连续区域之间的跳转
   - X轴：代表查询序列(Query)位置
   - Y轴：代表参考序列(Reference)位置
