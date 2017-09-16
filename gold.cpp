#include <iostream>
#include <stack>
#include <list>
using namespace std;
inline unsigned int GetBit(unsigned int bitMap, unsigned int nbit)
{
    return (bitMap >> nbit) & 1;
}
inline void SetBit(unsigned int &bitMap, unsigned int nbit)
{
    bitMap |= (1 << nbit);
}
class Gold
{
public:
	Gold(int nodesNum)
		:max_nodes(nodesNum + 1),act_nodes(0)
	{
		head = new int[max_nodes];
		memset((void*)head,-1,sizeof(int)*max_nodes);
        max_edges =  max_nodes*max_nodes;
        pEdges = new Edge[max_edges];
        m_nodes = new int[max_nodes];
	}
	~Gold()
	{
		delete [] head;
        delete [] pEdges;
        delete [] m_nodes;
	}
	void addNode(int nodeNum, int nodeValue)
	{
		m_nodes[nodeNum] = nodeValue;
		act_nodes++;
	}

	void addEdge_no_direct(int from, int to, int cost)
	{
		addEdge(from, to, cost);
		addEdge(to, from, cost);
	}
	int search(list<int>& path)
	{
		int max_Value = 0;
		stack<Info> s;
		int superStart = act_nodes; 
		addNode(superStart, 0);  //添加超级起点
		for (int i = 0; i < act_nodes; ++i)
			addEdge(superStart, i, 0);//添加超级起点到所有其他点的边，cost为0
		
		unsigned int tmp_bitMap = 0;
		s.push(Info(superStart, 0, tmp_bitMap));
		while(!s.empty())
		{
			Info cur_Info = s.top();      //性能优化
			s.pop();
			int cur_node = cur_Info.cur_node;
			int cur_money = cur_Info.cur_money;
			list<int> *pCur_path = &(cur_Info.path);
			tmp_bitMap = cur_Info.bitMap;
			if (cur_money > max_Value) 
			{
				max_Value = cur_money;
				path.clear();
				path = *pCur_path;
			}
			//遍历所有的相邻节点
			for (int next_edge_id = head[cur_node]; 
				next_edge_id + 1; next_edge_id = pEdges[next_edge_id].next)
			{
				PEdge pCur_edge = pEdges + next_edge_id;
				int next_node = pCur_edge->to;
				if (GetBit(tmp_bitMap, next_node)==0)
				{
					//如果这个点还没有走过
					//cout << "xx \n";
					int cost = pCur_edge->value;
					int cur_node_money = m_nodes[next_node];
					int tmp_total_money = cur_money + cur_node_money - cost;
					if (tmp_total_money < 0) continue;  //中途的钱不能为负数
					s.push(Info(next_node, tmp_total_money, tmp_bitMap, pCur_path));
				}
			}
			
		}

		return max_Value;
	}
private:
	typedef struct Edge 
    {
        int to, value;
        int next;
    }*PEdge,Edge;

    typedef struct Info
    {
    	int cur_node;         //走到的当前点
    	int cur_money;        //走到cur_node能够赚得的钱
    	unsigned int bitMap;  //经过了哪些点，经过的点的位为1
    	list<int> path;       //经过的顶点；
    	Info(int curNode,int cur_money,unsigned int last_bitMap, list<int>* plast_path = NULL)
    	:cur_node(curNode),cur_money(cur_money),bitMap(last_bitMap)
    	{
    		SetBit(bitMap,curNode);
    		if (plast_path != NULL)
    		{
    			path = *plast_path;
    			path.push_back(curNode);
    		}
    	}
    }*PInfo,Info;
   	void addEdge(int from, int to, int cost)
	{
		if (cur_edge_num >= max_edges) return;
        int cur_cnt = cur_edge_num;
        PEdge pCur = pEdges + cur_cnt;
        pCur->to = to;
        pCur->value = cost;
        pCur->next = head[from];
        head[from] = cur_cnt;
        ++cur_edge_num;
	}
    int max_nodes;
    int *head;
    int cur_edge_num;
    int* m_nodes;
    PEdge pEdges;
    int max_edges;

    int act_nodes;
};



int main()
{
	Gold g(10);
	g.addNode(0,100);
	g.addNode(1,150);
	g.addNode(2,130);
	g.addNode(3,50);
	g.addNode(4,250);
	g.addNode(5,50);
	g.addNode(6,150);
	g.addNode(7,20);
	g.addNode(8,80);
	g.addNode(9,280);
	g.addNode(10,300);
	g.addEdge_no_direct(0,1,30);
	g.addEdge_no_direct(0,2,100);
	g.addEdge_no_direct(1,5,80);
	g.addEdge_no_direct(2,4,80);
	g.addEdge_no_direct(2,5,10);
	g.addEdge_no_direct(3,4,100);
	g.addEdge_no_direct(3,5,50);
	g.addEdge_no_direct(4,8,80);
	g.addEdge_no_direct(4,9,110);
	g.addEdge_no_direct(5,8,150);
	g.addEdge_no_direct(5,9,100);
	g.addEdge_no_direct(5,6,50);
	g.addEdge_no_direct(7,8,90);
	g.addEdge_no_direct(8,9,150);
	g.addEdge_no_direct(9,10,400);
	g.addEdge_no_direct(6,10,250);
	list<int> path;
	int max = g.search(path);
	cout << max << endl;
	list<int>::iterator it;
	for (it = path.begin(); it != path.end(); ++it)
	{
		cout << *it << " " << endl;
	}
}