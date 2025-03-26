#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
using namespace std;

// 参数: 维持, 新开, 反向维持, 反向新开, 连续得分
const double maintainScore = 50, newScore = -10, maintainRevScore = 49, newRevScore = -10, continuousScore = 1;

// 预处理函数
void Repeater::prepAnalyze()
{
    refeLength = reference.length() - kmer_size + 1;
    querLength = query.length() - kmer_size + 1;
    refeHash = rollingHash(reference);
    querHash = rollingHash(query);
    refeRevHash = rollingHash(reverseStr(complementStr(reference)));

    for (int i = 0; i < querLength; i++)
    {
        vector<Align_Point> temp;
        for (int j = 0; j < refeLength; j++)
            temp.push_back({MIN_INF, -1, 0});
        Align.push_back(temp);
    }
    Align[0][0] = {0, -1, 1};

    for (int i = 0; i < querLength; i++)
        querAlign.push_back({MIN_INF, -1, -1});
    querAlign[0] = {0, 0, -1};

    for (int i = 0; i < querLength; i++)
    {
        vector<int> temp;
        for (int j = 0; j < refeLength; j++)
            temp.push_back(MIN_INF);
        Route.push_back(temp);
    }
    for (int i = 0; i < querLength; i++)
        pointRoute.push_back({0, -1, 0});
    return;
}

// 比对函数
bool Repeater::isEqual(int i, int j)
{
    return querHash[i][i + kmer_size - 1] == refeHash[j][j + kmer_size - 1];
}

// 比对函数
bool Repeater::isMatch(int i, int j)
{
    return querHash[i][i + kmer_size - 1] == refeRevHash[refeLength - j - 1][refeLength - j + kmer_size - 2];
}

// 分析重复序列
void Repeater::analyzeRoute()
{

    // 主循环,遍历每一个 query 碱基可以匹配的正向和反向 reference
    for (int i = 1; i < querLength; i++)
        for (int j = 0; j < refeLength; j++)
        {
            // 考虑正向匹配
            if (isEqual(i, j))
            {
                // 新开序列得分
                Align[i][j] = {querAlign[i - 1].maxScore + maintainScore + newScore, querAlign[i - 1].maxScoreIndex, 1};

                // 继续延续得分
                if (j - 1 >= 0 && isEqual(i - 1, j - 1))
                    if (Align[i][j].maxScore < Align[i - 1][j - 1].maxScore + maintainScore + Align[i - 1][j - 1].continuousCount * continuousScore)
                        Align[i][j] = {Align[i - 1][j - 1].maxScore + maintainScore + Align[i - 1][j - 1].continuousCount * continuousScore, j - 1, Align[i - 1][j - 1].continuousCount + 1};

                if (querAlign[i].maxScore < Align[i][j].maxScore)
                    querAlign[i] = {Align[i][j].maxScore, j, Align[i][j].prevIndex};
            }
            // 考虑反向匹配
            else if (isMatch(i, j))
            {
                // 新开序列得分
                Align[i][j] = {querAlign[i - 1].maxScore + maintainRevScore + newRevScore, querAlign[i - 1].maxScoreIndex, 1};

                // 继续延续得分
                if (j + 1 < refeLength && isMatch(i - 1, j + 1))
                    if (Align[i][j].maxScore < Align[i - 1][j + 1].maxScore + maintainRevScore + Align[i - 1][j + 1].continuousCount * continuousScore)
                        Align[i][j] = {Align[i - 1][j + 1].maxScore + maintainRevScore + Align[i - 1][j + 1].continuousCount * continuousScore, j + 1, Align[i - 1][j + 1].continuousCount + 1};

                if (querAlign[i].maxScore <= Align[i][j].maxScore)
                    querAlign[i] = {Align[i][j].maxScore, j, Align[i][j].prevIndex};
            }
        }
    return;
}

// 分析重复序列
void Repeater::analyzeRepeats()
{
    // 回溯得到最优路径
    int h = querLength - 1, l = refeLength - 1;
    while (h >= 0 && l >= 0)
    {
        Route[h][l] = Align[h][l].maxScore;
        pointRoute[h] = {Align[h][l].maxScore, l, Align[h][l].prevIndex};
        l = Align[h][l].prevIndex;
        h--;
    }

    // 通过 continuousCount 判断所有连续序列
    int head = 0;
    while (head < querLength)
    {
        int tail = head + 1;
        while (tail < querLength && abs(Align[tail][pointRoute[tail].maxScoreIndex].continuousCount) != 1)
            tail++;
        if (isMatch(head, pointRoute[head].maxScoreIndex))
        {
            Repeat_Segment temp(subStr(query, head, tail - 1), pointRoute[head].maxScoreIndex, tail - head, 1, isMatch(head, pointRoute[head].maxScoreIndex));
            segments.push_back(temp);
        }
        else
        {
            Repeat_Segment temp(subStr(query, head, tail - 1), pointRoute[head].maxScoreIndex + tail - head, tail - head, 1, isMatch(head, pointRoute[head].maxScoreIndex));
            segments.push_back(temp);
        }

        head = tail;
    }

    // 删去重复的片段(模糊匹配)
    for (int i = segments.size(); i > 0; i--)
        for (int j = 0; j < i; j++)
            if (abs(segments[i].location - segments[j].location) < 10 && abs(segments[i].length - segments[j].length) < 10 && segments[i].isReversed == segments[j].isReversed)
            {
                segments[j].repetitionCount++;
                segments.erase(segments.begin() + i);
                break;
            }
    for (int i = segments.size() - 1; i >= 0; i--)
    {
        if (segments[i].repetitionCount == 1)
        {
            segments.erase(segments.begin() + i);
        }
    }
    return ;
}

// 按照规定格式输出所有重复序列信息
void Repeater::printResults()
{
    cout << setw(5) << "No." << " | Repetition location(ref) | Sequence length | Repetition times | Reverse" << endl;

    for (int i = 0; i < segments.size(); i++)
    {
        cout << setw(4) << i + 1 << setw(15);
        cout << segments[i].location << setw(25);
        cout << segments[i].length << setw(18);
        cout << segments[i].repetitionCount << setw(17);
        if (segments[i].isReversed)
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
}