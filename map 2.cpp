#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
typedef pair<string,int> PAIR;
typedef map<string,int> MapType;
//使用此方法定义一个类,在调用sort()的时候需要用cmp_value()作为第三个参数；
/*struct cmp_value{
    bool operator()(const PAIR& lh, const PAIR & rh)
    {
    	return (lh.second < rh.second);
    }
};*/

 //使用此方法则只需要sort()的第三个参数为cmp_less即可
     bool cmp_less(const PAIR& lh, const PAIR & rh)
    {
    	return (lh.second < rh.second);
    }
int main(int argc, char ** argv)
{
	map<string,int> myMap;

	//map插入方法1：使用pair<string,int>()生成一个pair然后插入;
	myMap.insert(PAIR("a",1));
	myMap.insert(PAIR("b",2));
	myMap.insert(PAIR("c",2));
	myMap.insert(PAIR("d",20));
	myMap.insert(PAIR("e",5));
	myMap.insert(PAIR("a",70));  //不会修改已经存在的key的value;

	//map插入方法2：使用重载的[]进行插入，会重新修改key的value，不存在会自动添加;
	myMap["a"] = 70;

	//map插入方法3：使用make_pair<string,int>()生成pair进行插入；
	myMap.insert(make_pair<string,int>("xx",100));

	//map插入方法4：使用value_type
	myMap.insert(MapType::value_type("xy",200));
	//产生vector的方法，使用begin()和end();
	vector<PAIR> vec(myMap.begin(),myMap.end());
	sort(vec.begin(),vec.end(),cmp_less);
	for (int i = 0; i < vec.size(); ++i)
	{
		/* code */
		cout<<vec[i].first<<" "<<vec[i].second<<endl;
	}
	return 0;
}