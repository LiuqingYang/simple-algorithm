/*

Self defines STL classes,
using class inherit,
include container, interator.
This source code is for test of simple-stl.h.

Compile with VS 2010+ or gcc with -std=c++11

Liuqing Yang<yllqq#outlook.com>

*/

#include "simple-stl.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
using std::sort;
using std::string;
using std::cout;
using std::cin;

void f1(vector<string> &vs)
{
	vector<string>::iterator it = vs.begin();
	vector<string>::iterator itbegin;
	while(it != vs.end())
	{
		if((*it)[0] == 'b')
		{
			itbegin = it;
			break;
		}
		++it;
	}
	while(it != vs.end())
	{
		if((*it)[0] != 'b') break;
		++it;
	}
	vs.erase(itbegin, it);
}

void f2(vector<string> &vs, string s)
{
	vector<string> vstemp;
	for(vector<string>::const_iterator it = vs.begin();
		it != vs.end(); ++it)
	{
		bool issame = false;
		for(int i = 0; !issame && (*it)[i] != '\0'; i++)
			for(int j = 0; ; j++)
			{
				if(j == s.size())
				{
					issame = true;
					break;
				}
				if((i+j) == (*it).size() || s[j] != (*it)[i+j]) break;
			}
		if(!issame) vstemp.push_back(*it);
	}
	vs.swap(vstemp);
}

void vsin(vector<string> &vs)
{
	string instring;
	vs.push_back("apple");
	vs.push_back("orange");
	vs.push_back("orange a");
	vs.push_back("b orange");
	vs.push_back("pear");
	vs.push_back("pineapples");
	vs.push_back("bananas");
	vs.push_back("coconut");
	vs.push_back("blackberry");
}

template<class  T> void print(spe::const_Itor<T>& it)
{
	T const* p;
	p = it.first();
	if(!p) return;
	cout << *p << '\n';
	while((p = it.next()) != 0)
		cout << *p << '\n';
}

void print(obj::List *li)
{
	using namespace obj;
	while(Object* po = li->get())
	{
		intlink *il = static_cast<intlink*>(po);
		cout << il->data << '\n';
	}
}

void f()
{
	using namespace spe;
	Vector<int> vi(100);
	for(int i = 0; i < 100; i++)
		vi[i] = 100 - i;
	const_reverse_Vector_Itor<int> it(vi);
	print(it);
	cout << "\n\n\n";
	List<int> li;
	for(int i = 0; i < 100; i++)
		li.put(i+1);
	const_reverse_List_itor<int> lit(li);
	print(lit);
}

void g()
{
	using namespace obj;
	Vector li(100);
	intlink *lk = new intlink(1);
	li.put(lk);
	lk = new intlink(2);
	li.put(lk);
	lk = new intlink(3);
	li.put(lk);
	lk = new intlink(4);
	li.put(lk);
	cout << static_cast<intlink*>(li[2])->data;
}

void h()
{

}

const int looptime = 100000000;
void stltest()
{
	vector<int> stlvi(looptime);
	for(auto it = stlvi.begin(); it != stlvi.end(); ++it)
		*it = int(double(rand()) / RAND_MAX * 1024);
	int64_t count = 0;
	for(auto it = stlvi.begin(); it != stlvi.end(); ++it)
		count += *it;
	cout << count / looptime << '\n';
}

void spetest()
{
	using namespace spe;
	Vector<int> vi(looptime);
	for(Vector_Itor<int> it(vi); it.next() != 0;)
		**it = int(double(rand()) / RAND_MAX * 1024);
	int64_t count = 0;
	for(Vector_Itor<int> it(vi); it.next() != 0;)
		count += **it;
	cout << count / looptime << '\n';
}

void objtest()
{	
	using namespace obj;
	Vector li(looptime);
	intlink *lk;
	for(int i = 0; i < looptime; i++)
	{
		lk = new intlink(int(double(rand()) / RAND_MAX * 1024));
		li.put(lk);
	}

}

int main()
{
	cout << "Start stltest!\n";
	stltest();
	cout << "Finish stltest!\n\n\n\n";
	cout << "Start spetest!\n";
	spetest();
	cout << "Finish spetest!\n\n\n\n";
//	system("pause");
	//objtest();
}

