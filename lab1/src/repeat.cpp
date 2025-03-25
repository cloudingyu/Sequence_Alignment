#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
using namespace std;

// 参数: 维持, 新开, 反向维持, 反向新开
const double alignScore = 5, newScore = -1, alignRevScore = 5, newRevScore = -1, continousScore = 1;

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
    Align[0][0] = {0, -1,1};

    for (int i = 0; i < querLength; i++)
        querAlign.push_back({MIN_INF, -1, -1});
    querAlign[0] = {0, 0, -1};

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
    return querHash[i][i + kmer_size - 1] == refeRevHash[refeLength - j - kmer_size][refeLength - j - 1];
}

// 分析重复序列
void Repeater::analyzeRoute()
{

    // 主循环,遍历每一个 query 碱基可以匹配的正向和反向 reference
    for (int i = 1; i < querLength; i++)
        for (int j = 1; j < refeLength; j++)
        {
            // 考虑正向匹配
            if (isEqual(i, j))
            {
                // 新开序列得分
                Align[i][j] = {querAlign[i - 1].maxScore + alignScore + newScore, querAlign[i - 1].maxScoreIndex, 1};

                // 继续延续得分
                if (isEqual(i - 1, j - 1))
                    if (Align[i][j].maxScore < Align[i - 1][j - 1].maxScore + alignScore + Align[i - 1][j - 1].continuousCount * continousScore)
                    {
                        Align[i][j] = {Align[i - 1][j - 1].maxScore + alignScore+ Align[i - 1][j - 1].continuousCount * continousScore, j - 1};
                        Align[i][j].continuousCount = Align[i - 1][j - 1].continuousCount + 1;
                    }
                if (querAlign[i].maxScore < Align[i][j].maxScore)
                    querAlign[i] = {Align[i][j].maxScore, j, Align[i][j].prevIndex};
            }
            // 考虑反向匹配
            else if (isMatch(i, j))
            {
                // 新开序列得分
                Align[i][j] = {querAlign[i - 1].maxScore + alignRevScore + newRevScore, querAlign[i - 1].maxScoreIndex, 1};

                // 继续延续得分
                if (j + 1 < refeLength && isMatch(i - 1, j + 1))
                    if (Align[i][j].maxScore < Align[i - 1][j + 1].maxScore + alignRevScore + Align[i - 1][j + 11].continuousCount * continousScore)
                    {
                        Align[i][j] = {Align[i - 1][j + 1].maxScore + alignRevScore+ Align[i - 1][j + 1].continuousCount * continousScore, j + 1};
                        Align[i][j].continuousCount = Align[i - 1][j + 1].continuousCount + 1;
                    }
                if (querAlign[i].maxScore <= Align[i][j].maxScore)
                    querAlign[i] = {Align[i][j].maxScore, j, Align[i][j].prevIndex};
            }
            // 重新计入 querAlign[i] 的最大得分

        }
    return;
}

// 分析重复序列
void Repeater::analyzeRepeats()
{
    for (int j = refeLength - 1; j >= 0; j--)
    {
        for (int i = 0; i < querLength; i++)
            if (Align[i][j].maxScore == MIN_INF)
                cout << setw(4) << '_';
            else
                cout << setw(4) << Align[i][j].maxScore;
        cout << endl;
    }

    vector<vector<int>> Route(querLength, vector<int>(refeLength, -1));
    int h = querLength - 1, l = refeLength - 1;
    while (h >= 0 && l >= 0)
    {

        Route[h][l] = Align[h][l].maxScore;
        l = Align[h][l].prevIndex;
        h--;
    }
    for (int j = refeLength - 1; j >= 0; j--)
    {
        for (int i = 0; i < querLength; i++)
            if (Route[i][j] == -1)
                cout << setw(4) << '_';
            else
                cout << setw(4) << Route[i][j];
        cout << endl;
    }

    for (int j = refeLength - 1; j >= 0; j--)
    {
        for (int i = 0; i < querLength; i++)
            if (Route[i][j] == -1)
                cout << "  ";
            else
                cout << " 0";
        cout << endl;
    }


    return;
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