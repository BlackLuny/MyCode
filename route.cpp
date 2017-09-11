#include <iostream>
#include <map>
#include <string>
#include <set>
#include <queue>
using namespace std;
typedef struct Info
{
	int step;
	int data;
	Info(int step, int data):step(step),data(data)
	{

	}
}*PInfo,Info;
typedef map<int,set<int> > NodeMap;
typedef map<string, int> S2IMap;
/*class Graph
{
public:
	static int cur_cnt;
	int addboard(char *brd)
	{
		string tmp = brd;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it != m_s2i.end()) return -1;
		m_s2i[tmp] = cur_cnt++;
		return 0;
	}
	int connect(char *brd1, char *brd2)
	{
		string tmp = brd1;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id1 = (*it).second;
		tmp = brd2;
		it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id2 = (*it).second;
		set<int> &set1 = m_nodes[id1];
		set1.insert(id2);
		set<int> &set2 = m_nodes[id2];
		set2.insert(id1);
		return 0;
	}
	int findlength(char *brd1, char *brd2)
	{
		string tmp = brd1;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id1 = (*it).second;
		tmp = brd2;
		it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id2 = (*it).second;
		return search(id1, id2);
	}
	int search(int id1, int id2)
	{
		queue<Info> q;
		q.push(Info(0,id1));
		int step = 0;
		bool bflag[100] = {0};

		while(!q.empty())
		{
			Info cur = q.front();
			q.pop();
			if (cur.data == id2)
				return cur.step - 1;
			bflag[cur.data] = true;
			set<int> &set_next = m_nodes[cur.data];
			set<int>::iterator it;
			for(it = set_next.begin(); it != set_next.end(); ++it)
			{
				int cur_next = (*it);
				if (bflag[cur_next] == false)
					q.push(Info(cur.step+1,cur_next));
			}
		}
		return -1;
	}
	void Clear()
	{
		m_nodes.clear();
		m_s2i.clear();
		cur_cnt = 0;
	}
public:
	NodeMap m_nodes;
	S2IMap m_s2i;
};*/
typedef map<string, int> S2IMap;
class Graph
{
public:
	static int cur_cnt;
	Graph()
	{
		m_nodes = new int*[100];
		for (int i = 0; i < 100; ++i)
		{
			m_nodes[i] = new int[100];
			memset((void*)(m_nodes[i]),0,100*sizeof(int));
		}
	}
	~Graph()
	{
		for (int i = 0; i < 100; ++i)
		{
			delete [] m_nodes[i];
		}
		delete [] m_nodes;
	}
	int addboard(char *brd)
	{
		string tmp = brd;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it != m_s2i.end()) return -1;
		m_s2i[tmp] = cur_cnt++;
		return 0;
	}
	int connect(char *brd1, char *brd2)
	{
		string tmp = brd1;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id1 = (*it).second;
		tmp = brd2;
		it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id2 = (*it).second;
		m_nodes[id1][id2] = 1;
		m_nodes[id2][id1] = 1;
	}
	int findlength(char *brd1, char *brd2)
	{
		string tmp = brd1;
		S2IMap::iterator it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id1 = (*it).second;
		tmp = brd2;
		it = m_s2i.find(tmp);
		if (it == m_s2i.end()) return -1;
		int id2 = (*it).second;
		return search(id1, id2);
	}
	/*int search(int id1, int id2)
	{
		queue<Info> q;
		q.push(Info(0,id1));
		int step = 0;
		bool bflag[100] = {0};

		while(!q.empty())
		{
			Info cur = q.front();
			q.pop();
			int cur_node = cur.data;
			int cur_step = cur.step;
			if (cur_node == id2)
				return cur_step - 1;
			bflag[cur_node] = true;
			for(int i = 0; i < cur_cnt; ++i)
			{
				int cur_next = m_nodes[cur_node][i];
				if (cur_next == 1 && bflag[i] == false)
					q.push(Info(cur_step+1,i));
			}
		}
		return -1;
	}*/
		/*int search(int id1, int id2)
		{
			int** ppint = new int*[100];
			for (int i = 0; i < 100; ++i)
			{
				ppint[i] = new int[100];
				//memset((void*)ppint[i],0,sizeof(int)*100);
				for (int j = 0; j < 100; ++j)
				{
					if (m_nodes[i][j] == 1)
						ppint[i][j] = 1;
					else
						ppint[i][j] = 0x1f7f7f7f;
				}
			}
			for (int i = 0; i < cur_cnt; ++i)
			{
				for (int j = 0; j < cur_cnt; ++j)
				{
					for (int k = 0; k < cur_cnt; ++k)
					{
						if (i!=j && i!=k && j!=k && ppint[i][k] + ppint[k][j] < ppint[i][j])
							ppint[i][j] = ppint[i][k] + ppint[k][j];
					}
				}
			}
			int res = ppint[id1][id2] == 0x1f7f7f7f ? -1 : ppint[id1][id2] - 1;
			for (int i = 0; i < 100; ++i)
			{
				delete [] ppint[i];
			}
			delete [] ppint;
			return res;
		}*/

			//dijstra算法
		int search(int id1, int id2)
		{
			int s[100] = {0};
			int** ppint = new int*[100];
			for (int i = 0; i < 100; ++i)
			{
				ppint[i] = new int[100];
				for (int j = 0; j < 100; ++j)
				{
					if (m_nodes[i][j] == 1)
						ppint[i][j] = 1;
					else
						ppint[i][j] = 0x1f7f7f7f;
				}
			}

			s[id1] = 1;
			int dist[100];  //距离数组，从起点到i点的距离

			//初始化距离数组的值
			for (int i = 0; i < 100;++i)
			{
				dist[i] = 0x1f7f7f7f;
			}
			//设置起点距离起点的距离为0
			dist[id1] = 0;

			//主循环
			for (int i = 0; i < cur_cnt; ++i)
			{

				//找到没有在s集合中的距离最短的点，并且求出这个最短距离
				int minDist = 0x1f7f7f7f;
				int cur_nearest = id1;
				for (int v = 0; v < cur_cnt; ++v)
				{
					if (s[v]==0 && dist[v] < minDist)
					{
						cur_nearest = v;
						minDist = dist[v];
					}
				}
				//将这个点加入到s集合
				s[cur_nearest] = 1;

				//通过这个新的点cur_nearest来更新距离数组，只更新不在s集合中的数组就好了
				for (int i = 0; i < cur_cnt;++i)
				{
					if (s[i]==0 && dist[i] > dist[cur_nearest] + ppint[cur_nearest][i])
						dist[i] = dist[cur_nearest] + ppint[cur_nearest][i];
				}
			}

			int res = dist[id2] == 0x1f7f7f7f ? -1 : dist[id2] - 1;
			for (int i = 0; i < 100; ++i)
			{
				delete [] ppint[i];
			}
			delete [] ppint;
			return res;
		}
	void Clear()
	{
		//m_nodes.clear();
		m_s2i.clear();
		for (int i = 0; i < 100; ++i)
		{
			memset((void*)m_nodes[i],0,100*sizeof(int));
		}
		cur_cnt = 0;
	}
public:
	int** m_nodes;
	S2IMap m_s2i;
};
int Graph::cur_cnt = 0;
Graph g_graph;
int main()
{	
	char tmp[100],tmp2[100];
	for (int i = 0; i < 100; ++i)
	{
		string s = to_string(i);
		strcpy(tmp,s.c_str());
		g_graph.addboard(tmp);
	}
	for (int i = 0; i <= 90; ++i)
	{
		for (int j = i + 1; j < i + 11; ++j)
		{
			//if (i==j) continue;
			string s1 = to_string(i);
			strcpy(tmp,s1.c_str());
			string s2 = to_string(j);
			strcpy(tmp2,s2.c_str());
			g_graph.connect(tmp,tmp2);
		}
	}
	cout << g_graph.m_nodes[70][80] << endl;
	cout << g_graph.findlength("0","99") << endl;
	g_graph.Clear();
	for (int i = 0; i < 100; ++i)
	{
		string s = to_string(i);
		strcpy(tmp,s.c_str());
		g_graph.addboard(tmp);
	}
	for (int i = 0; i <= 90; ++i)
	{
		for (int j = i + 1; j < i + 11; ++j)
		{
			//if (i==j) continue;
			string s1 = to_string(i);
			strcpy(tmp,s1.c_str());
			string s2 = to_string(j);
			strcpy(tmp2,s2.c_str());
			g_graph.connect(tmp,tmp2);
		}
	}
	cout << g_graph.m_nodes[70][80] << endl;
	cout << g_graph.findlength("0","99") << endl;

}
