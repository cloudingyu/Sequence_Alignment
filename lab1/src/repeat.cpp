#include "repeat.h"
#include "stringProcess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
using namespace std;

// 参数: 维持, 新开, 反向维持, 反向新开
const int alignScore = 1, newScore = -5, alignRevScore = 9, newRevScore = -1;

const int MIN_INF = -2147483648;

// 记录重复信息
void Repeat_Segment::recordSegment(string const &seq, int loc, int len, int cnt, bool rev)
{
    sequence = seq;
    location = loc;
    length = len;
    repetitionCount = cnt;
    isReversed = rev;
}

// 按照规定格式输出所有重复序列信息
void Repeat_Collection::printResults()
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

// 分析重复序列
void Repeat_Collection::analyzeRepeats(const string &reference, const string &query, int kmer_size)
{

    // 记录哈希简化后的长度
    int refeLength = reference.length() - kmer_size + 1;
    int querLength = query.length() - kmer_size + 1;

    // 计算 reference 串和 query 串的哈希值
    vector<long long> refeHash = rollingHash(reference, kmer_size);
    vector<long long> querHash = rollingHash(query, kmer_size);

    // 计算 reference 串的反向互补哈希值
    vector<long long> refeRevHash = rollingRevHash(reference, kmer_size);

    // 动态规划数组初始化
    vector<vector<long long>> scoreMap(querLength, vector<long long>(refeLength, MIN_INF));
    vector<vector<pair<int, int>>> prevMap(querLength, vector<pair<int, int>>(refeLength, {-1, -1}));

    // 记录第 i 个碱基对所对应的得分最大值
    vector<pair<long long, pair<int, int>>> maxScoreMap(querLength, {MIN_INF, {0, 0}});

    vector<int> maxScoreIndex(querLength, 0);

    // 初始化 query 第 [0] 个碱基的匹配情况
    scoreMap[0][0] = alignScore;
    prevMap[0][0] = {-1, -1};
    maxScoreIndex[0] = 0;

    // 主循环,遍历每一个 query 碱基可以匹配的正向和反向 reference
    for (int i = 1; i < querLength; i++)
        for (int j = 1; j < refeLength; j++)
        {
            // 考虑正向匹配
            if (querHash[i] == refeHash[j])
            {
                // 新开序列得分
                scoreMap[i][j] = maxScoreMap[i - 1].first + alignScore + newScore ;
                prevMap[i][j] = make_pair(i - 1, maxScoreIndex[i - 1]);

                // 继续延续得分
                if (scoreMap[i - 1][j - 1] + alignScore >= scoreMap[i][j] && refeHash[j - 1] == querHash[i - 1])
                {
                    scoreMap[i][j] = scoreMap[i - 1][j - 1] + alignScore;
                    prevMap[i][j] = {i - 1, j - 1};
                }
                if (maxScoreMap[i].first < scoreMap[i][j])
                {
                    maxScoreMap[i] = {scoreMap[i][j], prevMap[i][j]};
                    maxScoreIndex[i] = j;
                }
            }
            /*
            // 考虑反向匹配
            else if (querHash[i] == refeRevHash[j])
            {
                scoreMap[i][j] = maxScoreMap[i - 1].first + alignRevScore + newRevScore - abs(j - maxScoreIndex[i - 1]);
                prevMap[i][j] = make_pair(i - 1, maxScoreIndex[i - 1]);

                if (scoreMap[i - 1][j + 1] + alignRevScore >= scoreMap[i][j] && refeRevHash[j + 1] == querHash[i - 1])
                {
                    scoreMap[i][j] = scoreMap[i - 1][j + 1] + alignRevScore;
                    prevMap[i][j] = {i - 1, j + 1};
                }
                if (maxScoreMap[i].first <= scoreMap[i][j])
                {
                    maxScoreMap[i] = {scoreMap[i][j], prevMap[i][j]};
                    maxScoreIndex[i] = j;
                }
            }
            */
        }
    cout << "equalMap" << endl;
    for (int i = refeLength - 1; i >= 0; i--)
    {
        for (int j = 0; j < querLength; j++)
            cout << setw(3) << ((refeHash[i] == querHash[j]) ? '*' : ' ');
        cout << endl;
    }
    cout << "complementMap" << endl;
    for (int i = refeLength - 1; i >= 0; i--)
    {
        for (int j = 0; j < querLength; j++)
            cout << setw(3) << ((refeRevHash[i] == querHash[j]) ? '*' : ' ');
        cout << endl;
    }

    cout << "scoreMap" << endl;
    for (int i = refeLength - 1; i >= 0; i--)
    {
        for (int j = 0; j < querLength; j++)
            cout << setw(12) << scoreMap[j][i] << " ";
        cout << endl;
    }

    for (int i = 0; i < querLength; i++)
        cout << i << " " << maxScoreMap[i].first << " " << maxScoreMap[i].second.first << " " << maxScoreMap[i].second.second << endl;

    cout << "map" << endl;
    
    vector<vector<int>> map(querLength, vector<int>(refeLength, 0));
    for (int i = 0; i < querLength - 1; i++)
        map[maxScoreMap[i + 1].second.first][maxScoreMap[i + 1].second.second] = 1;
    map[querLength - 1][refeLength - 1] = 1;
    for (int i = refeLength - 1; i >= 0; i--)
    {
        for (int j = 0; j < querLength; j++)
        {
            if (map[j][i] == 1)
                cout << setw(3) << "0";
            else
                cout << setw(3) << " ";
        }
        cout << endl;
    }
    return;
}