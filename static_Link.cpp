#include <iostream>
using namespace std;
typedef struct Link_S
{
	int pNextFree;   //指向下一个可以操作的空闲index
	int data;
}*PLink,Link;

int g_link_free;
void link_init(Link link[],int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
    	link[i].pNextFree = i + 1;
    }
    link[size - 1].pNextFree = -1;
}
void link_delete(Link link[],int index)
{
	link[index].pNextFree = g_link_free;
	g_link_free = index;
}
void link_add(Link link[],int data)
{
	int curIndex = g_link_free;
	g_link_free = link[curIndex].pNextFree;
	link[curIndex].pNextFree = -1;
	link[curIndex].data = data;
}
int main(int argc,char* argv[])
{
	Link link[10];
	link_init(link,10);
	for (int i = 0; i < 5; ++i)
	{
		link_add(link,i);
	}
	link_delete(link,7);
	//link_delete(link,2);
	//link_add(link,10);
	for (int i = 0; i < 10; ++i)
	{
		cout<<"i = "<<i<<"    "<<link[i].pNextFree<<endl;
	}
	cout<<"g_link_free = "<<g_link_free<<endl;
	return 0;
}