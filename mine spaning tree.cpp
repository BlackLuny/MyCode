#include <iostream>
#include <algorithm>
const int maxn = 100;
const int maxe = 100;
struct edge
{
int u,v,w;

bool operator<(const edge&b)const
{
return this->w>b.w;
}
}e[maxe];

//并查集相关
int f[maxn];

inline void init()
{
for(int i=0;i<maxn;i++)f[i]=i;
}

int find(int x)
{
if(f[x]==x)return x;
else return f[x]=find(f[x]);
}

//主算法
int kruskal(int n,int m)
{
//n:点数,m:边数
//所有边已经预先储存在e数组里
std::sort(e,e+m);
init();

int ans=0;
for(int i=0;i<m;i++)
{
int u=e[i].u,v=e[i].v,w=e[i].w;
if(find(u)==find(v))continue;
f[find(u)]=find(v);
ans+=w;
}

return ans;
}

int main(int argc,char *argv[])
{
	for(int i = 0; i < maxn; ++i)
	{
		e[i].u = i;
		e[i].v = i+1;
		e[i].w = 2*i;
	}
	std::cout<<kruskal(maxn,maxn)<<std::endl;
	return 0;
}

