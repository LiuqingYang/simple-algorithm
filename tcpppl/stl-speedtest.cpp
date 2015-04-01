/*

A C++ STL speed test between vector and list,
test random access & insert

Liuqing Yang<yllqq#outlook.com>

*/


#include <time.h>

#include <iostream>
#include <vector>
#include <list>
using namespace std;

const int REPEAT_TIME = 1000000;
const int SIZE = 1000;

template<class T>
inline void randomaccess(T &v, size_t n)
{
	typename T::value_type vt = v[n];
}

template<class T, class iter>
inline void tableoperation(T &v, iter it)
{
	typename T::value_type value;
	iter it1 = v.insert(it, value);
	v.erase(it1);
}
int main(int argc, char* argv[])
{
	long startclk, endclk;
	cout << "Repeattime : " << REPEAT_TIME << "\n\n";
	vector<int> vi(SIZE, 5);
	list<int>li(SIZE, 5);


	startclk = clock();
	for(int i = 0; i < REPEAT_TIME; i++)
		randomaccess(vi, 3);
	endclk = clock();
	cout << "Vector Randomaccess time : " << endclk - startclk << "\n\n";


	startclk = clock();
	for(int i = 0; i < REPEAT_TIME; i++)
		tableoperation(vi, vi.begin());
	endclk = clock();
	cout << "Vector tableoperation time : " << endclk - startclk << "\n\n";

	startclk = clock();
	for(int i = 0; i < REPEAT_TIME; i++)
		tableoperation(li, li.begin());
	endclk = clock();
	cout << "List tableoperation time : " << endclk - startclk << "\n\n";
}


