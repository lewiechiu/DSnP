#include <iostream>
#include <fstream>
#include "src/util/rnGen.h"
using namespace std;
int main()
{
    RandomNumGen rn;
	ofstream out("cirfn100");
	for(int i=3900;i<4000;i++)
    {

        size_t que = rn(81067) + 3400;
        for(int j=30;j<80;j++)
        {
            out << "cirg " << que << " -fanin " << j << endl;
            out << "cirg " << que << " -fanout " << j << endl;
        }
    }
		
	return 0;
}