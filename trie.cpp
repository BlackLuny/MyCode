#include <iostream>
class CTrie
{
public:
	typedef struct trie
	{
	struct trie **next;  //下一个字母
	int v;              //数据区
	}*PTrie,Trie;

	PTrie root;
	int max_cnt;
	CTrie():max_cnt(52)
	{
		root = createNode(max_cnt);
		root->v = 1;
	}
//检查字符串是否有不符合规则的
bool checkStr(const char *str)  
{
	char c;
	while(c = (*str++))
	{	
		if (!(c >= 'a' && c <='z' || c >= 'A' && c <= 'Z')) return false;
	}
	return true;
}
//在字典书中增加一个单词
bool createTrie(const char *str)
{
	PTrie p = root;
	if (!checkStr(str)) return false;
	while(*str != '\0')
	{
		char cur = *str;
		int id = cur - 'A';
		if (p->next[id] == NULL)
		{
			PTrie q = createNode(max_cnt);
			q->v = 1;
			p->next[id] = q;
			p = q;
		}else
		{
			(p->next[id]->v)++;
			p = p->next[id];
		}
		str++;
	}
	p->v= -1;
	return true;
}
CTrie &operator<<(char *str)
{
	if (createTrie(str))
		return *this;
	else
		throw -1;
}
int findTrie(const char *str)
{
	if(!checkStr(str)) return 0;
	PTrie p = root;
	while (*str != '\0')
	{
		int id = *str - 'A';
		if (p->next[id] == NULL )
			return -1;
		else if (p->next[id]->v == -1)
			return 0;
		p = p->next[id];
		str++;
	}
	return 0;
}
~CTrie()
{
	clearTrie(root);
}
private:
	void clearTrie(PTrie root)  //清理内存
	{
		PTrie p = root;
		if (p == NULL) return;
		for (int i = 0; i < max_cnt; ++i)
		{
			if (p->next[i] != NULL)
				clearTrie(p->next[i]);

		}
		free(p->next);
		free(p);
	}	
	static PTrie createNode(int maxNext)
	{
		PTrie node = (PTrie)malloc(sizeof(Trie));
		node->next = (PTrie*)malloc(sizeof(PTrie) * maxNext);
		for (int i = 0; i < maxNext; ++i)
		{
			node->next[i] = NULL;
		}
		return node;
	}
};
using namespace std;

int main()
{
	for (int i = 0; i < 1; ++i)
	{
	CTrie t;
	t<<("abcdefg")<<"aabbcc";
	cout << t.findTrie("aac") << endl;
	}

	return 0;
}