#include <iostream>
#include <vector>
#include <queue>
class C_dijkstra
{
    typedef struct Edge 
    {
        int to, value;
        int next;
    }*PEdge,Edge;

    int max_nodes;
    int *head;

    int cur_edge_num;
    PEdge pEdges;
    int max_edges;
    int* dist;
    typedef std::pair<int,int> pair_ii;//<dist,v>  从起点到v的距离为dist
public:
    C_dijkstra(int max_nodes)
            :max_nodes(max_nodes),cur_edge_num(0)
    {
        head = new int[max_nodes];
        for (int i = 0; i < max_nodes; ++i)
            head[i] = -1;
        max_edges = max_nodes*max_nodes;
        pEdges = new Edge[max_edges];
        dist = new int[max_nodes];//距离数组
        memset((void*)dist, 0x6f, sizeof(int)*max_nodes);
    }
    ~C_dijkstra()
    {
        delete [] head;
        delete [] pEdges;
        delete [] dist;
    }
    void addEdge(int from, int to, int value)
    {
        if (cur_edge_num >= max_edges) return;
        int cur_cnt = cur_edge_num;
        PEdge pCur = pEdges + cur_cnt;
        pCur->to = to;
        pCur->value = value;
        pCur->next = head[from];
        head[from] = cur_cnt;
        ++cur_edge_num;
    }
    void dijkstra(int startNode)
    {
        std::priority_queue<pair_ii, std::vector<pair_ii>, std::greater<pair_ii> > q;
        bool* vis = new bool[max_nodes];//s数组
        memset((void*)vis, 0, sizeof(bool)*max_nodes);
        std:: cout << "dist = " << dist << std::endl;
        dist[startNode] = 0;  //起点距离为0
        q.push(std::make_pair(dist[startNode],startNode));
        while(!q.empty())
        {
            pair_ii cur = q.top();
            q.pop();
            int cur_v = cur.second;
            if (vis[cur_v]) continue; 
            //第一个最短的不在s集合中的点
            vis[cur_v] = true; //添加到s集合中
            //通过cur_v来更新所有与cur_v相连的点的最短距离
            for (int next_id = head[cur_v]; next_id + 1; next_id = pEdges[next_id].next)
            {
                PEdge pCur = pEdges + next_id;
                int cur_to = pCur->to;
                if (dist[cur_v] + pCur->value < dist[cur_to])
                {
                    //如果经过cur_v之后的距离更小
                    dist[cur_to] = dist[cur_v] + pCur->value;
                    if (!vis[cur_to])
                        q.push(std::make_pair(dist[cur_to], cur_to));  //将此点添加进去
                }
            }

        }
        delete [] vis;
    }
    inline int getDist(int to)
    {
        return dist[to];
    }
};
int main()
{
    C_dijkstra* pdj = new C_dijkstra(100);
    for (int i = 0; i < 100; ++i)
    {
        pdj->addEdge(i,i+1,i+1);
    }
    pdj->dijkstra(0);
    std::cout << pdj->getDist(99) << std::endl;
    delete pdj;
    return 0;
}