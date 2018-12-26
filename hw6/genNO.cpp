#include <iostream>
#include <fstream>
using namespace std;
int main()
{

	ofstream out("cirpn100000");
	for(int i=0;i<100000;i++)
		out << "cirg " << i << endl;
	return 0;
}