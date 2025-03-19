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
    cout<<setw(3)<<"序号 | 额外重复出现位置(ref) | 序列长度 | 重复次数 | 是否反向"<<endl;
    for(int i=0;i<repeatAnswer.Repeat_Information_Number;i++)
    {
        cout<<setw(4)<<i+1<<setw(15);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Location<<setw(15);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Length<<setw(10);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Times<<setw(10);
        cout<<repeatAnswer.Repeat_Information[i].Repeat_Whether_Reverse?true:false<<endl;
    }
}