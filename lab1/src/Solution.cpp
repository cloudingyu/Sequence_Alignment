#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct RepeatSegment {
    int position;   // 第一次额外重复在reference的位置
    int length;     // 重复片段长度
    int count;      // 重复数量
    bool isReverse; // 是否为反向重复
};

string Reference = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA";
string Query = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA";
int Contact_Map[2005][2005], Reference_Length, Query_Length;
vector<RepeatSegment> repeatResults;

int Match(char a, char b);
void Preparation();
void Print_Map();
void Build_Contact_Map();
void Find_Repeats();
bool IsComplementary(char a, char b);
string ReverseComplement(string s);
void Print_Results();

int main()
{
    Preparation();
    Build_Contact_Map();
    Find_Repeats();
    Print_Results();
    Print_Map();
    fclose(stdout);
    return 0;
}

void Preparation()
{
    freopen("Answer.out", "w", stdout);
    Reference_Length = Reference.length();
    Query_Length = Query.length();
    return;
}

void Build_Contact_Map()
{
    for (int i = 0; i < Query_Length; i++)
        for (int j = 0; j < Reference_Length; j++)
            Contact_Map[i][j] = Match(Query[i], Reference[j]);
}

int Match(char a, char b)
{
    if (a == b)
        return 1;
    if (a + b == 149 || a + b == 138)
        return 2;
    return 0;
}

bool IsComplementary(char a, char b)
{
    return (a == 'A' && b == 'T') || (a == 'T' && b == 'A') ||
           (a == 'C' && b == 'G') || (a == 'G' && b == 'C');
}

string ReverseComplement(string s)
{
    string result = s;
    reverse(result.begin(), result.end());
    for (char &c : result) {
        switch (c) {
            case 'A': c = 'T'; break;
            case 'T': c = 'A'; break;
            case 'G': c = 'C'; break;
            case 'C': c = 'G'; break;
        }
    }
    return result;
}

void Find_Repeats()
{
    // 在Query中寻找重复片段
    for (int refStart = 0; refStart < Reference_Length; refStart++) {
        for (int refEnd = refStart + 1; refEnd <= Reference_Length; refEnd++) {
            // 获取参考序列中的一个片段
            string segment = Reference.substr(refStart, refEnd - refStart);
            int segmentLength = segment.length();
            
            // 忽略空片段和长度小于等于 20 的片段
            if (segmentLength <= 20) continue;
            
            // 在Query中寻找该片段的第一次出现
            size_t firstOccurrence = Query.find(segment);
            if (firstOccurrence != string::npos) {
                // 检查正向重复
                int repeatCount = 0;
                size_t pos = firstOccurrence + segmentLength;
                
                while (pos + segmentLength <= Query_Length && 
                       Query.substr(pos, segmentLength) == segment) {
                    repeatCount++;
                    pos += segmentLength;
                }
                
                if (repeatCount > 0) {
                    RepeatSegment rs;
                    rs.position = refEnd; // 第一次额外重复在reference的位置
                    rs.length = segmentLength;
                    rs.count = repeatCount;
                    rs.isReverse = false;
                    repeatResults.push_back(rs);
                }
                
                // 检查第一个片段后面是否有反向重复
                string revComp = ReverseComplement(segment);
                repeatCount = 0;
                pos = firstOccurrence + segmentLength;
                
                while (pos + segmentLength <= Query_Length && 
                       Query.substr(pos, segmentLength) == revComp) {
                    repeatCount++;
                    pos += segmentLength;
                }
                
                if (repeatCount > 0) {
                    RepeatSegment rs;
                    rs.position = refEnd; // 第一次额外重复在reference的位置
                    rs.length = segmentLength;
                    rs.count = repeatCount;
                    rs.isReverse = true;
                    repeatResults.push_back(rs);
                }
            }
        }
    }
}

void Print_Results()
{
    cout << "重复片段分析结果：" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "| 序号 | 位置 | 长度 | 重复数量 | 是否反向 |" << endl;
    cout << "------------------------------------------------" << endl;
    
    for (size_t i = 0; i < repeatResults.size(); i++) {
        cout << "| " << setw(4) << (i + 1) << " | "
             << setw(4) << repeatResults[i].position << " | "
             << setw(4) << repeatResults[i].length << " | "
             << setw(8) << repeatResults[i].count << " | "
             << setw(8) << (repeatResults[i].isReverse ? "是" : "否") << " |" << endl;
    }
    
    cout << "------------------------------------------------" << endl << endl;
}

void Print_Map()
{
    for (int i = Reference_Length - 1; i >= 0; i--)
    {
        cout << setw(3) << i << setw(3) << Reference[i];
        for (int j = 0; j < Query_Length; j++)
        {
            if (Contact_Map[j][i] == 1)
                cout << setw(4) << "*";
            else if (Contact_Map[j][i] == 2)
                cout << setw(4) << "~";
            else
                cout << setw(4) << " ";
        }
        cout << endl;
    }
    cout << "      ";
    for (int i = 0; i < Query_Length; i++)
        cout << setw(4) << Query[i];
    cout << endl
         << "      ";
    for (int i = 0; i < Query_Length; i++)
        cout << setw(4) << i;
    cout << endl;
    return;
}