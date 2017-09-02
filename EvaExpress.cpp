#include <iostream>
#include <stack>
#include <math.h>
#include <assert.h>
using namespace std;

stack<char> g_op_stack;
stack<int>  g_data_stack;
const int op_plus = 0;
const int op_minus = 1;
const int op_mul = 2;
const int op_div = 3;
const int op_pow = 4;
const int op_bracket_left = 5;
const int op_bracket_right = 6;
const int op_begincalc = 7;
const int op_err = 255;
                     //+,-,*,/,^,(,),#
int g_op_prio[8][8] = {1,1,0,0,0,0,1,1,//+
                       1,1,0,0,0,0,1,1,//-
                       1,1,1,1,0,0,1,1,//*
                       1,1,1,1,0,0,1,1,///
                       1,1,1,1,1,0,1,1,//^
                       0,0,0,0,0,0,2,3,//(
                       0,0,0,0,0,0,0,3,//)
                       0,0,0,0,0,0,3,1 //#
                       };

//具体的操作数计算 a 是 第二个操作数， b 是 第一个操作数
int op_handle(int a, int b, char op)
{
	int res = 0;
	switch (op)
	{
		case '+':
			res = a + b;
			break;
		case '-':
			res = b - a;
			break;
		case '*':
			res = a * b;
			break;
		case '/':
			res = b / a;   //注意操作数顺序
		case '^':
			res = pow(b * 1.0, a);
			break;
		default:
			throw -1;

	}
	return res;
}

//获取符号的映射号，用于查询g_op_prio表
int getOpType(char op)
{
	int res = op_err;
	switch (op)
	{
		case '+':
		    res = op_plus;
		    break;
		case '-':
		    res = op_minus;
		    break;
		case '*':
		    res = op_mul;
		    break;
        case '/':
            res = op_div;
            break;
        case '^':
            res = op_pow;
            break;
        case '(':
        	res = op_bracket_left;
        	break;
        case ')':
			res = op_bracket_right;
			break;
        case '#':
        case '\0':
            res = op_begincalc;
            break;
        default:
        	res = op_err;
            break;

	}
	return res;
}
//比较操作符的优先级，根据返回的结果执行不同的操作，0表示op_cur入栈，1表示执行op_pre的操作，2表示直接从栈中弹出op_pre，3表示异常
int cmp_op_prio(char op_cur, char op_pre)
{
	int res = 3;
    int op_cur_type = getOpType(op_cur);
    int op_pre_type = getOpType(op_pre);
    assert(op_cur_type<=7 && op_pre_type<=7);
    if (op_cur_type == op_err || op_pre_type == op_err)
    	throw op_err;
    else
        res = g_op_prio[op_pre_type][op_cur_type];
    return res;
}

//对表达式进行预处理
bool preHandle(const char*str)
{

}

//判断字符串的头部是否是一个正数
bool ifNum(const char* str)
{
    if (*str>='0'&&*str<='9')
    	return true;
    else 
    	return false;
}

void calc_op(char *& pStr)
{
    char op_cur = *pStr;
    char op_top = g_op_stack.top();
    int op_stack = cmp_op_prio(op_cur,op_top);
    int a,b,res;
    switch (op_stack)
    {
    	case 0:
 			g_op_stack.push(op_cur);
 			pStr++;
    	    break;
    	case 1:
    	    a = g_data_stack.top();
    	    g_data_stack.pop();
    	    b = g_data_stack.top();
    	    g_data_stack.pop();
    	    res = op_handle(a, b, op_top);
    	    cout << "res = "<<res <<" "<<a<<" "<<op_top<<" "<<b<<endl;
    	    g_data_stack.push(res);
    	    g_op_stack.pop();
    		break;
    	case 2:
    		g_op_stack.pop();
    		pStr++;
    		break;
    	default:
    		throw -2;
    		break;

    }

}

//获取字符串头的数字，并且跳过字符串头中的数字；
int getNum(char *& pStr)
{
	int res = 0;
	while(ifNum(pStr))
	{
		res *= 10;
		res += (*pStr++) - '0';
	}
	return res;
}
template  <class T>
void StackClear(stack<T> &pStack)
{
	while(!pStack.empty()) pStack.pop();
}
int calc(char* express)
{
	StackClear(g_op_stack);
	StackClear(g_data_stack);
	g_op_stack.push('#');
	if (express == NULL) throw -3;
    char *pStr = express;
    while (*pStr!='\0'||g_op_stack.size()>1)
    {
    	if(ifNum(pStr))
    	{
    		int num = getNum(pStr);
    		g_data_stack.push(num);
    	}else{
    		calc_op(pStr);
    	}
    }
    return g_data_stack.top();
}

int main(int argc, char **argv)
{
	char *x = "40*(2+1)^2^3-10" ;
    int res = calc(x);
    int res2 = getNum(x);
    cout<<res<<endl;


	return 0;
}