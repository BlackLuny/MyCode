#include <iostream>
#include <vector>
#include <map>
using namespace std;

//测试迭代器的定义，其实迭代器就是某个容器内部的一个public的类
class test_iterator
{
public:
	class iterator
	{
	public:
		iterator(){};
	};

	test_iterator(){};

};

void test_const_iterator()
{
	cout << "Begin test_const_iterator():\n";
	vector<int> vec;
	vec.push_back(10);
	vec.push_back(12);
	vec.push_back(20);
	//迭代器的值是常数 即 *it 不能修改
	for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		cout << *it << endl;
		//*it =100;//错误
	}

	//常数迭代器
	const vector<int>::iterator it = vec.begin();
	cout << *it << endl;
	//it++; //错误

	//迭代器之间加减运算
	const vector<int>::iterator it1 = vec.begin();
	vector<int>::iterator it2 = vec.end();
	size_t newIt = it2 - it1;  //相减的类型是int？,通过反汇编发现其实是直接newIt = (it2 - it1) / 4;
	cout << "it2 = " << &(*it2) << endl;  //0x7fa16040267c
	cout << "it1 = " << &(*it1) << endl;  //0x7fa160402670
	cout << newIt << endl;
}

void test_map_iterator()
{
	//map中的iterator 是Forward Iterator
	cout << "Begin test_map_iterator()\n";
	map<int,int> myMap;
	myMap[1] = 100;
	myMap[2] = 20;
	map<int,int>::iterator it1 = myMap.begin();
	//map<int,int>::iterator it2 = it1 + 1; //错误，非线性容器是没有重载+数字的，只重载了++
	map<int,int>::iterator it2 = it1++;
	--it1;
	cout << (*it1).first << " " << (*it1).second << endl;
	cout << (*it2).first << " " << (*it2).second << endl;

}

int main()
{
	test_iterator a;
	test_iterator::iterator itr;
	test_const_iterator();
	test_map_iterator();
	typedef typename vector<int>::iterator iterator1;

	return 0;
}