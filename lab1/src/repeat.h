#ifndef REPEAT_H
#define REPEAT_H

#include<iostream>
#include<vector>
using namespace std;


class Repeat_Segment
{
    public:
        string sequence;
        int location;
        int length;
        int repetitionCount;
        bool isReversed;
};

class Repeat_Collection 
{
    public:
        vector<Repeat_Segment> segments;
        int count = 0;
    public:
        void printResults();
        void analyzeRepeats(string reference, string query);
};

#endif