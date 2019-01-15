#include <iostream>
using namespace std;

int main() 
{
    size_t a = 128;
    cout << sizeof(a) << endl;
    for(int i=0;i<sizeof(a)*8;i++)
    {
        cout << ((a >> i) & 1U);
    }
    return 0;
}