#include <iostream>
#include <fstream>
#include "src//util//rnGen.h"

using namespace std;

#define lines 1000
//the following are probabilities for each event
//p(D) prob of delete 
//p(a) prob of add
//p(s) prob of sort
//p(r) prob of rest
//the following are in x%
//so type whatever x you want
#define pd 35
#define pdr 10000
#define pda 100000
#define pa 50
#define ps 10
#define pr 5
#define ppi 30
#define cases 1000
#define adtr 70

//print out occurs every 10 lines
#define print out << "adtp" << endl << "adtp -r" << endl


int main()
{
    RandomNumGen rn;
    ofstream out("tests//td1");
    for(int i=0;i<cases;i++)
    {
        int randN = rn(pa + pd + ps + pr + ppi);
        if(randN < pd)
        {
            randN = rn(100000);
            if(randN % 3 == 0)
                out << "adtd -r " << rn(pdr) << endl;
            else if(randN % 3 == 1)
                out << "adtd -f " << rn(pdr) << endl;
            else
                out << "adtd -b " << rn(pdr) << endl;
        }
        else if(pd < randN && randN  < pa+pd)
        {
            out << "adta -r " << rn(pda) << endl;
        }
        else if(pa+pd < randN && randN < pa + pd + ps)
        {
            out << "adts" << endl;
        }
        else if(pa + pd + ps < randN && randN < pa + pd + ps + pr)
        {
            out << "adtr " << rn(adtr) << endl;
        }
        else if(pa + pd + ps + pr < randN && randN < pa + pd + ps + pr + ppi)
            out << "adtp -i " << rn(pda) << endl;
        if(i % 10 == 0)
            print;
    }
    return 0;
}