/*

User-define comlex type,
using whole operator overload

This file is the test for complex.h

Liuqing Yang<yllqq#outlook.com>

*/

//#include <complex>
#include "complex.h"

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class Add
{
	complex val;
public:
	Add(complex c){val = c; }
	Add(double r, double i){val = complex(r, i); }
	void operator()(complex& c) const {c += val; }
};
int main(int argc, char* argv[])
{
	complex a(2,2), b(3,4);
	complex c = a + b;
	complex d;
	complex e = a;
	complex f = 2;
	int g;
	c += 2;
	c -= 1;
	f = 2 + a;
	f = - a + b;
	f.print();
#ifdef _WIN32
	system("pause");
#endif
	vector<complex> v1(5);
	for(int i = 0; i < 5; i++)
		v1[i] = complex(i,i+1);
	for_each (v1.begin(), v1.end(), Add(1,-1));
	for(int i = 0; i < 5; i++)
		v1[i].print();
#ifdef _WIN32
	system("pause");
#endif
}


