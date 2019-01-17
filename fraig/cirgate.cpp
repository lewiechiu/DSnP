#include <iostream>
#include <fstream>
#include "src/util/rnGen.h"

RandomNumGen rn;

using namespace std;
int main()
{
    ofstream out("checkFanOutS");
    for(int i=0;i<200;i++)
    {
        out << "cirg " << rn(1000) << endl;
        out << "cirg " << rn(1000) << " -fanin " << rn(10) << endl;
        out << "cirg " << rn(1000) << endl;
        out << "cirg " << rn(1000) << " -fanout " << rn(10) << endl;
    }
    out.close();
    out.open("checkFanOutM");
    for(int i=0;i<200;i++)
    {
        out << "cirg " << rn(10000) << endl;
        out << "cirg " << rn(10000) << " -fanin " << rn(20) << endl;
        out << "cirg " << rn(10000) << endl;
        out << "cirg " << rn(10000) << " -fanout " << rn(20) << endl;
    }
    out.close();
    out.open("checkFanOutML");
    for(int i=0;i<200;i++)
    {
        out << "cirg " << rn(80000) << endl;
        out << "cirg " << rn(80000) << " -fanin " << rn(30) << endl;
        out << "cirg " << rn(80000) << endl;
        out << "cirg " << rn(80000) << " -fanout " << rn(30) << endl;
    }
    out.close();
    out.open("checkFanOutL");
    for(int i=0;i<200;i++)
    {
        out << "cirg " << rn(120000) << endl;
        out << "cirg " << rn(120000) << " -fanin " << rn(40) << endl;
        out << "cirg " << rn(120000) << endl;
        out << "cirg " << rn(120000) << " -fanout " << rn(40) << endl;
    }
    out.close();
    out.open("checkFanOutSS");
    for(int i=0;i<200;i++)
    {
        out << "cirg " << rn(15) << endl;
        out << "cirg " << rn(15) << " -fanin " << rn(10) << endl;
        out << "cirg " << rn(15) << endl;
        out << "cirg " << rn(15) << " -fanout " << rn(10) << endl;
    }
    out.close();
    return 0;
}