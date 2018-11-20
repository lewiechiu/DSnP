#include <iostream>
#include <fstream>
#include "rnGen.h"
using namespace std;
#define maxNum 1000000
#define mtpS 14000
#define mtdS 3000
#define mtrS 7000000
int main() 
{
    ofstream out("td1");

    RandomNumGen rnGen;
    cout << rnGen(maxNum) << endl;
    cout << "this" << endl;
    for(int TDsize = 0;TDsize < 500000; TDsize++)
    {
        int RAPRAM = rnGen(maxNum);
        
        if(RAPRAM%2 == 0)
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
            out << "mtp" << endl;
        }
        else if(RAPRAM % 2 == 1)
        {
            if(RAPRAM % 5 == 3)
            {
                out << "mtd -r " << rnGen(mtdS);
                if(RAPRAM % 2 == 1)
                    out << " -a" << endl;
                else
                    out << endl;
                out << "mtp" << endl;
            }
            else if(RAPRAM % 5 == 2 || RAPRAM % 5 == 1 || RAPRAM % 5 == 3)
            {
                out << "mtd -i " << rnGen(mtdS);
                if(RAPRAM % 2 == 1)
                    out << " -a" << endl;
                else
                    out << endl;
                out << "mtp" << endl;
            }
            //mtd
            
        }
        else if(RAPRAM % 9 == 8 || RAPRAM % 9 == 4)
        {
            out << "mtr " << rnGen(mtrS) << endl;
            //mtr
            out << "mtp" << endl;
        }
        
    }
    
    out << "usage" << endl;
    out << "q -f" << endl;
    out.close();
    return 0;
}