#include <iostream>
#include <vector>
using namespace std;
typedef struct s_Node
{
	vector<struct s_Node*> vec_sons;
	int *pData;
}*PNode,Node;

int insert(PNode pNode, int data)
{
     
}

PNode createTree(int level)
{
	if (level > 0)
	{
		/* code */
		PNode root = new Node;
		for (int i = 0; i < 5; ++i)
		{
			PNode tmp_son = createTree(level - 1);
			root->vec_sons.push_back(tmp_son);
		}
        root->pData = new int(level);
        return root;
	} else
	    return NULL;
}

int getValue(PNode root)
{
    if (root != NULL)
    {
    	std::cout<<*(root->pData)<<std::endl;
    	for (int i = 0; i < root->vec_sons.size(); ++i)
    	{
    		/* code */
    		getValue(root->vec_sons[i]);
    	}
    	//std::cout<<*(root->pData)<<std::endl;

    }
}

int delTree(PNode root)
{
/*if (root != NULL)
{
	delTree(root->left);
	delTree(root->right);
	delete root->pData;
	delete root;
}*/
}

int main()
{
	PNode root = createTree(3);
	getValue(root);
	delTree(root);
	return 0;
}