
#include <iostream>
typedef struct set_t
{
int data;
int rank;
int parent;
}*PSet,set;

void init(set s[],int n)
{
for (int i = 0; i < n; ++i)
{
	s[i].parent = i;
	s[i].data = i;
	s[i].rank = 0;
}
}
int find(set s[],int x)
{
	if(s[x].parent == x)
		return x;
	else
		return find(s,s[x].parent);
}
void unionset(set s[],int x,int y)
{
	int parent_x = find(s,x);
	int parent_y = find(s,y);
	if(s[parent_x].rank > s[parent_y].rank)
	{
		s[parent_y].parent = parent_x;
	}
	else
	{
		s[parent_x].parent = parent_y;
		if(s[parent_x].rank == s[parent_y].rank)
			s[parent_y].rank++;
	}
}

int main(int argc,char* argv[])
{
	using namespace std;
	set s[256];
	init(s,256);
	for (int i = 0; i < 10; ++i)
	{
		unionset(s,i,i+1);
	}
	cout<<"find "<<find(s,11)<<endl;
	return 0;
}