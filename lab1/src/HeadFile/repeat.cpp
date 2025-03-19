#include"repeat.h"
#include<iostream>
#include<iomanip>
using namespace std;

Repeat_All_Information Repeat_Test()
{
    Repeat_All_Information repeatAnswer;
    repeatAnswer.Repeat_Information_Number = 3;
    repeatAnswer.Repeat_Information[0].Repeat_String = "C";
    repeatAnswer.Repeat_Information[0].Repeat_Location = 400;
    repeatAnswer.Repeat_Information[0].Repeat_Length = 50;
    repeatAnswer.Repeat_Information[0].Repeat_Times = 4;
    repeatAnswer.Repeat_Information[0].Repeat_Whether_Reverse = false;

    repeatAnswer.Repeat_Information[1].Repeat_String = "BC";
    repeatAnswer.Repeat_Information[1].Repeat_Location = 400;
    repeatAnswer.Repeat_Information[1].Repeat_Length = 70;
    repeatAnswer.Repeat_Information[1].Repeat_Times = 3;
    repeatAnswer.Repeat_Information[1].Repeat_Whether_Reverse = false;

    repeatAnswer.Repeat_Information[2].Repeat_String = "ABC";
    repeatAnswer.Repeat_Information[2].Repeat_Location = 400;
    repeatAnswer.Repeat_Information[2].Repeat_Length = 100;
    repeatAnswer.Repeat_Information[2].Repeat_Times = 2;
    repeatAnswer.Repeat_Information[2].Repeat_Whether_Reverse = true;

    return repeatAnswer;
}

Repeat_All_Information Repeat_Process(string reference,string query)
{
    Repeat_All_Information repeatAnswer=Repeat_Test();



    return repeatAnswer;
}

void Print_Repeat_Information(Repeat_All_Information repeatAnswer)
{
    cout<<endl<<endl;
    cout<<setw(5)<<"No."<<" | Repetition location(ref) | Sequence length | Repetition times | Reverse"<<endl;
    for(int i=0;i<repeatAnswer.Repeat_Information_Number;i++)
    {
        cout<<setw(4)<<i+1<<setw(15);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Location<<setw(22);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Length<<setw(18);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Times<<setw(17);
        if(repeatAnswer.Repeat_Information[i].Repeat_Whether_Reverse)
            cout<<"True"<<endl;
        else
            cout<<"False"<<endl;
    }
    cout<<endl<<endl;
}