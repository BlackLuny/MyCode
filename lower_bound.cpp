#include <iostream>
#include <vector>
using namespace std;

void test_lower_bound()
{
	cout << "Begin test_lower_bound()\n";
	vector<int> vec;
	vec.push_back(100);
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(20);
	vec.push_back(200);
	vec.push_back(300);
	sort(vec.begin(), vec.end());
	vector<int>::iterator itr_insert;
	itr_insert = lower_bound(vec.begin(), vec.end(), 20);
	cout << "find " << *itr_insert << " " << *(itr_insert+1)<< endl;  //20  返回第一个大于等于value的iterator,如果没有，则返回end()
	itr_insert = lower_bound(vec.begin(), vec.end(), 19);
	cout << *itr_insert << endl;   // 20
	vec.insert(itr_insert,21);   //在指定位置前插入
	for (int i = 0; i < vec.size(); ++i)
		cout << vec[i] << endl;
	itr_insert = lower_bound(vec.begin(), vec.end(), 400);
	if (itr_insert == vec.end()) cout << "end()\n";
	cout << *itr_insert << endl;  // 0
}


void test_upper_bound()
{
	cout << "Begin test_upper_bound()\n";
	vector<int> vec;
	vec.push_back(100);
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(20);
	vec.push_back(21);
	vec.push_back(200);
	vec.push_back(300);
	sort(vec.begin(), vec.end());
	vector<int>::iterator itr_insert;
	itr_insert = upper_bound(vec.begin(), vec.end(), 20);
	cout << "find " << *itr_insert << endl;  //20  返回第一个大于value的iterator,如果没有，则返回end()
	itr_insert = upper_bound(vec.begin(), vec.end(), 19);
	cout << *itr_insert << endl;   // 20
	vec.insert(itr_insert,21);   //在指定位置前插入
	for (int i = 0; i < vec.size(); ++i)
		cout << vec[i] << endl;
	itr_insert = upper_bound(vec.begin(), vec.end(), 400);
	if (itr_insert == vec.end()) cout << "end()\n";
	cout << *itr_insert << endl;  // 0
}
int main()
{
	test_lower_bound();
	test_upper_bound();
	return 0;
}