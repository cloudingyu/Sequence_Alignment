# repeat.h && repeat.cpp API 文档

本文档介绍了重复序列分析模块的接口和使用方法。该模块用于分析参考序列和查询序列中的重复片段。

## 类结构

### Repeat_Segment 类

表示一个重复序列片段的详细信息。

#### 成员变量

- `string sequence`：重复的序列字符串
- `int location`：在参考序列中的位置
- `int length`：序列的长度
- `int repetitionCount`：重复次数
- `bool isReversed`：是否为反向重复

### Repeat_Collection 类

用于收集和处理多个重复序列片段的容器类。

#### 成员变量

- `vector<Repeat_Segment> segments`：存储所有发现的重复序列片段
- `int count`：重复序列片段的数量

#### 成员函数

##### `void printResults()`

以表格形式打印所有重复序列片段的信息。

- **参数**：无
- **返回值**：无
- **输出格式**：表格形式展示每个重复序列的位置、长度、重复次数以及是否反向

##### `void analyzeRepeats(string reference, string query)`

分析参考序列和查询序列，找出其中的重复片段。

- **参数**：
  - `reference`：参考序列字符串
  - `query`：查询序列字符串
- **返回值**：无
- **效果**：将找到的重复序列片段添加到 `segments` 向量中，并更新 `count` 值

## 使用示例

```cpp
#include "repeat.h"

int main() 
{
    // 读取参考序列和查询序列
    string reference = "ACGTACGTACGT";
    string query = "ACGTACGT";
    
    // 创建重复序列集合对象
    Repeat_Collection results;
    
    // 分析序列中的重复
    results.analyzeRepeats(reference, query);
    
    // 打印分析结果
    results.printResults();
    
    return 0;
}
```

## 输出示例

分析结果将以下面的格式输出：

```
  No. | Repetition location(ref) | Sequence length | Repetition times | Reverse
   1             400                     50                 4              False
   2             400                     70                 3              False
   3             400                    100                 2               True
```


