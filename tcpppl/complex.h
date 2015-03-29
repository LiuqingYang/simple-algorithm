/*

User-define comlex type,
using whole operator overload

Liuqing Yang<yllqq#outlook.com>

*/
#include <iostream>
using std::cout;
using std::endl;

class complex
{
public:
	complex(double a = 0, double b = 0) : re(a), im(b) { };	//构造函数
	complex(const complex& a) : re(a.re), im(a.im) { };	//复制构造函数
	inline complex& operator=(const complex&);	//运算符“=”重载 复制函数
	inline complex& operator=(double);
	complex& operator+=(const complex&);
	complex& operator+=(double);
	complex operator+(const complex&) const;
	complex operator+(double) const;
	complex& operator+() {return *this;}
	friend complex operator+(double, const complex&);
	complex& operator-=(const complex&);
	complex& operator-=(double);
	complex operator-(const complex&) const; 
	complex operator-(double) const;
	complex& operator-();
	bool operator==(const complex& a) const
	{	
		return re == a.re && im == a.im;
	}
	bool operator!=(const complex& a) const
	{
		return re != a.re || im != a.im;
	}

	double real() const {return re;}
	inline double imag() const {return im;}

	void print()
	{
		cout << "Re = " << re << "; Im = " << im << endl;
	}
private:
	double re,im;
};

inline complex& complex::operator=(const complex& a)
{
	re = a.re;
	im = a.im;
	return *this;
}

inline complex& complex::operator=(double a)
{
	re = a;
	im = 0;
	return *this;
}

complex& complex::operator+=(const complex& a)
{
	re += a.re;
	im += a.im;
	return *this;
}

complex& complex::operator+=(double a)
{
	re += a;
	return *this;
}

complex complex::operator+(const complex& a) const
{
	complex c(*this);
	return c += a;
}

complex complex::operator+(double a) const
{
	complex c(*this);
	return c += a;
}

complex operator+(double a, const complex& b)
{
	complex c(b);
	return c += a;
}

complex& complex::operator-=(const complex& a)
{
	re -= a.re;
	im -= a.im;
	return *this;
}

complex& complex::operator-=(double a)
{
	re -= a;
	return *this;
}

complex complex::operator-(const complex& a) const
{
	
	complex c(*this);
	return c -= a;
}

complex complex::operator-(double a) const
{
	complex c(*this);
	return c -= a;
}


complex& complex::operator-()
{
	re = -re;
	im = -im;
	return *this;
}

