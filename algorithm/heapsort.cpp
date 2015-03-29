/*
Refer to:
Introduction to Algorithms
Chapter 6

Heap relate functions
Liuqing Yang<yllqq#outlook.com>

Compile with VS2010+ or gcc(-std=c++11)
*/

#include <stdio.h>
#include <vector>
#include <list>
#include <algorithm>
#include <assert.h>
using namespace std;

template<class T>
class Heap
{
public:
	Heap(vector<T> in = vector<T>()) : vt(in) , heapsize(in.size()) { }
	size_t parent(size_t i) {return i / 2;} 
	size_t left(size_t i) {i <<= 1; return i;}
	size_t right(size_t i) {i <<= 1; return ++i;}
	void max_heapify(size_t i);
	void min_heapify(size_t i);
	void build_max_heap();
	void build_min_heap();
	void heapsort();
	//以下5个适用于最大堆
	T heap_maximum();
	T heap_extract_max();
	void heap_increase_key(size_t i, T key);
	void max_heap_insert(T key);
	void heap_delete(size_t i);
	//以下4个适用于最小堆
	T heap_minimum();
	T heap_extract_min();
	void heap_decrease_key(size_t i, T key);
	void min_heap_insert(T key);
	T& operator[](size_t i) {return vt[i-1];}
	size_t size() {return heapsize;}
private:
	vector<T> vt;
	size_t heapsize;
};

template<class T>
void Heap<T>::max_heapify(size_t i)
{
	assert(i <= size());
	while(1)
	{
		size_t l = left(i);
		size_t r = right(i);
		size_t largest;
		if(l <= size() && (*this)[l] > (*this)[i])
			largest = l;
		else
			largest = i;
		if(r <= size() && (*this)[r] > (*this)[largest])
			largest = r;
		if(largest == i) return;
		swap((*this)[i], (*this)[largest]);
		i = largest;
	}
}

template<class T>
void Heap<T>::min_heapify(size_t i)
{
	assert(i <= size());
	while(1)
	{
		size_t l = left(i);
		size_t r = right(i);
		size_t smallest;
		if(l <= size() && (*this)[l] < (*this)[i])
			smallest = l;
		else
			smallest = i;
		if(r <= size() && (*this)[r] < (*this)[smallest])
			smallest = r;
		if(smallest == i) return;
		swap((*this)[i], (*this)[smallest]);
		i = smallest;
	}
}

template<class T>
void Heap<T>::build_max_heap()
{
	for(size_t i = size() / 2; i > 0; i--)
		max_heapify(i);
}

template<class T>
void Heap<T>::build_min_heap()
{
	for(size_t i = size() / 2; i > 0; i--)
		min_heapify(i);
}

template<class T>
void Heap<T>::heapsort()
{
	build_max_heap();
	for(int i = size(); i >= 2; i--)
	{
		swap((*this)[1], (*this)[i]);
		heapsize--;
		max_heapify(1);
	}
}

template<class T>
T Heap<T>::heap_maximum()
{
	return (*this)[1];
}

template<class T>
T Heap<T>::heap_extract_max()
{
	assert(size());
	T maxele = (*this)[1];
	(*this)[1] = (*this)[size()];
	heapsize--;
	vt.pop_back();
	max_heapify(1);
	return maxele;
}

template<class T>
void Heap<T>::heap_increase_key(size_t i, T key)
{
	assert(key >= (*this)[i]);
	(*this)[i] = key;
	while(i > 1 && (*this)[parent(i)] < (*this)[i])
	{
		swap((*this)[i], (*this)[parent(i)]);
		i = parent(i);
	}
}

template<class T>
void Heap<T>::max_heap_insert(T key)
{
	size_t i = ++heapsize;
	vt.push_back(key);
	while(i > 1 && (*this)[parent(i)] < (*this)[i])
	{
		swap((*this)[i], (*this)[parent(i)]);
		i = parent(i);
	}
}

template<class T>
void Heap<T>::heap_delete(size_t i)
{
	assert(i != 0 && i <= size());
	(*this)[i] = (*this)[size()];
	vt.pop_back();
	heapsize--;
	max_heapify(i);
}

template<class T>
T Heap<T>::heap_minimum()
{
	return (*this)[1];
}

template<class T>
T Heap<T>::heap_extract_min()
{
	assert(size());
	T minele = (*this)[1];
	(*this)[1] = (*this)[size()];
	heapsize--;
	vt.pop_back();
	if(heapsize) min_heapify(1);
	return minele;
}

template<class T>
void Heap<T>::heap_decrease_key(size_t i, T key)
{
	assert(key <= (*this)[i]);
	(*this)[i] = key;
	while(i > 1 && (*this)[parent(i)] > (*this)[i])
	{
		swap((*this)[i], (*this)[parent(i)]);
		i = parent(i);
	}
}

template<class T>
void Heap<T>::min_heap_insert(T key)
{
	size_t i = ++heapsize;
	vt.push_back(key);
	while(i > 1 && (*this)[parent(i)] > (*this)[i])
	{
		swap((*this)[i], (*this)[parent(i)]);
		i = parent(i);
	}
}

template<class T>
class mergelists
{
public:
	struct tkpair 
	{
		T data;
		size_t pos;
		tkpair(T& in, size_t p) : data(in), pos(p) { }
		bool operator< (const tkpair& sec) {return data < sec.data;}
		bool operator> (const tkpair& sec) {return data > sec.data;}
		tkpair& operator=(const tkpair& in) {
			if(&in == this) return *this;
			data = in.data;
			pos = in.pos;
			return *this;
		}
	};
	mergelists(vector<list<T>>& in)
		: vl(in)
	{
		it = vector<typename list<T>::iterator>(in.size());
		for(size_t i = 0; i < vl.size(); i++)
		{
			it[i] = vl[i].begin();
			if(it[i] != vl[i].end()){
				hp.min_heap_insert(tkpair(*it[i], i));
				++it[i];;
			}
		}
		
	}

	bool pop(T& val)
	{
		if(hp.size() == 0) return false;
		tkpair tmp = hp.heap_extract_min();
		if(it[tmp.pos] != vl[tmp.pos].end()){
			hp.min_heap_insert(tkpair(*it[tmp.pos], tmp.pos));
			++it[tmp.pos];
		}
		val = tmp.data;
		return true;
	}
private:
	vector<list<T>>& vl;
	vector<typename list<T>::iterator> it;
	Heap<tkpair> hp;
};

void f()
{
	list<int> after;
	vector<list<int>> li(88);
	for(int i = 0; i < 100; i++)
		for(size_t j = 0; j < li.size(); j++)
			li[j].push_back(rand());
	for(size_t j = 0; j < li.size(); j++)
		li[j].sort();
	mergelists<int> ml(li);
	int tmp;
	while(ml.pop(tmp))
		after.push_back(tmp);
}

int main(int argc, char* argv[])
{
	f();
	vector<int> vi(20);
	for(int i = 0; i < 20; i++)
		vi[i] = i;
	Heap<int> hp(vi);
	hp.build_max_heap();
	int j =hp.heap_maximum();
	j = hp.heap_extract_max();
	hp.heap_increase_key(5, 22);
	hp.max_heap_insert(67);
	hp.heap_delete(3);
	hp.heap_delete(3);
	hp.build_min_heap();
	j = hp.heap_minimum();
	j = hp.heap_extract_min();
	hp.heap_decrease_key(5, -2);
	hp.min_heap_insert(-66);
}


