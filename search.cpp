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



using namespace std;
typedef struct s_info
{
	int data;
}*PInfo,Info;

class myclass
{
public:
	bool visited[2000];
	myclass()
	{
		memset(visited,0, sizeof(bool) * 2000);
	}

	//重载()，当前的位置以及当前的步数,填充next
	Search<Info,myclass>::e_state operator()(const Info & cur,const int step, const vector<Info> &path,vector<Info>& next)
	{
		//标记已经走过
		visited[cur.data] = true;
		if (step > 3) return Search<Info,myclass>::STATE_CONTINUE;  //超过指定步数直接继续
		if (cur.data == 10)//达到目标点
		{
			cout <<"got it\n";
			cout << step << endl << endl;
			for (int i = 0; i < path.size(); ++i)
			{
				cout << path[i].data << endl;
			}
			return Search<Info,myclass>::STATE_CONTINUE;
		}
		else{
			//产生下一次遍历的点
			int cnt = 0;
			for (int i = 0; i < 30; ++i)
			{
				if (!visited[i])
				{
					Info t = {i};
					next.push_back(t);
					cnt++;
					if (cnt == 5)
						break;
				}
					
			}
		}
		return Search<Info,myclass>::STATE_CONTINUE;
	}
};

int main()
{
	Search<Info,myclass> s;
	Info st;
	st.data = 15;
	vector<Info> path;
	unsigned int step;
	if (s.bfs(st, path, step)==Search<Info,myclass>::STATE_REACH_GOAL)
	{
		cout << step << endl << endl;
		for (int i = 0; i < path.size(); ++i)
		{
			cout << path[i].data << endl;
		}
	}else{
		cout << "cant reach\n";
	}

	return 0;
}