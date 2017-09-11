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
			int cur_node = cur.data;
			int cur_step = cur.step;
			if (cur_node == id2)
				return cur_node - 1;
			bflag[cur_node] = true;
			for(int i = 0; i < cur_cnt; ++i)
			{
				int cur_next = m_nodes[cur_node][i];
				if (cur_next == 1 && bflag[i] == false)
					q.push(Info(cur.step+1,i));
			}
		}
		return -1;
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
	for (int i = 0; i < 1000; ++i)
	{
	g_graph.addboard("a");
	g_graph.addboard("b");
	g_graph.addboard("c");
	g_graph.addboard("d");
	g_graph.addboard("e");
	g_graph.addboard("f");
	g_graph.connect("a","b");
	g_graph.connect("c","b");
	g_graph.connect("d","b");
	g_graph.connect("e","b");
	cout << g_graph.findlength("a","f") << endl;
	g_graph.Clear();
	}


}
