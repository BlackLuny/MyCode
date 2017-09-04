#include <iostream>
using namespace std;
template <class T,bool Brever>
T test(T a, T b)
{
	if (Brever)
		cout << a << b << endl;
	else
		cout <<"no "<<a <<b << endl;
}

//显示实例化
template int test<int,false>(int,int);

class A
{
public:
	A():m_a(1),m_b(2){};
private:
	int m_a;
	int m_b;
};
int main()
{
	A a;
	test<int,false>(10,20);
	return 0;
}