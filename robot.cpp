#include <iostream>
#include <list>
#include <vector>
#include <math.h>
using namespace std;
#define DOUBLE_ZERO(a) if(fabs(a)<0.0000000000001f) a = 0
const int INF = 0x7f7f7f7f;
const int MAX_VECTS = 10000;
typedef enum E_Arc_State
{
    ARC_STATE_L,
    ARC_STATE_U,
    ARC_STATE_T
}ARC_STATE;

class Network_Arc {
public:
    int from;
    int to;
    long flow;
    long cap;
    long cost;
    long estimate_value;
    bool bArtificial;
    ARC_STATE state;
    inline Network_Arc(int from, int to, long cost, long cap = INF, long flow = 0, int bArtificial = false)
    :from(from),to(to),cost(cost),cap(cap),flow(flow),bArtificial(bArtificial)
    {
        estimate_value = 0;
        state = ARC_STATE_L;
    }
};

class Network_Node {
public:
    long demand;
    int ori_arc_nums;
    list<Network_Arc*> list_arc_out;
    Network_Node(long demand = 0):demand(demand),ori_arc_nums(0){}
    inline void add_arc_out(Network_Arc *arc)
    {
        list_arc_out.push_back(arc);
    }
    inline void change_edgeCost(int out_v, int cost) {
        list<Network_Arc*>::iterator itr;
        for (itr = list_arc_out.begin(); itr != list_arc_out.end(); ++itr) {
            if ((*itr)->to == out_v) {
                (*itr)->cost = cost;
                break;
            }
        }
    }
};

class Network
{
private:
    vector<Network_Arc*> artificial_arcs;
public:
    long total_cost;
    int total_nodes;
    int superSource;
    Network_Node **nodes;
    inline Network(int num_nodes, int superSource)
    :total_nodes(num_nodes),superSource(superSource)
    {
        nodes = new Network_Node*[num_nodes];
        memset(nodes, 0, sizeof(Network_Node*) * num_nodes);
        total_cost = 0;
    }
    virtual ~Network()
    {
        remove_all_artificials();
        for (int i = 0; i < total_nodes; ++i)
        {
            list<Network_Arc*>::iterator itr;
            list<Network_Arc*> &pList = nodes[i]->list_arc_out;
            for (itr = pList.begin(); itr != pList.end(); ++itr)
            {
                Network_Arc* pArc = *itr;
                delete pArc;
            }
            delete nodes[i];
        }
        delete [] nodes;
    }
    inline void add_node(int nodeID, long demand = 0) {
        if (nodes[nodeID]) {
            return;
        }
        nodes[nodeID] = new Network_Node(demand);
    }
    
    inline void change_node_demand(int node, long demand) {
        nodes[node]->demand = demand;
    }
    
    inline void remove_all_artificials()
    {
        size_t len = artificial_arcs.size();
        for (int i = 0; i < len; ++i)
        {
            Network_Arc *arc = artificial_arcs[i];
            nodes[arc->from]->list_arc_out.remove(arc);
            delete arc;
        }
        artificial_arcs.clear();
    }
    inline Network_Arc* add_artificial_arc(int from, int to) {
        Network_Arc *arc = new Network_Arc(from, to, total_cost, INF, 0, true);
        nodes[from]->add_arc_out(arc);
        artificial_arcs.push_back(arc);
        return arc;
    }
    
    inline void change_edge_cost(int vect, int v_out, int cost) {
        nodes[vect]->change_edgeCost(v_out, cost);
    }
    inline void remove_orig_arc(int from, Network_Arc *arc_remove)
    {
        nodes[from]->list_arc_out.remove(arc_remove);
        nodes[from]->ori_arc_nums--;
        delete arc_remove;
    }
    inline Network_Arc* add_orig_arc(int from, int to, int cost, int capacity = INF) {
        Network_Arc *arc = new Network_Arc(from, to, cost, capacity);
        //total_cost += arc->cost;
        total_cost = INF;
        nodes[from]->add_arc_out(arc);
        nodes[from]->ori_arc_nums++;
        return arc;
    }
    inline void reset()
    {
        remove_all_artificials();
        for (int i = 0; i < total_nodes; ++i) {
            Network_Node * pNode = nodes[i];
            list<Network_Arc*>::iterator itr;
            for (itr = pNode->list_arc_out.begin(); itr != pNode->list_arc_out.end(); ++itr) {
#ifdef _BUG_CHECK
                assert((*itr)->bArtificial == false);
#endif
                (*itr)->flow = 0;
                (*itr)->state = ARC_STATE_L;
            }
        }
    }
};

#define ROOT 0

typedef enum E_ARC_DIRECTION
{
    ARC_DIR_UP,
    ARC_DIR_DOWN
}ARC_DIR;



class SpnTree
{
private:
    Network *network;
    void artificial_initial_SpnTree();
    
    void update_tree(Network_Arc* entering, Network_Arc* leaving, int join);
    void update_threadAndparent(int a_leave, int b_leave, int a_enter, int b_enter, int join);
    void update_tree_arc_dir(int a_leave, int a_enter, Network_Arc *entering);
    void update_tree_depthAndpotential(int b_enter);
public:
    int *pred;
    int *depth;
    int *thread;
    long *potential;
    Network_Arc **tree_arcs;
    ARC_DIR *tree_arc_dir;
    bool dynamicCost;
    inline SpnTree(){
        pred = new int[MAX_VECTS *11];
        depth = new int[MAX_VECTS *11];
        thread = new int[MAX_VECTS *11];
        potential = new long[MAX_VECTS *11];
        tree_arcs = new Network_Arc*[MAX_VECTS *11];
        tree_arc_dir = new ARC_DIR[MAX_VECTS *11];
    }
    virtual ~SpnTree()
    {
        delete pred;
        delete depth;
        delete thread;
        delete potential;
        delete tree_arcs;
        delete tree_arc_dir;
    }
    inline void init(Network *pnetwork, bool dynCost)
    {
        dynamicCost = dynCost;
        network = pnetwork;
        artificial_initial_SpnTree();
    }
    long updateSpnTree(const vector<Network_Arc*> &FrontArcs, const vector<Network_Arc*> &BackArcs,
                       long theta, Network_Arc *entering, Network_Arc* leaving, int common_predecessor);
};

void SpnTree::artificial_initial_SpnTree()
{
    for (int i = 1; i < network->total_nodes; ++i) {
        Network_Node *node = network->nodes[i];
        Network_Arc *arc = NULL;
        if (node->demand < 0) {
            arc = network->add_artificial_arc(ROOT, i);
            arc->flow = -node->demand;
            tree_arcs[i] = arc;
            tree_arc_dir[i] = ARC_DIR_DOWN;
        } else {
            arc = network->add_artificial_arc(i, ROOT);
            arc->flow = node->demand;
            tree_arcs[i] = arc;
            tree_arc_dir[i] = ARC_DIR_UP;
        }
        arc->state = ARC_STATE_T;
    }
    tree_arcs[ROOT] = NULL;
    
//初始化pred,depth,thread
    depth[ROOT] = 0;
    pred[ROOT] = -1;
    thread[ROOT] = 1;
    potential[ROOT] = 0;
    
    int num_nodes = network->total_nodes;
    
    for (int i = 1; i < num_nodes; ++i) {
        depth[i] = 1;
        pred[i] = ROOT;
        thread[i] = i + 1;
        if (tree_arc_dir[i] == ARC_DIR_DOWN) {
            potential[i] = network->total_cost;
        } else {
            potential[i] = -network->total_cost;
        }
    }
    
    //最后一个指向ROOT
    thread[num_nodes - 1] = ROOT;
    
}
long SpnTree::updateSpnTree(const vector<Network_Arc*> &FrontArcs, const vector<Network_Arc*> &BackArcs, long theta, Network_Arc *entering, Network_Arc* leaving, int common_predecessor)
{
    long delta_cost = 0;
    if (theta != 0) {
        size_t size = FrontArcs.size();
        for (size_t i = 0; i < size; ++i) {
            Network_Arc* pArc = FrontArcs[i];
            pArc->flow += theta;
            if (!pArc->bArtificial) {
                //if (dynamicCost) {
                  //  network->UpdateDynamicCost(*it);
                //}else
                    delta_cost += theta * pArc->cost;
            }
            
        }
        size = BackArcs.size();
        for (size_t i = 0; i < size; ++i) {
            Network_Arc* pArc = BackArcs[i];
            pArc->flow -= theta;
            if (!pArc->bArtificial) {
                //if (dynamicCost) {
                    //network->UpdateDynamicCost(*it);
                //}else
                    delta_cost -= theta * pArc->cost;
            }
        }
    }
    
    if (leaving->flow == 0) {
        if (!leaving->bArtificial) {
            leaving->state = ARC_STATE_L;
        } else {
            network->nodes[leaving->from]->list_arc_out.remove(leaving);
        }
    } else {
        leaving->state = ARC_STATE_U;
    }
    
    update_tree(entering, leaving, common_predecessor);
    
    return delta_cost;
}
void SpnTree::update_tree(Network_Arc* entering, Network_Arc* leaving, int join)
{
    if (entering == leaving) {
        return;
    }
    
    int b_leave, a_leave, b_enter = -1, a_enter = -1;

    if (pred[leaving->from] == leaving->to) {
        b_leave = leaving->to;
        a_leave = leaving->from;
    } else {
        b_leave = leaving->from;
        a_leave = leaving->to;
    }

    int i = a_leave;
    

    i = entering->to;
    while (depth[i] > depth[join]) {
        if (i == leaving->from || i == leaving->to) {
            a_enter = entering->to;
            b_enter = entering->from;
            break;
        }
        i = pred[i];
    }
    i = entering->from;
    while (depth[i] > depth[join]) {
        if (i == leaving->from || i == leaving->to) {
            a_enter = entering->from;
            b_enter = entering->to;
            break;
        }
        i = pred[i];
    }
    
    update_threadAndparent(a_leave, b_leave, a_enter, b_enter, join);
    update_tree_arc_dir(a_leave, a_enter, entering);
    update_tree_depthAndpotential(b_enter);
    /*if (leaving->bArtificial) {
        delete leaving;
    }*/
}


void  SpnTree::update_threadAndparent(int a_leave, int b_leave, int a_enter, int b_enter, int join)
{
    bool parent_first = false;
    int i;
    int first = -1;
    int last;
    int right;
    
    int stem, newStem, predStem;
    
    if (join == b_leave) {

        i = pred[join];
        

        if (i == -1) {
            for (int index = 0; index < network->total_nodes; ++index) {
                if (thread[index] == ROOT) {
                    i = index;
                    break;
                }
            }
            
        }
        
        while (thread[i] != a_enter && thread[i] != b_enter) {
            i = thread[i];
        }
        
        if (thread[i] == b_enter) {
            parent_first = true;
        }
        
        while (thread[i] != a_leave) {
            i = thread[i];
        }
        first = i;
    }
    
    i = a_enter;
    

    while (depth[thread[i]] > depth[a_enter]) {
        i = thread[i];
    }
    right = thread[i];

    
    if (thread[b_enter] == a_leave) {
        last = i;
        while (depth[last] > depth[a_leave]) {
            last = thread[last];
        }
        if (last == a_leave) {
            last = thread[last];
        }
    } else {
        last = thread[b_enter];
    }
    
    thread[b_enter] = a_enter;
    stem = a_enter;
    predStem = b_enter;
    
    while (stem != a_leave) {

        thread[i] = pred[stem];
        
        i = pred[stem];
        while (thread[i] != stem) {
            i = thread[i];
        }

        thread[i] = right;
        
        newStem = pred[stem];
        pred[stem] = predStem;
        predStem = stem;
        stem = newStem;
        

        i = stem;
        while (depth[thread[i]] > depth[stem]) {
            i = thread[i];
        }
        right = thread[i];
    }
    
    thread[i] = last;
    
    if (join == b_leave) {
        if (!parent_first) {
            i = b_leave;
            while (thread[i] != a_leave) {
                i = thread[i];
            }
            thread[i] = right;
        } else if (first != b_enter) {
            thread[first] = right;
        }
    } else {
        i = b_leave;
        while (thread[i] != a_leave) {
            i = thread[i];
        }
        thread[i] = right;
    }
    
    pred[a_leave] = predStem;
}


void SpnTree::update_tree_arc_dir(int a_leave, int a_enter, Network_Arc *entering)
{
    int i = a_leave;
    
    while (i != a_enter) {
        int j = pred[i];
        tree_arcs[i] = tree_arcs[j];
        if (tree_arc_dir[j] == ARC_DIR_UP) {
            tree_arc_dir[i] = ARC_DIR_DOWN;
        } else {
            tree_arc_dir[i] = ARC_DIR_UP;
        }
        i = j;
    }
    
    tree_arcs[a_enter] = entering;
    
    if (entering->from == a_enter) {
        tree_arc_dir[a_enter] = ARC_DIR_UP;
    } else {
        tree_arc_dir[a_enter] = ARC_DIR_DOWN;
    }
}

void SpnTree::update_tree_depthAndpotential(int b_enter)
{
    int i, j;
    i = thread[b_enter];
    
    while (true) {
        j = pred[i];
        
        if (j == -1) {
            break;
        }
        
        depth[i] = depth[j] + 1;
        
        if (tree_arc_dir[i] == ARC_DIR_UP) {
            potential[i] = potential[pred[i]] - tree_arcs[i]->cost;
        } else {
            potential[i] = potential[pred[i]]
            + tree_arcs[i]->cost;
        }
        
        if (depth[i] <= depth[b_enter]) {
            break;
        } else {
            i = thread[i];
        }
        
    }
}


typedef enum E_SOLU_STATE
{
    SOLU_STATE_OPTIMAL,
    SOLU_STATE_UNBOUNDED,
    SOLU_STATE_NOT_OPTIMAL,
    SOLU_STATE_UNSOLVED
}SOLU_STATE;

class Cycle {
public:
    long theta;
    Network_Arc* blocking;
    int common_predecessor;
    vector<Network_Arc*> FrontArcs;
    vector<Network_Arc*> BackArcs;
};

class NetworkSimplex
{
private:
    Network *network;
    SpnTree *tree;
    
    SOLU_STATE solu_state;
    
    unsigned int max_list_size;
    unsigned int max_min_its;
    int current_startnode;
    
    list<Network_Arc*> candidate_list;
    Cycle cycle;
    
    SOLU_STATE major_iteration();
    void update_candidate_list();
    void getBlookArc(Network_Arc* entering);
    Network_Arc* get_best_arc();
    
public:
    int num_iterations;
    int cost;
    bool dynCost;
    inline NetworkSimplex() {
        tree = NULL;
    }
    virtual ~NetworkSimplex()
    {
        delete tree;
    }
    inline void init(Network *network, int max_list_size, int max_min_its, bool dynCost)
    {
        this->network = network;
        this->max_list_size = max_list_size;
        this->max_min_its = max_min_its;
        if (!tree) {
            tree = new SpnTree();
        }
        this->dynCost = dynCost;
    }
    SOLU_STATE compute_solution();
    void sorted_solution_arcs(list<Network_Arc*> &result); //可以优化
};


SOLU_STATE NetworkSimplex::compute_solution()
{
    num_iterations = 0;
    solu_state = SOLU_STATE_UNSOLVED;
    current_startnode = 0;
    cost = 0;
    tree->init(network,dynCost);
    while (major_iteration() == SOLU_STATE_NOT_OPTIMAL) {};
    
    for (int i = 0; i < network->total_nodes; ++i) {
        Network_Node *pNode = network->nodes[i];
        if (pNode->ori_arc_nums == pNode->list_arc_out.size()) {
            continue;
        }
        list<Network_Arc*>::iterator itr = pNode->list_arc_out.begin();
        while (itr != pNode->list_arc_out.end()) {
            Network_Arc *arc = *itr;
            if (arc->bArtificial && arc->flow>0) {
                solu_state = SOLU_STATE_UNBOUNDED;
                return solu_state;
            }else
                ++itr;
        }
    }
    return solu_state;
}


SOLU_STATE NetworkSimplex::major_iteration()
{
    update_candidate_list();
    
    //没有违规弧，最优了
    if (candidate_list.empty()) {
        solu_state = SOLU_STATE_OPTIMAL;
        return solu_state;
    }
    
    Network_Arc *best = get_best_arc();
    unsigned int removed_arcs = 0;
    unsigned int cout_zero = 0;
    
    while (best != NULL && removed_arcs < max_min_its) {
        getBlookArc(best);
        
        if (cycle.blocking == NULL) {
            solu_state = SOLU_STATE_UNBOUNDED;
            return solu_state;
        }
        
        best->state = ARC_STATE_T;
#ifdef _BUG_CHECK_
        long fbefor = best->flow;
#endif
        long delCost = tree->updateSpnTree(cycle.FrontArcs, cycle.BackArcs, cycle.theta, best, cycle.blocking, cycle.common_predecessor);
#ifdef _BUG_CHECK_
        assert(cycle.theta>=0);
        if (fbefor==0 && cycle.blocking != best && cycle.theta>0) {
            assert(best->flow>0);
        }
#endif
        cost += delCost;
        num_iterations++;
        
        removed_arcs++;
        
        if (cycle.theta == 0) {
            ++cout_zero;
            if (cout_zero == 12 ) {
                break;
            }
        }
        
        best = get_best_arc();
    }
    //solu_state = SOLU_STATE_NOT_OPTIMAL;
    return SOLU_STATE_NOT_OPTIMAL;
}


void NetworkSimplex::update_candidate_list()
{
    candidate_list.clear();
    
    int start_node = current_startnode;
    while (candidate_list.size() < max_list_size) {
        Network_Node *node = network->nodes[current_startnode];
        
        for (list<Network_Arc*>::iterator it = node->list_arc_out.begin();
             it != node->list_arc_out.end(); ++it) {
            Network_Arc *arc = (*it);
            if (arc->state == ARC_STATE_T) {
                continue;
            }
            
            if (arc->bArtificial) {
                continue;
            }
            
            /*if (arc->cost == INF) {
                continue;
            }*/
            /*long reduced_cost = tree->potential[arc->from]
            - tree->potential[arc->to] + arc->cost;*/
            
            long reduced_cost = tree->potential[arc->from]
            - tree->potential[arc->to] + arc->cost;
            //DOUBLE_ZERO(reduced_cost);
            
            if (reduced_cost < 0 && arc->flow == 0) {
                arc->estimate_value = -reduced_cost;
                candidate_list.push_back(arc);
            } else if (reduced_cost > 0 && arc->flow > 0) {
                arc->estimate_value = reduced_cost;
                candidate_list.push_back(arc);
            }
        }
        
        ++current_startnode;
        
        if (current_startnode >= network->total_nodes) {
            current_startnode = 0;
        }
        
        if (current_startnode == start_node) {
            break;
        }
    }
}
void NetworkSimplex::getBlookArc(Network_Arc* entering)
{
    int predBackwards = -1;
    int predForwards = -1;
    bool blockingIsFromBackward = false;
    cycle.theta = INF;
    cycle.blocking = NULL;
    cycle.FrontArcs.clear();
    cycle.BackArcs.clear();
    
    
    if (entering->state == ARC_STATE_L) {
        blockingIsFromBackward = true;
        predBackwards = entering->from;
        predForwards = entering->to;
        if (entering->cap != INF) {
            cycle.blocking = entering;
            cycle.theta = entering->cap - entering->flow;
        }
        cycle.FrontArcs.push_back(entering);
    } else {
        blockingIsFromBackward = true;
        predBackwards = entering->to;
        predForwards = entering->from;
        cycle.blocking = entering;
        cycle.theta = entering->flow;
        cycle.BackArcs.push_back(entering);
    }
    
    
    //遍历整个圈，寻找能够满足限制条件的最大发送流量
    while (predBackwards != predForwards) {
        
        if (tree->depth[predBackwards] > tree->depth[predForwards]) {
            if (tree->tree_arc_dir[predBackwards] == ARC_DIR_UP) {
                Network_Arc *arc = tree->tree_arcs[predBackwards];
                cycle.BackArcs.push_back(arc);
                if (arc->flow < cycle.theta || (arc->flow == cycle.theta && !blockingIsFromBackward)) {
                    cycle.theta = arc->flow;
                    cycle.blocking = arc;
                    blockingIsFromBackward = true;
                }
            } else {
                Network_Arc *arc = tree->tree_arcs[predBackwards];
                cycle.FrontArcs.push_back(arc);
                if (arc->cap != INF) {
                    long resCapacity = arc->cap - arc->flow;
                    if (resCapacity < cycle.theta || (resCapacity == cycle.theta && !blockingIsFromBackward)) {
                        cycle.theta = resCapacity;
                        cycle.blocking = arc;
                        blockingIsFromBackward = true;
                    }
                }
            }
            predBackwards = tree->pred[predBackwards];
        } else {
            if (tree->tree_arc_dir[predForwards] == ARC_DIR_DOWN) {
                Network_Arc *arc = tree->tree_arcs[predForwards];
                cycle.BackArcs.push_back(arc);
                if (arc->flow < cycle.theta || (arc->flow == cycle.theta && !blockingIsFromBackward)) {
                    cycle.theta = arc->flow;
                    cycle.blocking = arc;
                    blockingIsFromBackward = false;
                }
            } else {
                Network_Arc *arc = tree->tree_arcs[predForwards];
                cycle.FrontArcs.push_back(arc);
                if (arc->cap != INF) {
                    long resCapacity = arc->cap - arc->flow;
                    if (resCapacity < cycle.theta || (resCapacity == cycle.theta && !blockingIsFromBackward)) {
                        cycle.theta = resCapacity;
                        cycle.blocking = arc;
                        blockingIsFromBackward = false;
                    }
                }
            }
            predForwards = tree->pred[predForwards];
        }
    }
    cycle.common_predecessor = predForwards;
}


Network_Arc* NetworkSimplex::get_best_arc()
{
    Network_Arc *best = NULL;
    long bestReduceCost = 0;
    
    list<Network_Arc*>::iterator it = candidate_list.begin(), it_s;
    while (it != candidate_list.end()) {
        Network_Arc *arc = (*it);
        
        /*long reducedCost = tree->potential[arc->from]
        - tree->potential[arc->to] + arc->cost;*/
        
        long reducedCost = tree->potential[arc->from]
        - tree->potential[arc->to] + arc->cost;
        
        //DOUBLE_ZERO(reducedCost);
        
        if (arc->flow == 0) {
            if (reducedCost >= 0) {
                arc->estimate_value = 0;
            } else {
                arc->estimate_value = -reducedCost;
            }
        } else {
            if (reducedCost <= 0) {
                arc->estimate_value = 0;
            } else {
                arc->estimate_value = reducedCost;
            }
        }
        
        if (arc->estimate_value > bestReduceCost) {
            best = arc;
            it_s = it;
            bestReduceCost = arc->estimate_value;
        }
        ++it;
    }
    if (best) {
        candidate_list.erase(it_s);
    }
    
    return best;

}

void NetworkSimplex::sorted_solution_arcs(list<Network_Arc*> &result)
{
    result.clear();
    for (int i = 0; i < network->total_nodes; ++i) {
        for (list<Network_Arc*>::iterator it = network->nodes[i]->list_arc_out.begin();
             it != network->nodes[i]->list_arc_out.end(); ++it) {
            Network_Arc *arc = (*it);
            if (arc->flow > 0) {
                result.push_back(arc);
            }
        }
    }
}


#include <set>


//判断from到to是否有路径
bool robot_can_reach_goal(int *map, int m, int from, int to)
{
    #define ID(i,j) ((i)*(m) + (j))
    #define MAP(i,j) (*(map + (i)*(m) +(j)))
    int total_nodes = m*m;
    int start_node = ID(m-1,0);
    int end_node = ID(0,m-1);
    Network *net = new Network(total_nodes,0);
    NetworkSimplex *simplex = new NetworkSimplex;
    for (int i = 0; i < total_nodes; ++i)
    {
        net->add_node(i);
    }
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int cur_state = MAP(i,j);
            if (cur_state == 1)  continue;  //障碍
            int curid = ID(i,j);

            int id_next;
            //up
            if (i - 1 >= 0)
            {
                id_next = ID(i-1,j);
                net->add_orig_arc(curid, id_next,1,1);
            }
            //right
            if (j + 1 < m)
            {
                id_next = ID(i, j+1);
                net->add_orig_arc(curid, id_next,1,1);
            }
        }
    }

    //设置起点的流量为输出1
    net->change_node_demand(start_node,1);
    //设置终点的流量为输入1
    net->change_node_demand(end_node,-1);
    simplex->init(net,100,100,false);
    SOLU_STATE state =  simplex->compute_solution();
    bool res = false;
    if (state == SOLU_STATE_OPTIMAL)
        res = true;
    else
        res = false;
    delete net;
    delete simplex;
    return res;
}

//计算从起点往终点走所有能够达到的点
void robot_front(int *map, int m, set<int> &canReachSet)
{
    #define ID(i,j) ((i)*(m) + (j))
    #define MAP(i,j) (*(map + (i)*(m) +(j)))
    int total_nodes = m*m;
    int start_node = ID(m-1,0);
    int end_node = ID(0,m-1);
    Network *net = new Network(total_nodes,0);
    NetworkSimplex *simplex = new NetworkSimplex;
    for (int i = 0; i < total_nodes; ++i)
    {
        net->add_node(i);
    }
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int cur_state = MAP(i,j);
            int next_state;
            if (cur_state == 1)  continue;  //障碍
            int curid = ID(i,j);
            //cantSet.insert(curid);
            net->add_orig_arc(curid, end_node, -1 , 1);  //添加每一个节点到达终点的边，且cost为 -1 ，cap为1


            int id_next;
            //up
            if (i - 1 >= 0)
            {
                id_next = ID(i-1,j);
                next_state = MAP(i-1,j);
                if (next_state!=1)
                    net->add_orig_arc(curid, id_next,0,total_nodes*100);
                //net->add_orig_arc(id_next, curid,1,1);
            }
            //right
            if (j + 1 < m)
            {
                id_next = ID(i, j+1);
                next_state = MAP(i,j + 1);
                if (next_state!=1)
                    net->add_orig_arc(curid, id_next,0,total_nodes*100);
                //net->add_orig_arc(id_next, curid,1,1);
            }
        }
    }

    net->add_orig_arc(end_node, start_node,-1,0x8000000);
    //net->add_orig_arc(start_node, end_node,0,1);
    //net->change_node_demand(start_node,0);
    //net->change_node_demand(end_node,0);
    simplex->init(net,100,100,false);
    SOLU_STATE state =  simplex->compute_solution();

    list<Network_Arc*> lst;
    simplex->sorted_solution_arcs(lst);
    list<Network_Arc*>::iterator itr = lst.begin();
    int sum = 0;
    for (; itr != lst.end(); ++itr)
    {
        Network_Arc * pArc = *itr;
        if (pArc->to == end_node)
        {
            canReachSet.insert(pArc->from);
        }
            
    }
    if (robot_can_reach_goal(map,m,start_node, end_node))
        canReachSet.insert(end_node);
    delete net;
    delete simplex;
}

//计算从终点往回走，能够到达的所有点
void robot_reverse(int *map, int m,set<int>& canReach_set)
{
    #define ID(i,j) ((i)*(m) + (j))
    #define MAP(i,j) (*(map + (i)*(m) +(j)))
    int total_nodes = m*m;
    //set<int> set_not_wall;
    int end_node = ID(m-1,0);
    int start_node = ID(0,m-1);
    Network *net = new Network(total_nodes,0);
    NetworkSimplex *simplex = new NetworkSimplex;
    for (int i = 0; i < total_nodes; ++i)
    {
        net->add_node(i);
    }
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int cur_state = MAP(i,j);
            int next_state;
            if (cur_state == 1)  continue;  //障碍
            int curid = ID(i,j);
            net->add_orig_arc(curid, end_node, -1 , 1);  //添加每一个节点到达终点的边，且cost为 -1 ，cap为1，用来让尽量的每一个点都向终点输送流量


            int id_next;
            //up
            if (i - 1 >= 0)
            {
                id_next = ID(i-1,j);
                next_state = MAP(i-1,j);
                if (next_state!=1)
                    net->add_orig_arc(id_next, curid,0,total_nodes*100);//添加反向边,从终点往会走
            }
            //right
            if (j + 1 < m)
            {
                id_next = ID(i, j + 1);
                next_state = MAP(i, j + 1);
                if (next_state!=1)
                    net->add_orig_arc(id_next, curid,0,total_nodes*100);//添加反向边
                    
            }
        }
    }

    net->add_orig_arc(end_node, start_node,-1,0x8000000);
    //net->add_orig_arc(start_node, end_node,0,1);
    simplex->init(net,100,100,false);
    SOLU_STATE state =  simplex->compute_solution();

    list<Network_Arc*> lst;
    simplex->sorted_solution_arcs(lst);
    list<Network_Arc*>::iterator itr = lst.begin();
    int sum = 0;
    for (; itr != lst.end(); ++itr)
    {
        Network_Arc * pArc = *itr;
        if (pArc->to == end_node)  //如果这个点有流量指向终点，则表示这个点肯定能够从起点开始到达
        {
            canReach_set.insert(pArc->from);
        }
            
    }
    if (robot_can_reach_goal(map,m,start_node,end_node))
        canReach_set.insert(end_node);
    delete net;
    delete simplex;
}

void getAvalablePoint(int *map, int m,set<int> &total_set)
{
    #define ID(i,j) ((i)*(m) + (j))
    #define MAP(i,j) (*(map + (i)*(m) +(j)))
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int cur_state = MAP(i,j);
            if (cur_state == 1)  continue;  //障碍
            total_set.insert(ID(i,j));
        }
    }
}
#include <algorithm>


void set_intersection(set<int> &set1, set<int>&set2, set<int> &set_res)
{
    set_res.clear();
    set_intersection(set1.begin(),set1.end(),set2.begin(),set2.end(),inserter(set_res,set_res.begin()));
}
void set_union(set<int> &set1, set<int>&set2, set<int> &set_res)
{
    set_res.clear();
    set_union(set1.begin(),set1.end(),set2.begin(),set2.end(),inserter(set_res,set_res.begin()));
}
void set_difference(set<int> &set1, set<int>&set2, set<int> &set_res)
{
    set_res.clear();
    set_difference(set1.begin(),set1.end(),set2.begin(),set2.end(),inserter(set_res,set_res.begin()));
}
int main()
{
    int map[] = {0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,1,1,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,
                 1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,
                 1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,
                 0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1
                };
    /*int map[] = {0,1,0,1,0,
                 1,0,1,0,0,
                 0,0,0,0,1,
                 1,0,1,0,0,
                 0,0,0,0,1

    };*/
    #define ID(i,j) ((i)*(m) + (j))
    set<int> canReachSet_front;
    set<int> canReachSet_reverse;
    set<int> total_Set;
    cout << sizeof(map) << endl;
    getAvalablePoint(map,50,total_Set);
    cout << "can't reach set\n";

    //查找前向能够到达的所有点
    robot_front((int*)map, 50, canReachSet_front);
    set<int>res1;
    //从所有可行点中，删除前向能够到达的点，剩下的就是所有不能达到的点了
    set_difference(total_Set,canReachSet_front,res1);
    set<int>::iterator itr = res1.begin();
    for (; itr != res1.end(); ++itr)
    {
        cout << *itr<<endl;
    }
    cout << "trap set = \n";
    robot_reverse((int*)map, 50, canReachSet_reverse);


    //从所有可行点中，删除前向能够到达的点，剩下的就是所有不能达到的点了
    set_difference(total_Set,canReachSet_reverse,res1);  //所有后向不能达到的点
    set<int> res2;
    set_intersection(canReachSet_front,res1,res2);
    itr = res2.begin();
    for (; itr != res2.end(); ++itr)
    {
        cout << *itr<<endl;
    }

}