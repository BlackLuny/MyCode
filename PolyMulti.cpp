#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class SigExp
{
public:
	SigExp(int coff,int exp):coff(coff),exp(exp){};
    SigExp(){};
    int coff;
    int exp;

    //两个单项式相加
    SigExp operator+(const SigExp &rh) const
    {
    	if (this->exp == rh.exp) 
    	{
    		SigExp tmp;
    		tmp.coff = this->coff + rh.coff;
    		tmp.exp = this->exp;
    		return tmp;
    	}
    	throw "Error 0";
    }

    //两个单项式相乘
    SigExp operator*(const SigExp &rh) const
    {
    	SigExp tmp;
    	tmp.coff = this->coff * rh.coff;
    	tmp.exp = this->exp + rh.exp;
    	return tmp;
    }

    //打印输出单项式
    friend ostream &operator<<(ostream &os, const SigExp &SigExp);

};

ostream &operator<<(ostream &os, const SigExp &SigExp)
{
    os<<SigExp.coff<<"X^"<<SigExp.exp;
    return os;
}

bool cmp_exp(const SigExp& lh, const SigExp& rh)
{
	return lh.exp > rh.exp;
}

class PolyExp
{
public:
	PolyExp(char *expr)
	{
		while(*expr != '\0')
		{
			int num_coff = atoi(expr);
			expr++;
			while(*expr>='0' && *expr<='9') expr++;
			expr++; expr++;  //跳过"X^"
			int num_exp = atoi(expr);
			vec_sg.push_back(SigExp(num_coff,num_exp));
			while(*expr>='0' && *expr<='9') expr++;
		}
	}
	PolyExp(){}
	vector<SigExp> vec_sg;  //一个多项式由多个单项式组成

	//合并同类项 两个多项式相加
	PolyExp operator+(const PolyExp& rh) const
	{
		PolyExp tmp;
		tmp.vec_sg = this->vec_sg;
		tmp.vec_sg.insert(tmp.vec_sg.begin(),rh.vec_sg.begin(),rh.vec_sg.end());  //将两个多项式的所有项都提取出来
		sort(tmp.vec_sg.begin(),tmp.vec_sg.end(),cmp_exp);  //按照指数从高到低排序，这样指数相同的就挨到一起了
		for (int i = 0; i < tmp.vec_sg.size() - 1; ++i)
		{
			SigExp* pPoly1 = &tmp.vec_sg[i];
			SigExp* pPoly2 = &tmp.vec_sg[i + 1];
			if (pPoly1->exp == pPoly2->exp)  //判断指数是否相同,不相同就啥事儿不干
			{
				*pPoly2 = *pPoly1 + *pPoly2;
				pPoly1->coff = 0;        //将当前项的系数置0，也就是丢弃这一项，因为已经被合并了
			}	
		}
		PolyExp res;
		for (int i = 0; i < tmp.vec_sg.size(); ++i)
		{
			SigExp* pPoly1 = &tmp.vec_sg[i];
			if (pPoly1->coff != 0) res.vec_sg.push_back(*pPoly1);//只保留系数非0的项
		}
		return res;
	}

	//多项式+单项式
	friend PolyExp operator+(const PolyExp& poly, const SigExp &rh);
	//为了支持 单项式+多项式
	friend PolyExp operator+(const SigExp &rh, const PolyExp& poly);

	//多项式 * 多项式
	PolyExp operator*(const PolyExp& rh) const
	{
		PolyExp tmp;
		for (int i = 0; i < this->vec_sg.size(); ++i)
		{
			const SigExp * pSg1 = &(this->vec_sg[i]);
			for (int j = 0; j < rh.vec_sg.size(); ++j)
			{
				tmp = tmp + (*pSg1 * rh.vec_sg[j]);  //多项式 = 多项式 + 单项式 * 单项式
			}
		}
		return tmp + (char*)"0X^0";  //强行进行加运算，以此来合并同类项
	}

	//输出打印多项式
  friend ostream &operator<<(ostream &os, const PolyExp &poly);
};

PolyExp operator+(const PolyExp& poly, const SigExp &rh)
{
	PolyExp tmp;
	tmp.vec_sg = poly.vec_sg;
	if (rh.coff != 0)  tmp.vec_sg.push_back(rh);
	return tmp;
}
PolyExp operator+(const SigExp &lh, const PolyExp& poly)
{
	return poly + lh;
}
ostream &operator<<(ostream &os, const PolyExp &poly)
{
	size_t cnt = poly.vec_sg.size();
	if (cnt == 0) return os;
	os<<poly.vec_sg[0];
	for (int i = 1; i < cnt; ++i)  
	{
		if (poly.vec_sg[i].coff > 0) os << "+";
		os << poly.vec_sg[i];
	}
	return os;
}

//多项式乘法  str1 * str2,结果保存到outStr,按照降幂排序
void PolyMulti(char *str1, char *str2, char *outStr)
{
	PolyExp poly1(str1), poly2(str2);
    ostringstream os;
    os << poly1 * poly2;
    cout<<os.str()<<endl;
    strcpy(outStr, os.str().c_str());
}

int main(int argc, char ** argv)
{
	char str1[2560] = "7X^4-10X^2-8X^4+1X^100-60X^40+7X^4-10X^2-8X^4+1X^100-60X^40+7X^4-10X^2-8X^4+1X^100-60X^40-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40";
	char str2[2560] = "-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40+7X^4-10X^2-8X^4+1X^100-60X^40-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-7X^4+5X^6-9X^0+8X^4-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40-1024X^108+800X^90+7X^4-10X^2-8X^4+1X^100-60X^40";
	char * tmp_result = (char*)malloc(4000);
	memset(tmp_result,0,4000);
	if(tmp_result==NULL) return -1;
	PolyMulti(str1, str2, tmp_result);
	if(strcmp(tmp_result,"") == 0)
		cout << "OK" << endl;
	free(tmp_result);
}