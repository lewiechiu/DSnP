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
#define pd 50
#define pdr 50
#define pda 700
#define pa 65
#define ps 20
#define pr 40
#define ppi 10
#define cases 1500
#define adtr 5

//print out occurs every 10 lines
#define print out << "adtp" << endl


int main()
{
    RandomNumGen rn;
    size_t len = 3;
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
            int nums = rn(pda);
            if(rn(100) > 65)
                out << "adta -r " << nums << endl;
            else
            {
                for(int j=0;j<nums;j++)
                {
                    string toadd;
                    toadd.resize(len);
                    for (int i = 0; i < len; ++i)
                        toadd[i] = 'a' + rn(26);
                    out << "adta -s " << toadd << endl;
                }
                
            }
            
            
        }
        else if(pa+pd < randN && randN < pa + pd + ps)
        {
            out << "adts" << endl;
        }
        else if(pa + pd + ps < randN && randN < pa + pd + ps + pr)
        {
            string toadd;
            toadd.resize(len);
            for (int i = 0; i < len; ++i)
                toadd[i] = 'a' + rn(26);
            out << "adtq " << toadd << endl;
        }
        else if(pa + pd + ps + pr < randN && randN < pa + pd + ps + pr + ppi)
            out << "adtp " << rn(pda) << endl;
        if(i % 20 == 0)
            print;
    }
    out << "usage" << endl;
    out << "q -f " << endl;
    out.close();
    return 0;
}