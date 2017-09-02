#include <iostream>
typedef struct s_Node
{
	struct s_Node *left;
	struct s_Node *right;
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
        root->left = createTree(level - 1);
        root->right = createTree(level - 1);
        root->pData = new int(level);
        return root;
	} else
	    return NULL;
}

int getValue(PNode root)
{
    if (root != NULL)
    {
    	
    	getValue(root->left);
    	std::cout<<*(root->pData)<<std::endl;
    	getValue(root->right);
    }
}

int delTree(PNode root)
{
if (root != NULL)
{
	delTree(root->left);
	delTree(root->right);
	delete root->pData;
	delete root;
}
}

int main()
{
	PNode root = createTree(3);
	getValue(root);
	delTree(root);
	return 0;
}