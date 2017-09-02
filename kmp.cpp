#include <iostream>


//构建回退数组
void getNext(char *str, int *Next)
{
	Next[0] = -1;
	int len = strlen(str);
	int j = -1;
	for (int i = 1; i < len; ++i)
	{
		while(j>=0 && str[j+1]!=str[i]) j = Next[j];
		if (str[j+1] == str[i])  j = j + 1;
		Next[i] = j;
	}
}

//开始搜索
void kmp(char *str1, char *str2, int *Next)
{
	int len = strlen(str1);
	int len2 = strlen(str2);
	int j = -1;
	for (int i = 0; i < len; ++i)
	{
		while(j>=0 && str2[j+1] != str1[i]) j = Next[j];
		if (str2[j+1] == str1[i])  j = j +1;
		if (j == len2 - 1)
		{
			std::cout << "Got at "<< i - j << std::endl;
		}
	}
}

using namespace std;
int main(int argc, char **argv)
{
	int *Next = new int[256];
	getNext("abcd",Next);
	for (int i = 0; i < 10; ++i)
	{
		/* code */
		cout<<Next[i]<<endl;
	}
	kmp("ababacdasfsefsgsfeababacdfsefsfsaaaaaaabcdaaaaaaaaabaaaaabfegefsgesfesef","abcd",Next);
	delete[] Next;
	return 0;
}