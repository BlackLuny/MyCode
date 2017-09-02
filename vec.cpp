#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>

using namespace std;

void split(char *str, char *sp, vector<string> &vec_out)
{
	vec_out.clear();
	string tmp = str;
	int index = 0;
	size_t len = tmp.size();
	while (index < len)
	{
		int index_next = tmp.find_first_of(sp,index);
		if (index_next == -1)
		{
			vec_out.push_back(tmp.substr(index, len - index));
			break;
		}
		else{
			vec_out.push_back(tmp.substr(index, index_next - index));
		}
		index = index_next + 1;
	}
}

void string_test()
{
	string tmpStr = "abc-";
	string::iterator it;
	for (it = tmpStr.begin(); it < tmpStr.end(); ++it)
	{
		cout << (void*)&(*it)<<endl;
	}
	//cout << tmpStr.c_str()<<endl;
	
	const char *tmp_str = tmpStr.c_str();
	cout << (void*)(tmp_str) << endl;

	tmpStr = "123111111";
	cout << (void*)(tmp_str) << endl;

	cout << tmpStr[1] << endl;   //可以直接打印指定位置的字符；
	cout <<  (void*)tmpStr.find_first_of("01",0) << endl;  //从字符串中查找第一个出现的0或者1，的index，不存在返回 -1
	cout << "substr()" << tmpStr.substr(0,4) << endl;  //获取从index 0 开始之后的 4 个字符组成的子串

	tmpStr.resize(100);
	cout << "size = " << tmpStr.size() << endl;
	cout << "length = " << tmpStr.length() << endl;
}
void vector_test()
{
	cout << "Begin vector test:\n";
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(10);
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(4);
	int a = 10;
	cout <<" &a = " << (void *)(&a) << endl;
	cout <<" &vec = " << (void *)(&vec) << endl;
	cout <<" &(vec[0]) = " << (void *)(&(vec[0])) << endl;
	cout <<" &(vec[1]) = " << (void *)(&(vec[1])) << endl;
	vector<int>::iterator itr;
	for (itr = vec.begin(); itr < vec.end();)
	{
		int cur = *itr;
		cout << "cur = " << &(*itr) << endl;
		if (cur == 4)
		{
			itr = vec.erase(itr);  //删除一个元素 erase会返回一个iterator
			continue;
		}
		itr++;
	}
	cout << "meth1:" << endl;
	for ( itr = vec.begin(); itr != vec.end(); ++itr)
	{
		cout << *itr << endl;
	}

	vec.clear();
	vec.push_back(1);
	vec.push_back(10);
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(4);
	for (itr = vec.begin(); itr < vec.end();)
	{
		int cur = *itr;
		cout << "cur = " << &(*itr) << endl;
		if (cur == 4)
		{
			vec.erase(itr);  //删除一个元素 erase会返回一个iterator
			continue;
		}
		itr++;
	}
	cout <<"meth2:" << endl;
	for ( itr = vec.begin(); itr != vec.end(); ++itr)
	{
		cout << *itr << endl;
	}
}

void list_test()
{
	cout << "Begin list test:" << endl;
	list<int> lst;
	lst.push_back(5);
	lst.push_back(10);
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(100);
	lst.push_back(4);
	list<int>::iterator itr;
	for (itr = lst.begin(); itr != lst.end();)
	{
		/* code */
		cout << "cur = " << &(*itr) << endl;
		if (*itr == 10)
		{
			itr = lst.erase(itr);  //等效于 lst.erase(itr++);
			//itr++;
			cout << "del cur = " << &(*itr) << endl;
			continue;
		}
		itr++;
			
	}
	cout << "List meth1 :" << endl;
	for (itr = lst.begin(); itr != lst.end(); ++itr)
	{
		cout << *itr << endl;
	}
}

void set_test()
{
	cout << "Begin set test:" << endl;
	set<int> myset;
	myset.insert(1);
	myset.insert(2);
	myset.insert(2);
	myset.insert(3);
	for (set<int>::iterator itr = myset.begin(); itr != myset.end(); )
	{
		cout << "cur = " << &(*itr) << endl;
		if (*itr == 2)
		{
			itr = myset.erase(itr);
			cout << "cur1 = " << &(*itr) << endl;
			continue;
		}
		++itr;
	}
	if (myset.find(2) != myset.end())  //set查找
		cout << "error \n";
	for (set<int>::iterator itr = myset.begin(); itr != myset.end(); ++itr)
	{
		cout <<  *itr << endl;
	}
}

bool cmp_key(const pair<string,int> &lh, const pair<string,int> &rh)
{
	return lh.first < rh.first;
}

bool cmp_value(const pair<string,int> &lh, const pair<string,int> &rh)
{
	return lh.second < rh.second;
}
struct s_cmp_key
{
	bool operator()(const pair<string,int> &lh,const pair<string,int> &rh) const
	{
		return lh.second < rh.second;
	}
};
void map_test()
{
	cout << "Begin map test:" << endl;
	map<string,int, less<string> > mymap;
	mymap.insert(make_pair<string,int>("abc",1));
	mymap["bcd"] = 2;
	mymap["def"] = 5;
	mymap["cde"] = 0;
	
	for (map<string ,int>::iterator itr = mymap.begin(); itr != mymap.end(); ++itr)
	{
		pair<string,int> tmp = *itr;  //使用pair进行访问
		cout << tmp.first << " " << tmp.second<< endl;
	}

	//按照key值进行排序
	vector< pair<string,int> > vec(mymap.begin(),mymap.end());
	sort(vec.begin(), vec.end(), s_cmp_key());
	for (int i = 0; i < vec.size(); ++i)
		cout << vec[i].first << vec[i].second << endl;
	//按照value值进行排序，不能用sort()因为sort只能针对线性容器进行排序
	vec.clear();
	vec = vector< pair<string,int> >(mymap.begin(),mymap.end());
	sort(vec.begin(), vec.end(), cmp_value);
	for (int i = 0; i < vec.size(); ++i)
		cout << vec[i].first << vec[i].second << endl;

}

int main()
{
	string_test();
	vector<string> tmp;
	split("111","+-", tmp);
	for (int i = 0; i < tmp.size(); ++i)
	{
		/* code */
		cout << tmp[i] <<endl;
	}
	vector_test();
	list_test();
	set_test();
	map_test();
	return 0;
}