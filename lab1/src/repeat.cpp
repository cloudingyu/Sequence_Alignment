#include "repeat.h"
#include <iostream>
#include <iomanip>
using namespace std;

void test_Data(Repeat_Collection & repeatCollection) 
{
    Repeat_Segment segment1;
    segment1.sequence = "C";
    segment1.location = 400;
    segment1.length = 50;
    segment1.repetitionCount = 4;
    segment1.isReversed = false;
    repeatCollection.segments.push_back(segment1);
    
    Repeat_Segment segment2;
    segment2.sequence = "BC";
    segment2.location = 400;
    segment2.length = 70;
    segment2.repetitionCount = 3;
    segment2.isReversed = false;
    repeatCollection.segments.push_back(segment2);
    
    Repeat_Segment segment3;
    segment3.sequence = "ABC";
    segment3.location = 400;
    segment3.length = 100;
    segment3.repetitionCount = 2;
    segment3.isReversed = true;
    repeatCollection.segments.push_back(segment3);
    
    repeatCollection.count = repeatCollection.segments.size();
}

void Repeat_Collection::printResults() 
{
    cout << endl << endl;
    cout << setw(5) << "No." << " | Repetition location(ref) | Sequence length | Repetition times | Reverse" << endl;
    
    for(int i = 0; i < segments.size(); i++) {
        cout << setw(4) << i+1 << setw(15);
        cout << segments[i].location << setw(25);
        cout << segments[i].length << setw(18);
        cout << segments[i].repetitionCount << setw(17);
        if(segments[i].isReversed)
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
    cout << endl << endl;
}

void Repeat_Collection::analyzeRepeats(string reference, string query) 
{
    test_Data(*this);
    return ;
}