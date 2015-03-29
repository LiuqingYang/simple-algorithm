/*
Copyright by Pearson Education
The C++ Programming Language Special Edition
Chapter 6

Desktop Calculator
Liuqing Yang<yllqq#outlook.com>

Compile with VS2005+ or gcc
*/

#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include "math.h"
using namespace std;


typedef double (*FUN) (double);
map<string,FUN> funtable;	//函数列表

namespace error
{
	struct Syntax_error{
		const char* p;
		Syntax_error(const char* q){p=q;}
	};
	struct Divide_by_zero{ };
}

namespace Driver
{
	void skip()
	{
		istream* input = &cin;
		char ch;
		while(input && *input)
		{
			input->get(ch);
			if(ch=='\n' || ch==';') return;
		}
	}
}
enum Token_value{			//数据类型 
	NAME,		NUMBER,		END,	PLUS='+',		MINUS='-',
	MUL='*',	DIV='/',	PRINT,	ASSIGN='=',		LP='(',		RP=')',
	FUNC
};
map<string,double> table;	//符号表 

Token_value curr_tok=PRINT;	//当前符号

struct symbol{
	bool get;		//是否需要取值
	double number_value;
	string string_value;
};

//void error(const string& text)	//错误处理 
//{
//	cerr<<"Error:"<<text<<'\n';
//}

symbol expr (symbol get);
symbol get_token()		//取当前操作值 
{
	symbol numstr;
	numstr.get=true;
	char ch=0;
	cin.get(ch);
	switch(ch){
	case '\n':	case ';':
		curr_tok=PRINT;
		return numstr;
	case 0:
		curr_tok=END;
		return numstr;
	case '+':	case '-':	case '*':	case '/':
	case '(':	case ')':	case '=':
		curr_tok=Token_value(ch);
		return numstr;
	case '0':	case '1':	case '2':	case '3':
	case '4':	case '5':	case '6':	case '7':
	case '8':	case '9':	case '.':
		cin.putback(ch);
		cin>>numstr.number_value;
		curr_tok=NUMBER;
		return numstr;
	default:
		if(isalpha(ch)){	//检测到字母
			cin.putback(ch);
			while(cin.get(ch) && isalnum(ch)) numstr.string_value.push_back(ch);
			cin.putback(ch);

			map<string,FUN>::const_iterator fit;
			fit=funtable.find(numstr.string_value);
			if(fit!=funtable.end())
			{
				curr_tok=FUNC;
				return numstr;
			}

			map<string,double>::iterator it;
			it=table.find(numstr.string_value);
			cin.get(ch);
			if(ch=='='){
				if(it!=table.end()){
					numstr.number_value=expr(numstr).number_value;
					it->second=numstr.number_value;
					cout<<"Redefine:"<<numstr.string_value<<" = "<<numstr.number_value<<endl<<endl;
					return numstr;
				}
				else
					numstr.number_value=expr(numstr).number_value;
					table.insert(map<string, double> :: value_type(numstr.string_value,numstr.number_value));
					cout<<"Define:"<<numstr.string_value<<" = "<<numstr.number_value<<endl<<endl;
					return numstr;
			}
			else
				if(it!=table.end()){
					numstr.number_value=it->second;
					cin.putback(ch);
					curr_tok=NUMBER;
					return numstr;
				}
				else{
					throw error::Syntax_error("expect '=' after a undef name");
					return get_token();
				}
		}
		throw error::Syntax_error("Bad token ");
		return get_token();
	}		
}

symbol prim(symbol);

symbol term(symbol get)	//处理* /号 
{
	symbol left=prim(get);
	get.get=true;
	for(;;)
		switch(curr_tok){
		case MUL:
			left.number_value*=prim(get).number_value;
			break;
		case DIV:
			if(double d=prim(get).number_value){
				left.number_value/=d;
				break;
			}
			throw error::Divide_by_zero();
		default:
			return left;
		}
}

symbol expr(symbol get)	//处理+ -号 
{
	symbol left=term(get);
	get.get=true;
	for(;;)
		switch(curr_tok){
		case PLUS:
			left.number_value+=term(get).number_value;
			break;
		case MINUS:
			left.number_value-=term(get).number_value;
			break;
		default:
			return left;
		}
}

symbol prim(symbol get)	//处理初等运算 
{
	if(get.get) get=get_token();
	get.get=true;
	switch(curr_tok){
	case NUMBER:{
		//double v=get.number_value;
		get_token();
		return get;
	}
	case MINUS:{
		symbol numstr=prim(get);
		numstr.number_value=-numstr.number_value;
		return numstr;
	}
	case LP:{
		get=expr(get);
		if(curr_tok!=RP) throw error::Syntax_error("expected')'");
		get_token();
		return get;
	}
	case PRINT:
		return get;
	case FUNC:{
		get_token();
		if(curr_tok!=LP) throw error::Syntax_error("expected'('");
		FUN proc;
		proc=funtable[get.string_value];
		get.number_value=proc(expr(get).number_value);
		if(curr_tok!=RP) throw error::Syntax_error("expected')'");
		get_token();
		return get;
	}
	default:
		throw error::Syntax_error("Bad token ");
		return get;
	}
}

int main()
{
	table["pi"]=3.1415926535897932385;
	table["e"]=2.7182818284590452345;
	funtable["sin"]=sin;		//定义函数
	funtable["log"]=log;
	funtable["sqrt"]=sqrt;
	symbol numstr;
	while(1){
		try{
			numstr=get_token();
			if(curr_tok==END) break;
			if(curr_tok==PRINT) continue;
			numstr.get=false;
			cout<<"Output:"<<expr(numstr).number_value<<"\n\n";
		}
		catch(error::Syntax_error a){
			cout<<"Syntax error:"<<a.p<<endl;
			//Driver::skip();
		}
		catch(error::Divide_by_zero){
			cout<<"Divide by zero\n";
			//Driver::skip();
		}
	}
#ifdef _WIN32
	system("pause");
#endif
}

