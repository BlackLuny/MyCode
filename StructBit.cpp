#include <iostream>
using namespace std;
typedef struct bitTest
{
	unsigned int a:14;
	unsigned int b:19;
	struct aaa{
		int yy;
	}xy;

}bitTest;

int main()
{
	bitTest bit;
	bit.a = 16;
	bit.b = 3;
	bit.xy.yy = 1000;
	int *p = (int*)&bit;
	cout << (void*)(*(p)) <<" "<<(void*)(*(p+1)) << endl;
}
