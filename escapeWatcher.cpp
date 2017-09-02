#include <iostream>
using namespace std;

int getMax(unsigned int a, unsigned int b, unsigned int c)
{
	if (a <= b)
		a = b;
	if (c >= a)
		return c;
	else 
		return a;
}
int func(unsigned int uiMagic, unsigned int uiDist, unsigned int uiTime)
{
	if (uiTime == 0 || uiTime * 60 < uiDist) return 0;  //不能到达
	if (uiDist<=17 && uiTime>0) return 1;      //直接一步就到达
	if (uiMagic / 10 <= uiTime && uiMagic / 10 * 60 >= uiDist)
		return uiDist / 60 + (uiDist % 60 > 0 ? 1 : 0);  //直接一直快走
	int maxMagicDx = uiDist / 60 * 10 + 1 + 4 < 4 * (uiTime + 1)? uiDist / 60 * 10 + 1 + 4 : 4 * (uiTime+1);//最大可以增加的魔法值
	//int maxMagicDx = uiTime - uiDist / 60 + 5;
	int maxMagic = uiMagic + maxMagicDx + 1;
	unsigned int **f = new unsigned int*[maxMagic];
	for (int i = 0; i < maxMagic; ++i)
		f[i] = new unsigned int[uiTime + 1];
	for (int i = 0; i < maxMagic; ++i)
		f[i][0] = 0;

	for (int i = 1; i <= uiTime; ++i)
	{
		for (int j = 0; j < maxMagic - 4; ++j)
			f[j][i] = j >=10 ? getMax(f[j-10][i-1] + 60, f[j][i-1] + 17, f[j+4][i-1]) : getMax(0, f[j][i-1] + 17, f[j+4][i-1]);
        if (f[uiMagic][i] >= uiDist)
        {
        	cout<<"Ok "<< i << endl;
        	for (int i = 0; i < maxMagic; ++i)
		        delete [] f[i];
		    delete []f;
        	return i;
        }
	}
	unsigned int res = f[uiMagic][uiTime];
    for (int i = 0; i < maxMagic; ++i)
		delete [] f[i];
	delete []f;
	return res;

}

int main()
{
    cout<<func(2,60,10)<<endl;
}