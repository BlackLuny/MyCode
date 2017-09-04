#include <iostream>
using namespace std;
class MyClass
{
public:
	MyClass(int v = 0):m_a(v){};
	int m_a;
	MyClass &operator++()//前缀++
	{
		m_a++;
		return *this;
	}
	MyClass operator++(int) //后缀++  ,int 只用于编译器区分，因为返回值类型不同不能当做重载函数
	{
		MyClass temp(this->m_a);
		++(*this);
		return temp;
	}
	int fun(int a,int)
	{
		cout << "fun(int a,int)" << endl;
	}
};

//函数可以只给出形参类型，而不给出形参名
int fun1(int a,int)
{
	cout << "int\n";
}
int fun1(int a,double)
{	
	cout << "double\n";
}
int main()
{
	MyClass c1(10);
	cout << (c1++).m_a << endl;  //10
	cout << c1.m_a << endl;    //11
	fun1(3,4);
	fun1(4,4.0);
}