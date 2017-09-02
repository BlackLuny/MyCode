#include <iostream>
#include <string>
using namespace std;

template <class T1,class T2> 
struct mypair
{
	T1 first;
	T2 second;
	mypair(const T1 &t1, const T2& t2):first(T1(t1)),second(T2(t2)){};
	//mypair():first(T1()),second(T2()){};
	mypair(const mypair<T1,T2> & mp):first(mp.first),second(mp.second)
	{
		cout << "copy \n";
	}
	mypair<T1,T2> &operator=(const mypair<T1,T2> &rh)
	{
		this->first = rh.first;
		this->second = rh.second;
		cout <<"operator = \n";
		return *this;
	}
};
ostream &operator<<(ostream & os, mypair<string,int> &pairs)
{
	return os << pairs.first << " " << pairs.second << endl;
}
int main()
{
	mypair<string,int> pairs("a",1);
	cout <<"stub1\n";
	mypair<string,int> pairs2(pairs);
	cout <<"stub2\n";
	pairs2 = pairs;
	cout << pairs2 << endl;
	return 0;
}