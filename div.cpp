#include <iostream>

int gcd(unsigned int a, unsigned int b)
{
    unsigned int m = a, n = b;
    while(m % n )
    {
    	unsigned int tmp = n;
    	n = m % n;
    	m = tmp;
    }
    return a * b / n;
}



int main()
{
	std::cout <<"gcd("<<10<<","<<5<<")="<<gcd(5,701)<<std::endl;
	return 0;
}