#include <iostream>
#include <queue>
#include <stack>

template <class T, class cmp>
class Search
{
public:
	typedef enum
	{
		STATE_CONTINUE,
		STATE_REACH_GOAL,
		STATE_UNREACH
	}e_state;
	Search(){};
	//广度优先搜索，返回路径和最短距离
	e_state bfs(const T &start,std::vector<T> &path, unsigned int &step)
	{
		Info first;
		first.data = start;
		first.step = 0;
		std::queue<Info> q;
		q.push(first);
		cmp mycmp;
		Info tmp_new;
		while (!q.empty())
		{
			Info cur = q.front();
			q.pop();
			std::vector<T> next;
			e_state state = mycmp(cur.data, cur.step, cur.path, next);
			switch (state)
			{
				case STATE_CONTINUE:
					for (int i = 0; i < next.size(); ++i)
					{
						tmp_new.step = cur.step + 1;
						tmp_new.path = cur.path;
						tmp_new.path.push_back(cur.data);
						tmp_new.data = next[i];
						q.push(tmp_new);
					}
					break;
				case STATE_REACH_GOAL:
					step = cur.step;
					path = cur.path;
					path.push_back(cur.data);
					return state;
					break;
				default:
					break;
			}
		}
		return STATE_UNREACH;
	}
	//深度优先搜索
	e_state dfs(const T &start,std::vector<T> &path, unsigned int &step)
	{
		Info first;
		first.data = start;
		first.step = 0;
		std::stack<Info> q;
		q.push(first);
		cmp mycmp;
		Info tmp_new;
		while (!q.empty())
		{
			Info cur = q.top();
			q.pop();
			std::vector<T> next;
			e_state state = mycmp(cur.data,cur.step, cur.path, next);
			switch (state)
			{
				case STATE_CONTINUE:
					for (int i = 0; i < next.size(); ++i)
					{
						tmp_new.step = cur.step + 1;
						tmp_new.path = cur.path;
						tmp_new.path.push_back(cur.data);
						tmp_new.data = next[i];
						q.push(tmp_new);
					}
					break;
				case STATE_REACH_GOAL:
					step = cur.step;
					path = cur.path;
					path.push_back(cur.data);
					return state;
					break;
				default:
					break;
			}
		}
		return STATE_UNREACH;
	}
private:
	typedef struct s_tmp
	{
		int step;
		std::vector<T> path;
		T data;
	}*PInfo,Info;

};

#include <map>
using namespace std;
typedef struct 
{
	map<unsigned int,vector <int> > next;//(lineID,NextStation)
}*PNode,Node;

typedef pair<unsigned int,Node> PAIR_NODE; //(stationID,Next数组)
typedef pair<unsigned int,unsigned int> PAIR_LINE;//(lineID,startStationID)

map<unsigned int,Node> g_map_nodes;   //(nodeID,附近的站点)
map<unsigned int,unsigned int> g_map_lines; //(线路号，开始的站点号)
vector< vector<unsigned int> > g_total_paths; //保存找到的路径

typedef struct 
{
	int cur;
}*PInfo,Info;
int g_goal;
int g_max_steps;
class myclass
{
public:
	bool visited[10000];
	int maxSteps;
	myclass()
	{
		memset(visited,0,sizeof(bool) * 10000);
		maxSteps = 10000;
	}
	Search<Info,myclass>::e_state operator()(const Info &cur, int step,const vector<Info> &path,vector<Info> &next)
	{
		visited[cur.cur] = true;
		if (step > maxSteps) return Search<Info,myclass>::STATE_CONTINUE;
		if (cur.cur == g_goal)
		{	//达到了目标站点
			cout <<"got it ! steps = "<< step <<endl;
			maxSteps = step;
			g_max_steps = step;  //记录一下最大的步数
			vector<unsigned int> tmp;
			for (int i = 0; i < path.size(); ++i)
			{
				tmp.push_back(path[i].cur);
			}
			tmp.push_back(g_goal);  //最后一个的目标站点
			g_total_paths.push_back(tmp);  //增加一条路径
			cout <<"\n";
			return Search<Info,myclass>::STATE_CONTINUE;  //由于需要所有的路径，所以需要再继续搜索
		}else{
			Node node = g_map_nodes[cur.cur];
			for (map<unsigned int,vector <int> >::iterator itr = node.next.begin(); itr != node.next.end(); ++itr)
			{	//遍历所有的线路
				for(int i = 0; i < (*itr).second.size();++i)
				{
					//遍历每一条线路上的相邻站点
					unsigned int next_station = (*itr).second[i];
					if (!visited[next_station])
					{
						Info tmp = {next_station};
						next.push_back(tmp);
					}

				}

			}
			return Search<Info,myclass>::STATE_CONTINUE;
		}
	}
};


void AddLine(unsigned int LineNo, unsigned int StationNum ,unsigned  int *pStationArray)
{
	g_map_lines[LineNo] = *pStationArray;
	for (int i = 1; i < StationNum; ++i)
	{
		unsigned int cur_station = *(pStationArray + i);
		unsigned int last_station = *(pStationArray + i -1);
		PNode cur_node = &(g_map_nodes[cur_station]);
		cur_node->next[LineNo].push_back(last_station);
		PNode last_node = &(g_map_nodes[last_station]);
		last_node->next[LineNo].push_back(cur_station);
	}
	
}
void Clear()
{
	g_map_nodes.clear();
	g_map_lines.clear();
}
int CalcMinPathLen(unsigned int SrcStation, unsigned int DesStation)
{
	Search<Info,myclass> s;
	vector<Info> path;
	Info start = {SrcStation};
	unsigned int steps;
	g_goal = DesStation;
	s.bfs(start, path, steps);
	return steps;
}
int SearchMinPathes(unsigned int SrcStation,
                    unsigned int DesStation,
                    unsigned int* pPathNum,
                    unsigned int* pPathLen,
                    unsigned int **ppPathes)
{
	g_total_paths.clear();
	CalcMinPathLen(SrcStation,DesStation);
	*pPathNum = g_total_paths.size();
	*pPathLen = g_max_steps;
	for (int i = 0; i < *pPathNum; ++i)
	{
		for (int j = 0; j < g_total_paths[i].size(); ++j)
		{
			ppPathes[i][j] = g_total_paths[i][j];
		}
	}
                    
}
int main()
{
	unsigned int line[] = {1,2,3,4,5};
	AddLine(1,sizeof(line) / sizeof(unsigned int),line);
	unsigned int line2[] = {1,10,9,7,6};
	AddLine(2,sizeof(line2) / sizeof(unsigned int),line2);
	unsigned int line3[] = {5,7,8};
	AddLine(3,sizeof(line3) / sizeof(unsigned int),line3);
	unsigned int line4[] = {11,5};
	AddLine(4,sizeof(line4) / sizeof(unsigned int),line4);
	for (map<unsigned int,Node>::iterator itr = g_map_nodes.begin(); itr != g_map_nodes.end(); ++itr)
	{
		cout << "cur node = " << (*itr).first << endl << endl;
		Node node = (*itr).second;
		for (map<unsigned int,vector <int> >::iterator j = node.next.begin(); j != node.next.end(); ++j)
		{
			cout << "at line "<<(*j).first<<endl;
			for (int ii = 0; ii < (*j).second.size(); ++ii)
				cout << "next " <<(*j).second[ii]<<endl;
			//cout << "next "<<(*j).second[1]<<endl;
		}
			
	}
	cout << CalcMinPathLen(1,9) << endl;
	unsigned int pathNums;
	unsigned int pathLen;
	unsigned int **paths = (unsigned int**)malloc(sizeof(unsigned int*) * 100);
	for (int i = 0; i < 100; ++i)
	{
		/* code */
		paths[i] = (unsigned int*)malloc(sizeof(unsigned int)*200);
	}
	SearchMinPathes(1,11,&pathNums,&pathLen,paths);
	cout << "pathNums " << pathNums << endl;
	cout << "pathLens " << pathLen << endl;
	for (int i = 0; i < pathNums; ++i)
	{
		int j = 0;
		cout << "path " << i << endl;
		while (paths[i][j] != 11)
		{
			cout << paths[i][j++]<<endl;
		}
		cout << paths[i][j] << endl;
	}
	Clear();
	return 0;
}