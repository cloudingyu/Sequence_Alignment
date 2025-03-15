#include <iostream>
using namespace std;

string Reference, Query;
void scan();

int main()
{
    scan();
    cout<<Reference<<endl;
    cout<<Query<<endl;
    return 0;
}

void scan()
{
    freopen("reference.in", "r", stdin);
    cin >> Reference;
    fclose(stdin);
    freopen("query.in", "r", stdin);
    cin >> Query;
    fclose(stdin);
}