#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "src/util/rnGen.h"
int min(int a,int b)
{
    if(a>b)
        return b;
    return a;
}
int main()
{
    RandomNumGen rn;
    ofstream out("tre.aag");
    size_t PI = 300, l = 0, o = 30000, a = 100000;
    size_t undef = 0;
    size_t m = PI + a + undef;
    int *isUsed = new int[m+1];
    int *Height  = new int[m+1];
    out << "aag " << m << " " << PI << " 0 " << o << " " << a << endl;
    //isUsed[0] is for const 0
    // 1 is for PI
    // 2 is for AIG
    vector<int > andG;
    for(int i = 0;i<PI;i++)
    {
        size_t num = rn(m)+1;
        while(isUsed[num]==1)
            num = rn(m)+1;
        
        out << num*2 << endl;
        isUsed[num] = 1;
    }
    for(int i=0;i<o;i++)
    {
        size_t num = rn(m)+1;
        out << num*2+rn(2) << endl;
    }
    for(int i=0;i<=m;i++)
    {
        if(isUsed[i]==0)
        {
            isUsed[i] = 2;
            Height[i] = 0;
            if(i!=0)
                andG.push_back(i);
        }
        else
            Height[i] = 1;
            
    }
    int big = andG.size();
    for(int i=0;i<a;i++)
    {
        size_t andChoose = rn(big);
        out << andG[andChoose]*2 << " ";
        
        
        size_t c1 = rn(m+1);
        
        size_t c2 = rn(m+1);
        while(1)
        {
            c1 = rn(m+1);
            if(Height[c1]!=0)
                break;
        }
            
        while(1)
        {
            c2 = rn(m+1);
            if(Height[c2]!=0)
                break;
        }
            
        Height[andG[andChoose]] = max(Height[c1],Height[c2])+1;
        andG[andChoose] = andG[big-1];
        big--;
        out << c1*2 + rn(2) << " " << c2*2 + rn(2) << endl;
    }
    
    

}