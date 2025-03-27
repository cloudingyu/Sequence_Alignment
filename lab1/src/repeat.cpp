#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <fstream>
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

// 提取连续子串
void Repeater::analyzeSequence()
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
            Repeat_Segment temp(
                subStr(query, head, tail - 1),
                pointRoute[head].maxScoreIndex,
                tail - head,
                1,
                isMatch(head, pointRoute[head].maxScoreIndex),
                head,
                tail - 1);
            segments.push_back(temp);
        }
        else
        {
            Repeat_Segment temp(
                subStr(query, head, tail - 1),
                pointRoute[head].maxScoreIndex + tail - head,
                tail - head,
                1,
                isMatch(head, pointRoute[head].maxScoreIndex),
                head,
                tail - 1);
            segments.push_back(temp);
        }

        head = tail;
    }

    return;
}

// 删去与原字符串相同的部分, 合并相同子串(模糊处理)
void Repeater::eraseSequence()
{
    // 删去与原字符串相同的部分
    vector<vector<int>> refeRoute(querLength, vector<int>(refeLength, 0));
    for (int j = refeLength - 1; j >= 0; j--)
    {
        int h = 0;
        while (Route[h][j] == MIN_INF)
            h++;
        refeRoute[h][j] = 1;
        continue;
    }
    for (int i = segments.size() - 1; i >= 0; i--)
        for (int j = segments[i].endd; j >= segments[i].begin; j--)
            if (refeRoute[j][querAlign[j].maxScoreIndex] == 1)
                segments[i].length = segments[i].length - 1;

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
        if (segments[i].length < 10)
            segments.erase(segments.begin() + i);
    return;
}

// 绘制比对演示图像
void Repeater::drawSequence()
{
    ofstream pyfile("../src/drawer.py");

    if (!pyfile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        return;
    }

    pyfile << "import matplotlib.pyplot as plt" << endl;
    pyfile << "import numpy as np" << endl;
    pyfile << "plt.figure(figsize=("<<8*querLength/refeLength<<", 8))" << endl;

    for (int i = 0; i < segments.size(); i++)
    {
        pyfile << "plt.plot([";
        pyfile << segments[i].begin << "," << segments[i].endd;
        pyfile << "],[";
        pyfile << pointRoute[segments[i].begin].maxScoreIndex << "," << pointRoute[segments[i].endd].maxScoreIndex;
        pyfile << "], linewidth=2.5, color='blue')" << endl;

        if (i == segments.size() - 1)
            break;

        pyfile << "plt.plot([";
        pyfile << segments[i].endd << "," << segments[i + 1].begin;
        pyfile << "],[";
        pyfile << pointRoute[segments[i].endd].maxScoreIndex << "," << pointRoute[segments[i + 1].begin].maxScoreIndex;
        pyfile << "], linewidth=1, color='red')" << endl;
    }

    pyfile << "plt.xlabel('Query', fontsize=14)" << endl;
    pyfile << "plt.ylabel('Reference', fontsize=14)" << endl;
    pyfile << "plt.title('DNA Sequence Alignment', fontsize=16)" << endl; // 修正拼写错误
    pyfile << "plt.savefig('alignment.png', dpi=300)" << endl;
    pyfile << "plt.show()" << endl;

    pyfile.close();
}

// 按照规定格式输出所有重复序列信息
void Repeater::printResults()
{

    cout << setw(5) << "index" << " | POS in REF | Repeat size | Repeat count | Inverse" << endl;

    for (int i = 0; i < segments.size(); i++)
    {
        cout << setw(5) << i + 1 << setw(13);
        cout << segments[i].location << setw(14);
        cout << segments[i].length << setw(15);
        cout << segments[i].repetitionCount << setw(10);
        if (segments[i].isReversed)
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
    return;
}