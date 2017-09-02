#include <iostream>
#include <cstring>
using namespace std;

const int MAX_SIZE = 100;
const int INF      = 1 << 30;

int capacityGraph[MAX_SIZE][MAX_SIZE];
int flowMap[MAX_SIZE][MAX_SIZE];
int height[MAX_SIZE];
int excess[MAX_SIZE];
int src, des;
int vertex_num, edge_num;


void init()
{    
    memset( capacityGraph, 0, sizeof( capacityGraph ) );
    memset( flowMap,       0, sizeof( flowMap ) );
    memset( height,        0, sizeof( height ) );
    memset( excess,        0, sizeof( excess ) );
    
    cout << "Enter vertex num && edge num : ";
    cin >> vertex_num >> edge_num;
    
    for( int i = 1; i <= edge_num; ++i )
    {
        int start, end, cap;
        cout << "Enter start && end && capacity : ";
        cin >> start >> end >> cap;
        
        capacityGraph[start][end] = cap;
    }
    
    src         = 1;
    des         = vertex_num;
    height[src] = vertex_num;
}


void preFlow()
{
    for( int i = src; i <= des; ++i )
    {
        if( capacityGraph[src][i] > 0 )
        {
            const int flow  = capacityGraph[src][i];
            flowMap[src][i] += flow;
            flowMap[i][src] = - flowMap[src][i];
            excess[src]     -= flow;
            excess[i]       += flow;
        }
    }
}


void push( int start, int end )
{
    int flow = min( excess[start], capacityGraph[start][end] - flowMap[start][end] );
    flowMap[start][end] += flow;
    flowMap[end][start] = -flowMap[start][end];
    excess[start]       -= flow;
    excess[end]         += flow;
}


bool reLabel( int index )
{
    int minestHeight = INF;
    
    for( int i = src; i <= des; ++i )
    {
        if( capacityGraph[index][i] - flowMap[index][i] > 0 )
            minestHeight = min( minestHeight, height[i] );
    }
    
    if( minestHeight == INF ) 
        return false;
    
    height[index] = minestHeight + 1;
    
    for( int i = src; i <= des; ++i )
    {
        if( excess[index] == 0 )
            break;
        if( height[i] == minestHeight && capacityGraph[index][i] > flowMap[index][i] )
            push( index, i );
    }
    
    return true;
}


void pushReLabel()
{
    bool flag = true;
    
    preFlow();
    
    while( true )
    {
        if( flag == false ) 
            break;
            
        flag = false;
        
        for( int i = src; i <= des - 1; ++i )
        {
            if( excess[i] > 0 ) flag = flag || reLabel( i );
        }
    }
}


int main()
{
    init();
    pushReLabel();
    
    cout << "max flow : " << excess[des] << endl;
    
    return 0;
}