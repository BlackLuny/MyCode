#include <iostream>

//辗转相除法求最大公约数
int gcd(int a, int b)
{
	while(b!=0)
	{
		int c = a;
		a = b;
		b = c % b;
	}
	return a;
}

//扩展欧几里得算法 求数论倒数
int invert(int e, int f)
{
	int a = f, b = e, t1 = 0, t2 = 1;
	
	while(b != 0)
	{
		int t = a;
		a = b;
		int q = t / b;
		b = t % b;
		t = t1 - q * t2;
		t1 = t2;
		t2 = t;
	}
	
	if(t1 < 0)  //扩展欧几里得算法得到的结果可能为负数，所以需要把它“掰正”
		t1 += f;
	return t1;
}

int main(int arcg,char *argv[])
{
	int a = 3 % 1;
	std::cout<<invert(20,7)<<std::endl;
	
	
	return 0;
}