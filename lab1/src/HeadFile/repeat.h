#ifndef REPEAT_H
#define REPEAT_H

#include<iostream>
using namespace std;


class Repeat_Information
{
    public:
        string Repeat_String;
        int Repeat_Location;
        int Repeat_Length;
        int Repeat_Times;
        bool Repeat_Whether_Reverse;
};

class Repeat_All_Information
{
    public:
        Repeat_Information *Repeat_Information;
        int Repeat_Information_Number = 0;
};

Repeat_All_Information Repeat_Process(string reference,string query);

void Print_Repeat_Information(Repeat_All_Information repeatAnswer);

#endif