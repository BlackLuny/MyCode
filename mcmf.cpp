#include <iostream>
#include <queue>
#include <vector>
class Mcmf
{
public:
	Mcmf(int nodeNums)
	    :m_nodes_num(nodeNums),m_cur_edge_cnt(0)
	{
		m_head = new int[nodeNums];
		memset((void*)m_head, -1, sizeof(int)*nodeNums);
		m_edgs = new Edge[nodeNums*100];  //可能会越界
		m_vis = new bool[nodeNums];
		m_dist = new int[nodeNums];
		m_pre = new int[nodeNums];
	}
	~Mcmf()
	{
		delete [] m_head;
		delete [] m_edgs;
		delete [] m_vis;
		delete [] m_dist;
		delete [] m_pre;
	}
	void addEdge(int from, int to, int cost, int cap)
	{
		PEdge pCur = m_edgs + m_cur_edge_cnt;
		pCur->from = from;
		pCur->to = to;
		pCur->cost = cost;
		pCur->cap = cap;
		pCur->flow = 0;
		pCur->next = m_head[from];
		m_head[from] = m_cur_edge_cnt++;
		pCur = m_edgs + m_cur_edge_cnt;
		pCur->from = to;
		pCur->to = from;
		pCur->cap = 0;
		pCur->cost = -cost;
		pCur->flow = 0;
		pCur->next = m_head[to];
		m_head[to] = m_cur_edge_cnt++;
	}
	bool run(int startNo, int endNo, int &minCost, int &maxFlow)
	{
		
		int t_minCost = 0, t_maxFlow = 0;
		reset();
		while(true)
		{
			//寻找增广流
			spafa(startNo);

			//判断增广流是否存在
			if (m_dist[endNo] == 0x3f3f3f3f)
				break;
			//std::cout << "cost = " << t_minCost << std::endl;
			//遍历从起点到终点所有经历的边，获取能够流动的最小的流量
			int maxCanFlow = 0x3f3f3f3f;
			for (int edge_id = m_pre[endNo]; 
				edge_id + 1; edge_id = m_pre[m_edgs[edge_id].from])
			{
				PEdge pCur = m_edgs + edge_id;
				if (pCur->cap - pCur->flow < maxCanFlow)
					maxCanFlow = pCur->cap - pCur->flow;
			}

			//沿着路径更新流量；
			for (int edge_id = m_pre[endNo]; 
				edge_id + 1; edge_id = m_pre[m_edgs[edge_id].from])
			{
				m_edgs[edge_id].flow += maxCanFlow;
				m_edgs[edge_id^1].flow -= maxCanFlow;
			}

			//更新费用和流量
			t_minCost += maxCanFlow * m_dist[endNo];
			t_maxFlow += maxCanFlow;

		}
		minCost = t_minCost;
		maxFlow = t_maxFlow;
		return true;
	}
public:
	void reset()
	{
		for (int i = 0; i < m_cur_edge_cnt; ++i)
		{
			m_edgs[i].flow = 0;
		}
	}
	void spafa(int startNode)
	{
		int tmp_nodes = m_nodes_num;
		memset((void*)m_vis, 0, sizeof(bool)*tmp_nodes);
		memset((void*)m_pre, -1, sizeof(int)*tmp_nodes);
		memset((void*)m_dist, 0x3f, sizeof(int)*tmp_nodes);
		m_dist[startNode] = 0;
		std::queue<int> q;
		q.push(startNode);
		while(!q.empty())
		{
			int cur_node = q.front();
			q.pop();
			m_vis[cur_node] = true;
			//遍历这个点的所有出边
			for (int next_edge_id = m_head[cur_node]; 
				next_edge_id + 1; next_edge_id = m_edgs[next_edge_id].next)
			{
				PEdge pCur = m_edgs + next_edge_id;
				int cur_node_to = pCur->to;
				//寻找增广流
				if (pCur->cap > pCur->flow 
					&& m_dist[cur_node_to] > m_dist[cur_node] + pCur->cost)
				{
					m_dist[cur_node_to] = m_dist[cur_node] + pCur->cost;
					m_pre[cur_node_to] = next_edge_id;
					m_vis[cur_node_to] = false;
					q.push(cur_node_to);
				}
			}
		}
	}
	typedef struct Edge
	{
		int from,to;
		int cost;
		int flow;
		int cap;
		int next;
	}*PEdge,Edge;
	PEdge m_edgs;
	int m_nodes_num;
	int* m_head;
	int m_cur_edge_cnt;

	bool *m_vis;
	int  *m_dist;
	int  *m_pre;
};


int main()
{
	Mcmf mc(10000);
	for (int i = 0; i < 9990; ++i)
	{
		for (int j = i; j < i + 11; ++j)
		{
			mc.addEdge(i,j,20000 - i+1,20000 - j+1);
		}
		
	}
	int cost,flow;
	for (int i = 0; i < 1;++i)
	{
		//mc.spafa(0);
		mc.run(0,999,cost,flow);
	}
	//mc.spafa(0);
	std::cout << "cost = "<< cost <<" flow = " << flow << std::endl;
	return 0;
}