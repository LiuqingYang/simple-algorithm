/*

24-point game C++ solution
Compile with VS 2010+ or gcc with -std=c++11 -pthread

Seems cannot run on Linux Mint16

Liuqing Yang<yllqq#outlook.com>

*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
using namespace std;

inline string tostring(double _Val)
{	// convert long double to string
	typedef back_insert_iterator<string> _Iter;
	typedef num_put<char, _Iter> _Nput;
	const _Nput& _Nput_fac = use_facet<_Nput>(locale());
	ostream _Ios((streambuf *)0);
	string _Str;
	_Nput_fac.put(_Iter(_Str), _Ios, ' ', _Val);
	return (_Str);
}

struct data
{
	double d;
	string s;
	int priority;
	bool used;
	data(double dd) :d(dd), used(false), priority(0)
	{
		s = to_string(d);
	}
	data(int p) : priority(p), used(false) { }
};

typedef data (*FUN)(data, data);	//公共变量区域
double target;
vector<FUN> funlist;
//int abcount, fpluscount, recursivecount;
static const double ALLOWERROR = 0.00001;
mutex filemtx;
ofstream ofs;

inline void addbracket(data &a, data &b, data &c)
{
	//abcount++;
	if(a.priority > c.priority )
	{
		a.s = "(" + a.s + ")";
	}
	if(b.priority >= c.priority )
	{
		b.s = "(" + b.s + ")";
	}
}

data fplus(data a, data b)
{
	//fpluscount++;
	data c(3);
	c.d = a.d + b.d;
	addbracket(a, b ,c);
	c.s = a.s + " + " + b.s;
	return c;
}

data fminus(data a, data b)
{
	data c(2);
	c.d = a.d - b.d;
	addbracket(a, b ,c);
	c.s = a.s + " - " + b.s;
	return c;
}

data fmul(data a, data b)
{
	data c(1);
	c.d = a.d * b.d;
	addbracket(a, b ,c);
	c.s = a.s + " * " + b.s;
	return c;
}

data fdiv(data a, data b)
{
	data c(1);
	c.d = a.d / b.d;
	addbracket(a, b ,c);
	c.s = a.s + " / " + b.s;
	return c;
}

data fdiv2(data a, data b)
{
	data c(1);
	c.d = b.d / a.d;
	addbracket(a, b ,c);
	c.s = b.s + " / " + a.s;
	return c;
}

template<class T> class myitor{
public:
	typedef typename T::iterator itor;
	myitor(T &c) : myc(c){
		myit = c.begin();
		while(!this->end() && myit->used) ++myit;
	}
	myitor& operator++() {	//下个未使用且值不等于当前值的数
		double val = myit->d;
		++myit;
		while(!this->end() && (myit->used || myit->d == val)) ++myit;
		return *this;
	}
	myitor& next(){	//下个未使用的数
		double val = myit->d;
		++myit;
		while(!this->end() && myit->used) ++myit;
		return *this;
	}
	itor& operator*() {return myit;}
	bool end(){return myit == myc.end();}
private:
	T &myc;
	itor myit;
};

template<class T> bool allused(T& myc)
{
	for(typename T::const_iterator it = myc.begin(); it != myc.end(); ++it)
		if(!it->used) return false;
	return true;
}

template<class T> typename T::iterator myinsert(T &myc, data d)
{
	for(typename T::iterator it = myc.begin(); it != myc.end(); ++it)
		if(it->d < d.d)
		{
			it = myc.insert(it, d);
			return it;
		}
		myc.push_back(d);
		return --myc.end();
}

void recursivefun(list<data>& v, ofstream& ofs)
{
	//recursivecount++;
	for (myitor<list<data>> it1(v); !it1.end(); ++it1)	//it1:第一个数
	{
		(*it1)->used = true;
		myitor<list<data>> it2 = it1;
		for (it2.next(); !it2.end(); ++it2)	//it2:第二个数
		{
			(*it2)->used = true;
			for(vector<FUN>::const_iterator it = funlist.begin();
				it != funlist.end(); ++it)
			{
				data datatemp = (*it)(**it1, **it2);
				if(allused(v))
				{
					if((datatemp.d - target) < ALLOWERROR && (datatemp.d - target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << datatemp.s << '\n';
					}
					if((datatemp.d + target) < ALLOWERROR && (datatemp.d + target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << "-(" << datatemp.s << ")\n";
					}
				}
				else
				{
					list<data>::iterator insit = myinsert(v, datatemp);
					recursivefun(v, ofs);
					v.erase(insit);
				}
			}
			(*it2)->used = false;
		}
		(*it1)->used = false;
	}
}
void recursivefun1(list<data>& v, ofstream& ofs)		//执行+ - *运算
{
	//recursivecount++;
	for (myitor<list<data>> it1(v); !it1.end(); ++it1)	//it1:第一个数
	{
		(*it1)->used = true;
		myitor<list<data>> it2 = it1;
		for (it2.next(); !it2.end(); ++it2)	//it2:第二个数
		{
			(*it2)->used = true;
			for(vector<FUN>::const_iterator it = funlist.begin();	//执行+ - *运算
				it != funlist.begin() + 3; ++it)
			{
				data datatemp = (*it)(**it1, **it2);
				if(allused(v))
				{
					if((datatemp.d - target) < ALLOWERROR && (datatemp.d - target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << datatemp.s << '\n';
					}
					if((datatemp.d + target) < ALLOWERROR && (datatemp.d + target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << "-(" << datatemp.s << ")\n";
					}
				}
				else
				{
					list<data>::iterator insit = myinsert(v, datatemp);
					recursivefun(v, ofs);
					v.erase(insit);
				}
			}
			(*it2)->used = false;
		}
		(*it1)->used = false;
	}
}

void recursivefun2(list<data>& v, ofstream& ofs)		//执行/运算
{
	//recursivecount++;
	for (myitor<list<data>> it1(v); !it1.end(); ++it1)	//it1:第一个数
	{
		(*it1)->used = true;
		myitor<list<data>> it2 = it1;
		for (it2.next(); !it2.end(); ++it2)	//it2:第二个数
		{
			(*it2)->used = true;
			for(vector<FUN>::const_iterator it = funlist.begin() + 3;	//执行/运算
				it != funlist.end(); ++it)
			{
				data datatemp = (*it)(**it1, **it2);
				if(allused(v))
				{
					if((datatemp.d - target) < ALLOWERROR && (datatemp.d - target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << datatemp.s << '\n';
					}
					if((datatemp.d + target) < ALLOWERROR && (datatemp.d + target) > -ALLOWERROR){
						lock_guard<mutex> _(filemtx);
						ofs << "-(" << datatemp.s << ")\n";
					}
				}
				else
				{
					list<data>::iterator insit = myinsert(v, datatemp);
					recursivefun(v, ofs);
					v.erase(insit);
				}
			}
			(*it2)->used = false;
		}
		(*it1)->used = false;
	}
}

int main(int argc, char* argv[])
{
	funlist.push_back(fplus);
	funlist.push_back(fminus);
	funlist.push_back(fmul);
	funlist.push_back(fdiv);
	funlist.push_back((fdiv2));

	//myinsert(v, a);
	//cout << "请输入目标数：" << endl;
	//cin >> target;
	target = 24;
	list<data> v;
	double d;
	char filename[] = "a.txt";
	vector<thread> tvec;
	cout << "请输入一串数, 0结束：" << endl;
	cin >> d;
	while(d != 0.0){
		myinsert(v, data(d));
		cin >> d;
	}
	ofs.open(filename);
	tvec.push_back(thread([&v]{
		auto v1 = v;
		recursivefun1(v1, ofs);
	}));
	tvec.push_back(thread([&v]{
		auto v1 = move(v);
		recursivefun2(v1, ofs);
	}));
	cout << "正在处理..\n";
	for(auto& thr : tvec)
		thr.join();
	//cout << abcount << "  " << fpluscount << "  " << recursivecount << endl;

	//system("pause");
}

