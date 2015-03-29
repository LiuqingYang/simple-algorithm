/*

Implement of a safe pointer,
using a C++ self-define object.

Liuqing Yang<yllqq#outlook.com>

*/

#include <iostream>
#include <string>
using namespace std;

struct out_of_rangeerror { };
typedef int T;

class Ptr_to_T
{
	T *s;
	T *v;
	int num;
public:
	Ptr_to_T(T* ss, T* vv, int n) : s(ss), v(vv), num(n) { }
	T& operator*();
	Ptr_to_T& operator++();
	Ptr_to_T operator++(int);
	Ptr_to_T& operator--();
	Ptr_to_T operator--(int);
};

T& Ptr_to_T::operator*()
{
	if(v < s || v > s - 1 + num)
		throw out_of_rangeerror();
	else return *v;
}

Ptr_to_T& Ptr_to_T::operator++()
{
	v++;
	return *this;
}

Ptr_to_T Ptr_to_T::operator++(int)
{
	Ptr_to_T temp = *this;
	v++;
	return temp;
}

Ptr_to_T& Ptr_to_T::operator--()
{
	v--;
	return *this;
}

Ptr_to_T Ptr_to_T::operator--(int)
{
	Ptr_to_T temp(*this);
	v++;
	return temp;
}

void fun()
{
	T v[200];
	Ptr_to_T p(&v[0], v, 1);
	--p;
	*p = 24;	//will cause out of range error
	++p;
	//p++;
	//p++;
	*p = 53;
}

int main()
{
	try
	{
		fun();
	}
	catch(out_of_rangeerror)
	{
		cerr << "Out of range!" << endl;
	}
#ifdef _WIN32
	system("pause");
#endif
}
