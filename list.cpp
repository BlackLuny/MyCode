#include <iostream>
#include <list>
using namespace std;
//list只重载了++it 和 it++运算符，没有it+=2这些，也就是说只能够往list的下一个走
//删除m_val 最多 times次
struct s_remove_if
{
	int m_val;
	int times;
	int cur_times;
	s_remove_if(int value, int times):m_val(value),times(times),cur_times(0){};
	bool operator()(const int& rh)
	{
		if (rh == m_val && cur_times < times)
		{
			cur_times++;
			return true;
		}
		return false;
	}
};

bool cmp_greater(const int& lh , const int& rh)
{
	return lh > rh;
}


int main(int argc, char* argv[])
{
	list<int> mylist;
	mylist.push_back(10);
	mylist.push_back(30);
	mylist.push_front(20);
	list<int>::iterator it;
	mylist.sort(cmp_greater);
	for(it = mylist.begin(); it != mylist.end(); it++)
	{
		cout<< *it << endl;//30 20 10
	}
	cout<<endl;


	mylist.sort();
	for(it = mylist.begin(); it != mylist.end(); it++)
	{
		cout<< *it <<endl;  //10 20 30
	}
	cout<<endl;

	mylist.push_back(10);
	mylist.push_back(10);
	s_remove_if rem(10,1);//删除10 最多1次
	mylist.remove_if(rem);
	for(it = mylist.begin(); it != mylist.end(); it++)
	{
		cout<< *it <<endl;  //20 30 10 10
	}
	cout<<endl;

	mylist.push_back(10);
	mylist.push_back(10);
	s_remove_if rem2(10,10);//删除10 最多10次
	mylist.remove_if(rem2);
	for(it = mylist.begin(); it != mylist.end(); it++)
	{
		cout<< *it <<endl;  //20 30
	}
	cout<<endl;

	mylist.push_back(10);
	mylist.push_back(10);
	mylist.remove(10);  //删除所有的10
	for(it = mylist.begin(); it != mylist.end(); it++)
	{
		cout<< *it <<endl;  //20 30
	}


	return 0;
}