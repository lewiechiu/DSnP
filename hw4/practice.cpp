#include <iostream>
using namespace std;

class A{
	public:
	short elem1 = 123463;
	int s[3];
	double a[5] = {7,6,4,3,7};
	A askfor(){return new A[10];}
};


int main()
{
	cout << "size of A:: "<<sizeof(A) << endl;
	A current, next;
	next.elem1 = 300;
	A* a = &current;
	A* b = &next;
	size_t* as = (size_t*) a;
	*as = (size_t)b;
	
	cout << "content of a->elem1: " << a->elem1 << endl;
	cout << "*as: " << *as << endl;
	cout << "as: " << as << endl;
	as = (size_t*)*as;
	cout << "*as: " << *as << endl;
	cout << "as: " << as << endl;
	cout << "cast elem1 to size_t*: " << (size_t*) a->elem1 << endl;
	cout << "address of current: " << a << endl << "address of next: " << b << endl; 	
	A tryy;
	A alist = tryy.askfor();
	size_t* listas = (size_t*)alist;
	cout << sizeof(alist) << endl;
	listas -= 1;
	cout << "content on listas -1 "<<*listas << endl;
	listas += 2;
	cout << "content on listas +1 " << *listas << endl;
}



