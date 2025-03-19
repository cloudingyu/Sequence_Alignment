#include"repeat.h"
#include<iostream>
#include<iomanip>
using namespace std;

Repeat_All_Information Repeat_Test()
{
    Repeat_All_Information repeatAnswer;
    
    repeatAnswer.Repeat_Information_Number = 3;
    
    Repeat_Information info1;
    info1.Repeat_String = "C";
    info1.Repeat_Location = 400;
    info1.Repeat_Length = 50;
    info1.Repeat_Times = 4;
    info1.Repeat_Whether_Reverse = false;
    repeatAnswer.Repeat_Information.push_back(info1);
    
    Repeat_Information info2;
    info2.Repeat_String = "BC";
    info2.Repeat_Location = 400;
    info2.Repeat_Length = 70;
    info2.Repeat_Times = 3;
    info2.Repeat_Whether_Reverse = false;
    repeatAnswer.Repeat_Information.push_back(info2);
    
    Repeat_Information info3;
    info3.Repeat_String = "ABC";
    info3.Repeat_Location = 400;
    info3.Repeat_Length = 100;
    info3.Repeat_Times = 2;
    info3.Repeat_Whether_Reverse = true;
    repeatAnswer.Repeat_Information.push_back(info3);

    return repeatAnswer;
}

Repeat_All_Information Repeat_Process(string reference, string query)
{
    Repeat_All_Information repeatAnswer = Repeat_Test();
    
    
    return repeatAnswer;
}

void Print_Repeat_Information(Repeat_All_Information repeatAnswer)
{
    cout << endl << endl;
    cout << setw(5) << "No." << " | Repetition location(ref) | Sequence length | Repetition times | Reverse" << endl;
    
    for(int i = 0; i < repeatAnswer.Repeat_Information.size(); i++)
    {
        cout << setw(4) << i+1 << setw(15);
        cout << repeatAnswer.Repeat_Information[i].Repeat_Location << setw(25);
        cout << repeatAnswer.Repeat_Information[i].Repeat_Length << setw(18);
        cout << repeatAnswer.Repeat_Information[i].Repeat_Times << setw(17);
        if(repeatAnswer.Repeat_Information[i].Repeat_Whether_Reverse)
            cout << "True" << endl;
        else
            cout << "False" << endl;
    }
    cout << endl << endl;
}