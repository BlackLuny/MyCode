#include <iostream>
template <class T = std::size_t>
class Adjacency
{
	typedef T Value;  //方便外面能够访问这种类型,因为T是不能够在类Adjacency<T>中访问的，所以需要这样写
public:
	Adjacency(const Value);

	template <class InerType>   //支持任意类型的构造函数
	Adjacency(InerType v1,int i = 100){};  
	bool operator<(const Value&) const;
	bool operator<=(const Value&) const;
	const T data()const;
private:
	T data_;
};
template <class T>
Adjacency<T>::Adjacency(const T v):data_(v){};
template <class T>
inline bool Adjacency<T>::operator<(const T& rh) const
{
	return data_ < rh.data_;
}
template <class T>
inline bool Adjacency<T>::operator<=(const T& rh) const
{
	return data_ < rh.data_;
}
template <class T1>
inline const typename Adjacency<T1>::Value Adjacency<T1>::data() const //typename告诉编译器Adjacency<T1>::Value是一种类型
{
	return data_;
}

using namespace std;
int main()
{
	Adjacency<> a(10);
	Adjacency<> x("123");
	return 0;
}