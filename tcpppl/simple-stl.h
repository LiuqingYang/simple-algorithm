/*

Self defines STL classes,
using class inherit,
include container, interator.

Liuqing Yang<yllqq#outlook.com>

*/

#include <cstddef>

namespace spe
{	//专门化的容器和迭代器
template<class T> class Vector{
	T* _data;
	size_t _size;
	Vector(Vector &vv) { }
	Vector& operator=(Vector &vv) { }
public:
	explicit Vector(size_t n)  : _size(n) {_data = new T[n];}
	inline T& operator[](size_t n) {return _data[n];}
	const size_t size() const {return _size;}
	~Vector() {delete[] _data;}
	// ...
};

template<class T> class List{
public:
	class Link{
	public:
		T data;
		Link* prev;
		Link* next;
		Link(T in, Link* pp)
			: data(in), prev(pp), next(0){ } 
	};
	List() : _first(0), _curr(0){ }
	void put(T t) {
		Link *lnk = new Link(t, _curr);
		if(_first)
		{
			_curr->next = lnk;
			lnk->prev = _curr;
			_curr = lnk;
		}
		else
			_first = _curr = lnk;
	}
	Link* get() const {
		return _first;
	}
	Link* getcurr() const {
		return _curr;
	}

	// ...
private:
	Link *_first;
	Link *_curr;
};

template<class T> class Itor{
	//无元素返回0
public:
	virtual T* first() = 0;
	virtual T* next() = 0;
};

template<class T> class const_Itor{
	//无元素返回0
public:
	virtual T const* first() = 0;
	virtual T const* next() = 0;
};

template<class T> class Vector_Itor : public Itor<T>{
	Vector<T> &v;
	size_t index;
public:
	Vector_Itor(Vector<T> &vv) : v(vv), index(0) { }
	T* first() {return(v.size() ? &v[index=0] : 0);}
	T* next() {return(++index < v.size() ? &v[index] : 0);}
	T* operator*() {return &v[index];}
};

template<class T> class reverse_Vector_Itor : public Itor<T>{
	Vector<T> &v;
	size_t index;
public:
	reverse_Vector_Itor(Vector<T> &vv) : v(vv), index(0) { }
	T* first() {return(v.size() ? &v[index = v.size()-1] : 0);}
	T* next() {return(--index < v.size() ? &v[index] : 0);}
};

template<class T> class List_itor : public Itor<T>{
	List<T> &lst;
	typename List<T>::Link *p;
public:
	List_itor(List<T> &ll) : lst(ll), p(0){ }
	T* first() {
		p = lst.get();
		return p ? &(p->data) : 0;
	}
	T* next() {
		p ? p = p->next : 0;
		return p ? &(p->data) : 0;
	}
};

template<class T> class reverse_List_itor : public Itor<T>{
	List<T> &lst;
	typename List<T>::Link *p;
public:
	reverse_List_itor(List<T> &ll) : lst(ll), p(0){ }
	T* first() {
		p = lst.getcurr();
		return p ? &(p->data) : 0;
	}
	T* next() {
		p ? p = p->prev : 0;
		return p ? &(p->data) : 0;
	}
};

template<class T> class const_Vector_Itor : public const_Itor<T>{
	Vector<T> &v;
	size_t index;
public:
	const_Vector_Itor(Vector<T> &vv) : v(vv), index(0) { }
	T const* first() {return(v.size() ? &v[index=0] : 0);}
	T const* next() {return(++index < v.size() ? &v[index] : 0);}
};

template<class T> class const_reverse_Vector_Itor 
	: public const_Itor<T>{
	Vector<T> &v;
	size_t index;
public:
	const_reverse_Vector_Itor(Vector<T> &vv) : v(vv), index(0) { }
	T const* first() {return(v.size() ? &v[index = v.size()-1] : 0);}
	T const* next() {return(--index < v.size() ? &v[index] : 0);}
};

template<class T> class const_List_itor 
	: public const_Itor<T>{
	List<T> &lst;
	typename List<T>::Link *p;
public:
	const_List_itor(List<T> &ll) : lst(ll), p(0){ }
	T const* first() {
		p = lst.get();
		return p ? &(p->data) : 0;
	}
	T const* next() {
		p ? p = p->next : 0;
		return p ? &(p->data) : 0;
	}
};

template<class T> class const_reverse_List_itor 
	: public const_Itor<T>{
	List<T> &lst;
	typename List<T>::Link *p;
public:
	const_reverse_List_itor(List<T> &ll) : lst(ll), p(0){ }
	T const* first() {
		p = lst.getcurr();
		return p ? &(p->data) : 0;
	}
	T const* next() {
		p ? p = p->prev : 0;
		return p ? &(p->data) : 0;
	}
};
}	//end namespace spe

namespace obj
{
class Object{

};

class container : public Object{
public:
	class bad_op{};
	virtual Object* get() {throw bad_op();};
	virtual void put(Object*) {throw bad_op();}
	virtual Object* operator[](size_t) {throw bad_op();}
};

struct Link : public Object{
	Link *prev;
	Link *next;
	Link() : prev(0), next(0) { }
};

struct intlink : public Link{
	int data;
	intlink(int in = 0) : data(in) { }
};

class List : public container{
	Link *head;
	Link *curr;
public:
	List() : head(0), curr(0) { }
	Object* get(){
		if(!curr) return 0;
		Link* ret = curr;
		curr = curr->prev;
		if(curr) curr->next = 0;
		else head = 0;
		ret->prev = 0;
		return ret;
	}
	void put(Object* inobj) {
		Link *in = static_cast<Link*>(inobj);
		if(!head) head = curr = in;
		else{
			curr->next = in;
			in->prev = curr;
			in->next = 0;
			curr = in;
		}
		return;
	}
};

class Vector : public container{
	size_t size;
	size_t curr;
	Link **data;
public:
	Vector(size_t siz) : size(siz), curr(0){
		data = new Link*[size];
	}
	void put(Object* inobj) {
		data[curr++] = static_cast<Link*>(inobj);
	}
	Object* operator[](size_t num){
		return static_cast<Object*>(data[num]);
	}
};

}	//end namespace obj

