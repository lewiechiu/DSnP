#include <iostream>
#include <fstream>
#include "rnGen.h"
using namespace std;
#define maxNum 1000000
#define mtpS 1000
#define mtdS 500
#define mtrS 35000
int main() 
{
    ofstream out("td1");

    RandomNumGen rnGen;
    cout << rnGen(maxNum) << endl;
    
    for(int TDsize = 0;TDsize < 10000; TDsize++)
    {
        int RAPRAM = rnGen(maxNum);
        if(RAPRAM%6== 0 || RAPRAM%6== 1)
        {
            if(RAPRAM % 2 == 1)
            {
                out << "mtn " << rnGen(mtpS) << endl;
            }
            else
            {
                out << "mtn -a " << rnGen(mtpS) << endl;
            }
            //mtn 
            
        }
        else if(RAPRAM % 6 == 3 || RAPRAM % 6 == 2)
        {
            if(RAPRAM % 5 == 3)
            {
                out << "mtd -r " << rnGen(mtdS);
                if(RAPRAM % 2 == 1)
                    out << " -a" << endl;
                else
                    out << endl;
            }
            else if(RAPRAM % 5 == 2 || RAPRAM % 5 == 1)
            {
                out << "mtd -i " << rnGen(mtdS);
                if(RAPRAM % 2 == 1)
                    out << " -a" << endl;
                else
                    out << endl;
            }
            //mtd
        }
        else if(RAPRAM % 9 == 8 )
        {
            out << "mtr " << rnGen(mtrS) << endl;
            //mtr
        }
    }
    
    
    return 0;
}