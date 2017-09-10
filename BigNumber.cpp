#include <iostream>
#include <string>
#include <math.h>
class BigNumber
{
public:
	BigNumber(const std::string &str)
	{
		m_numbetString = str;
		m_isNegative = str[0] == '-' ? true : false;
		removeHighZeros();
	}
	BigNumber(){}
	BigNumber(long long number)
	{
		m_numbetString = std::to_string(number);
		m_isNegative = number < 0 ? true : false;
	}
	BigNumber &operator=(const BigNumber&rh)
	{
		m_numbetString = rh.m_numbetString;
		m_isNegative = rh.m_isNegative;
		removeHighZeros();
		return *this;
	}
	BigNumber(const BigNumber& rh)
	{
		*this = rh;
	}
	BigNumber Add(const BigNumber& number2)const
	{
		BigNumber bn1 = *this;
		BigNumber bn2 = number2;
		if (bn1.isNegative() || bn2.isNegative())
		{
			if (bn2.isNegative() && !bn1.isNegative())
				return bn1 - bn2.negate();
			else if (bn1.isNegative() && !bn2.isNegative())
				return bn2 - bn1.negate();
			else
				return (bn1.negate()+bn2.negate()).negate();
		}
		if (bn1 < bn2)
			return bn2 + bn1; 
		bn2.fillZeros(bn1.m_numbetString.size());
		std::string result;
		int carry = 0;
		for (int i = bn1.m_numbetString.size() - 1; i >= 0; --i)
		{
			int cur_add = bn1[i] + bn2[i] + carry;
			carry = cur_add / 10;
			result.insert(0, std::to_string(cur_add % 10));
		}
		return BigNumber(result);
	}

	BigNumber Subtract(const BigNumber& number2)const
	{
		BigNumber bn1 = *this;
		BigNumber bn2 = number2;
		if (bn1.isNegative() || bn2.isNegative())
		{
			if (bn2.isNegative() && !bn1.isNegative())
				return bn1 + bn2.negate();
			else if (bn1.isNegative() && !bn2.isNegative())
				return bn2 - bn1.negate();
			else
				return (bn2.negate() - bn1.negate());
		}
		std::cout <<"bn1 = " << bn1 << " = ";
		if (bn1 < bn2)
			return (bn2-bn1).negate();
		if (bn1 == bn2)
			return 0;
		bn2.fillZeros(bn1.m_numbetString.size());
		std::string result;
		int tmp_diff = 0;
		for (int i = bn1.m_numbetString.size() - 1; i >= 0; --i)
		{
			int cur_sub = bn1[i] - bn2[i] + tmp_diff;
			if (cur_sub < 0)
			{
				cur_sub += 10;
				tmp_diff = -1;
			}else
				tmp_diff = 0;
			result.insert(0, std::to_string(cur_sub));
		}
		return BigNumber(result);
	}
	BigNumber Multiple(const BigNumber& number2) const
	{
		BigNumber bn1 = *this;
		BigNumber bn2 = number2;
		if (bn1.isNegative() || bn2.isNegative())
		{
			if (bn2.isNegative() && !bn1.isNegative())
				return (bn1 * bn2.negate()).negate();
			else if (bn1.isNegative() && !bn2.isNegative())
				return (bn2 * bn1.negate()).negate();
			else
				return bn1.negate() * bn2.negate();
		}
		if (bn1 < bn2)
			return bn2 * bn1; 
		if (bn1 == 0 || bn2 == 0)
			return 0;
		if (bn2 == 1)
			return bn1;
		bn2.fillZeros(bn1.m_numbetString.size());
		std::size_t length = m_numbetString.size();
		if (length <= 9)  //notation 可能不同的平台会不一样？有些平台可能会溢出
		{
			unsigned long long oprands1 = bn1.getNumberLongLong();
			unsigned long long oprands2 = bn2.getNumberLongLong();
			unsigned long long res = oprands1 * oprands2;
			return BigNumber(res);
		}
		size_t len_left = length / 2;
		size_t len_right = length - len_left;
		BigNumber bn_a = BigNumber(bn1.m_numbetString.substr(0, len_left));
		BigNumber bn_b = BigNumber(bn1.m_numbetString.substr(len_left, len_right));
		BigNumber bn_c = BigNumber(bn2.m_numbetString.substr(0, len_left));
		BigNumber bn_d = BigNumber(bn2.m_numbetString.substr(len_left, len_right));
		BigNumber bn_ac = (bn_a * bn_c).powEleven(2 * len_right);
		BigNumber bn_ad = (bn_a * bn_d).powEleven(len_right);
		BigNumber bn_bc = (bn_b * bn_c).powEleven(len_right);
		BigNumber bn_bd = bn_b * bn_d;
		return bn_ac + bn_ad + bn_bc + bn_bd;
	}
	BigNumber Div(const BigNumber& number2)
	{
		BigNumber bn1 = *this;
		BigNumber bn2 = number2;
		if (bn2 == 0)
		{
			std::cerr << "dived by zero\n";
		}
		if (bn1.isNegative() || bn2.isNegative())
		{
			if (bn2.isNegative() && !bn1.isNegative())
				return (bn1 / bn2.negate()).negate();
			else if (bn1.isNegative() && !bn2.isNegative())
				return (bn2 / bn1.negate()).negate();
			else
				return bn1.negate() / bn2.negate();
		}
		if (bn1 < bn2)
			return 0;
		if (bn1 == bn2)
			return 1; 
		//bn2.fillZeros(bn1.m_numbetString.size());
		int res = 0;
		while(bn1 >= bn2)
		{
			bn1 = bn1 - bn2;
			res++;
		}
		return res;
	}
	BigNumber Mod(const BigNumber& number2)
	{
		BigNumber bn1 = *this;
		BigNumber bn2 = number2;
		if (bn1.isNegative() || bn2.isNegative())
		{
			if (bn2.isNegative() && !bn1.isNegative())
				return (bn1 % bn2.negate()).negate();
			else if (bn1.isNegative() && !bn2.isNegative())
				return (bn2 % bn1.negate()).negate();
			else
				return bn1.negate() % bn2.negate();
		}
		return bn1 - (bn1/bn2)*bn2;
	}
	BigNumber operator+(const BigNumber &rh)
	{
		return Add(rh);
	}
	BigNumber operator-(const BigNumber &rh)
	{
		return Subtract(rh);
	}
	BigNumber operator*(const BigNumber &rh)
	{
		return Multiple(rh); 
	}
	BigNumber operator/(const BigNumber &rh)
	{
		return Div(rh); 
	}
	BigNumber operator%(const BigNumber &rh)
	{
		return Mod(rh); 
	}
	bool operator<(const BigNumber& rh)
	{
		return !(*this == rh) && !(*this > rh);
	}
	bool operator<=(const BigNumber& rh)
	{
		return *this < rh || *this == rh;
	}
	bool operator>=(const BigNumber& rh)
	{
		return *this > rh || *this == rh;
	}
	bool operator==(const BigNumber &rh) const
	{
		return this->m_numbetString == rh.m_numbetString;
	}
	bool operator>(const BigNumber &b2) {
		BigNumber b1 = *this;
    	if (b1.isNegative() || b2.isNegative()) {
        	if (b1.isNegative() && b2.isNegative()) {
            	BigNumber bt = b2;
            	b1.m_numbetString.erase(0, 1);
            	bt.m_numbetString.erase(0, 1);
            	return b1 < bt;
        	}
        	else {
            	return !(b1.isNegative() && !b2.isNegative());
        	}
    	}
    	if (b1 == b2) {
        	return false;
    	}
    	if (b1.m_numbetString.size() > b2.m_numbetString.size()) {
        	return true;
    	}
    	else if (b2.m_numbetString.size() > b1.m_numbetString.size()) {
        	return false;
    	}
    	else {
        	for (unsigned int i = 0; i < b1.m_numbetString.size(); ++i) {
            	if (b1[i] == static_cast<unsigned int>(b2.m_numbetString[i] - '0')) {
                	continue;
            	}
            	return b1[i] > static_cast<unsigned int>(b2.m_numbetString[i] - '0');
        	}
    	}
    	return false;
    }
	bool isNegative() const
	{
		return m_isNegative;
	}
	BigNumber negate() {
    	if (this->m_numbetString[0] == '-') {
        	this->m_numbetString.erase(0, 1);
    	}
    	else {
       	 	this->m_numbetString.insert(this->m_numbetString.begin(), '-');
    	}
    	m_isNegative = m_isNegative == false ? true : false;
    	return *this;
	}
	unsigned int operator[](int index) {
    	if (this->m_numbetString[index] == '-') {
       	 	std::cerr << "Can't get the signal \n";
    	}
    	return static_cast<unsigned int>(this->m_numbetString[index] - '0');
	}
private:
	friend std::ostream &operator<<(std::ostream &os, const BigNumber &rh);
private:
	//将str的左边填入0，使总共的位数满足totalLength位 
	void fillZeros(std::size_t totalLength)
	{	
		std::size_t origLen = m_numbetString.size();
		if (totalLength <= origLen) return;
		std::size_t diff = totalLength - origLen;
		std::string tmp_str = "";
		for (std::size_t i = 0; i < diff; ++i)
		{	
			tmp_str += "0";
		}
		m_numbetString.insert(0,tmp_str);
	}
	void removeHighZeros()
	{
		//移除高位多余的0
		int index_not_zero;;
		if (m_isNegative)
		{
			//是负数
			index_not_zero = m_numbetString.find_first_not_of("0",1);
			if (index_not_zero != -1)
				m_numbetString = "-" + m_numbetString.substr(index_not_zero,m_numbetString.size() - index_not_zero);
			else
			{	//全是0
				m_numbetString = "0";
				m_isNegative = false;
			}
				
		}else{
			index_not_zero = m_numbetString.find_first_not_of("0",0);
			if (index_not_zero != -1)
				m_numbetString = m_numbetString.substr(index_not_zero,m_numbetString.size() - index_not_zero);
			else
				m_numbetString = "0";
		}
	}
	unsigned long long getNumberLongLong()
	{
		unsigned long long result = 0;
		size_t len = m_numbetString.size();
		for (int i = 0; i < len; ++i)
		{
			result = result + operator[](i) * pow(10.0, len - i - 1);
		}
		return result;
	}
	//*10^n
	BigNumber &powEleven(int n)
	{
		m_numbetString += std::string(n,'0');
		return *this;
	}	
	bool m_isNegative;
	std::string m_numbetString;
};
	std::ostream &operator<<(std::ostream &os, const BigNumber &rh)
	{
		size_t size = rh.m_numbetString.size();
		for (int i = 0; i < size; ++i)
		{
			os << rh.m_numbetString[i];
		}
		return os;
	}

int main()
{
	BigNumber b1("-999999999819283012749173821680227340273958920347298364826398162012471974837492836482365868162874917204728479832486234926");
	BigNumber b2("1");
	BigNumber b3 = b2 * b1;
	BigNumber b4 = b3 - b1;
	std::cout << b3 << std::endl;
	std::cout << b4 << std::endl;
	return 0;
}