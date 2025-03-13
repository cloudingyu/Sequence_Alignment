#include <iostream>
#include <iomanip>
using namespace std;
string Reference = "ATGCTCCC";
string Query = "ATGCTCCTCCC";
int Contact_Map[2005][2005], Reference_Length, Query_Length;
int Match(char a, char b);
void Preparation();
void Print_Map();
void Build_Contact_Map();

int main()
{
    Preparation();
    Build_Contact_Map();
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
                cout << setw(4) << "&";
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