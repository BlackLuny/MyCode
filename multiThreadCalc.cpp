#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <sstream>
using namespace std;
class calc;
template <class T>
string ouput_pack(vector<T> &vec,string op)
{
	ostringstream os;
	for (int i = 0; i < vec.size(); ++i)
	{
		if (i!=0)
			os<<op<<vec[i];
		else
			os<<vec[i];
	}
	return os.str();
}

bool cmp_vec(const vector<int> &lh, const vector<int> &rh)
{
	if (lh.size() == rh.size())
	{	
		for (int i = 0; i < lh.size(); ++i)
		{
			if (lh[i] < rh[i])
				return true;
			if (lh[i] > rh[i])
				return false;
		}
	}else{
		return lh.size() < rh.size();
	}
	return false;  //完全相同
}
class calc
{
public:
	calc():cap_add(1),cap_mul(1){};
	void input(const char *str)
	{
		vector<string> tmp;
		split(str, "+", tmp);
		for (int i = 0; i < tmp.size(); ++i)
		{
			string tmp_s = tmp[i];
			if (tmp_s.length() == 1)
			{
				//add
				vec_org_add.push_back(*tmp_s.c_str() - '0');
				//cout << *tmp_s.c_str() - '0' << endl;
			}else{
				const char *tmp_c_str = tmp_s.c_str();
				vector<int> tmp_vec;
				while(*tmp_c_str!='\0')
				{	char tmp_char = *tmp_c_str;
					if (tmp_char >= '0' && tmp_char <= '9')
						tmp_vec.push_back(*tmp_c_str - '0');
					tmp_c_str++;
				}
				sort(tmp_vec.begin(), tmp_vec.end());
				vec_org_mul.push_back(tmp_vec);
			}
		}
		sort(vec_org_add.begin(), vec_org_add.end());
		sort(vec_org_mul.begin(), vec_org_mul.end(),cmp_vec);
	}
	void setCapability(unsigned int n, unsigned int m)
	{
		cap_add = n;
		cap_mul = m;
	}
	int GetRoundResult(unsigned int Round, char*output)
	{
		string res;
		vec_add = vec_org_add;
		vec_mul = vec_org_mul;
		for (int i = 0; i < Round; ++i)
		{
			op_add();op_mul();
			if (i == Round - 1)
				res = this->output();
			update_vec();
		}
		strcpy(output,res.c_str());
	}
	unsigned int GetRound(void)
	{
		vec_add = vec_org_add;
		vec_mul = vec_org_mul;
		unsigned int res = 1;
		while(vec_add.size() != 1){
			op_add();op_mul();update_vec();
			res++;
		}
		return res;
	}
	void Clear()
	{
		vec_org_add.clear();
		vec_org_mul.clear();
		cap_add = 1;
		cap_mul = 1;
	}
private:
	unsigned int cap_add;
	unsigned int cap_mul;
	vector<int> vec_add,vec_org_add;
	vector< vector<int> > vec_mul,vec_org_mul;
	void op_add()
	{
		vector<int> tmp_vec = vec_add;
		size_t vec_size = tmp_vec.size();
		int cnt = min(cap_add,vec_size/2);
		if (cnt == 0) return;
		vec_add.clear();
		for (int i = 0; i < cnt; ++i)
		{
			int a = tmp_vec[2*i];
			int b = tmp_vec[2*i+1];
			vec_add.push_back(a+b);
			//i++;//跳过下一个
		}
		//将后面没有参与运算的还原
		for (int i = cnt*2; i < vec_size; ++i)
		{
			vec_add.push_back(tmp_vec[i]);
		}

	}
	//对一个乘法组的元素进行一次乘法运算
	void op_mul_per(vector<int> &sub_mul)
	{
		vector<int> tmp_vec = sub_mul;
		size_t vec_size = tmp_vec.size();
		int cnt = min(1,vec_size/2);
		if (cnt == 0) return;
		sub_mul.clear();
		for (int i = 0; i < cnt; ++i)
		{
			int a = tmp_vec[2*i];
			int b = tmp_vec[2*i+1];
			sub_mul.push_back(a*b);
		}
		//将后面没有参与运算的还原
		for (int i = 2 * cnt; i < vec_size; ++i)
		{
			sub_mul.push_back(tmp_vec[i]);
		}
	}
	void op_mul()
	{
		vector< vector<int> > tmp_vec = vec_mul;
		size_t vec_size = tmp_vec.size();
		int cnt = min(cap_mul,vec_size);
		if (cnt == 0) return;
		vec_mul.clear();
		for (int i = 0; i < cnt; ++i)
		{
			op_mul_per(tmp_vec[i]);
			vec_mul.push_back(tmp_vec[i]);
		}
		//将后面没有参与运算的还原
		for (int i = cnt; i < vec_size; ++i)
		{
			vec_mul.push_back(tmp_vec[i]);
		}
	}
	string output()
	{
		vector<string> tmp_vec;
		ostringstream os;
		//先填写加法的
		for (int i = 0; i < vec_add.size(); ++i)
		{
			os<<vec_add[i];
			tmp_vec.push_back(os.str());
			//cout << os.str() << endl;
			os.str("");
		}
		//再填写乘法的
		for (int i = 0; i < vec_mul.size(); ++i)
		{
			os<<ouput_pack<int>(vec_mul[i],"*");
			tmp_vec.push_back(os.str());
			os.str("");
		}
		return ouput_pack<string>(tmp_vec,"+");
	}

	//如果乘法运算之后只剩1个数，则将这个数放到加法运算符里面去，同时进行排序操作，准备进行下一次运算
	void update_vec()
	{
		vector< vector<int> >::iterator itr;
		for (itr = vec_mul.begin(); itr != vec_mul.end(); )
		{
			if ((*itr).size() == 1)
			{
				vec_add.push_back((*itr)[0]);
				itr = vec_mul.erase(itr);
				continue;
			}
			sort(((*itr)).begin(),(*itr).end());
			itr++;
		}
		sort(vec_add.begin(), vec_add.end());
		sort(vec_mul.begin(), vec_mul.end(),cmp_vec);
		//show_vec_add();show_vec_mul();
	}

	int min(const int a, const int b)
	{
		return a < b ? a : b;
	}
	void split(const char *str, char *sp, vector<string> &vec_out)
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
};

calc g_calc;

void setCapability(unsigned int N, unsigned int M)
{
	g_calc.setCapability(N, M);
}
void setInput(const char* input)
{
	g_calc.input(input);
}
unsigned int GetRound(void)
{
	return g_calc.GetRound();
}
void Clear()
{
	g_calc.Clear();
}
int main()
{
	setCapability(2,2);
	setInput("1+8+3*4+5*6*7+9*2+7*8*9*3+2");
	char str[256] = {0};
	for (int i = 0; i < GetRound(); ++i)
	{
		g_calc.GetRoundResult(i,str);
		cout << str<<endl;
	}
	return 0;
}