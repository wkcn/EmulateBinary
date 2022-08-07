#ifndef _FLOAT_H_
#define _FLOAT_H_

#include <vector>
#include <stack>
#include <cmath>
using namespace std;
#include "Bits.h"
#include "Int.h" // 使用Int进行二进制加法运算

//32位单精度浮点数，1位符号位，8位指数，尾数23位，偏阶127
class Float : public Bits<32> {
public:
	Float();
	Float(float num);
	operator float();
	friend Float operator+(const Float &a, const Float &b);
	Float& operator+=(const Float &b);
	//FloatBase& operator=(float num);
	bool isZero() const;
private:
	int GetE() const;
	void SetE(int E);
	float GetF() const;
};

Float::Float() {

}

Float::Float(float num) {
	if (num == 0)return; // 0要特殊处理
	//long long is 64 bits;
	bool minus = false;
	if (num < 0) {
		minus = true;
		num = -num;
	}

	//设置符号位
	data[31] = minus; 
	long long z = num; // 获取整数部分
	float f = num - z; // 获取小数部分
	vector<bool> bs;
	stack<bool> st; // 栈，为了将高位放在前面
	const int FLen = 23 + 1; // 尾数23位长，加上隐藏位共24位
	while (z > 0 && st.size() < FLen) {
		st.push(z % 2 == 1);
		z /= 2;
	}
	//st 的顶部不可能是 0
	while (!st.empty()) {
		bs.push_back(st.top());
		st.pop();
	}
	int zlen = bs.size(); // 整数长度，zlen 可能取 0
	int firstOne = (zlen == 0 ? -1 : 0);
	// 当firstOne 为 -1 时，因为已经排除num == 0的情况，因此f必不为0，必定有firstOne
	while (firstOne == -1 || bs.size() - firstOne < FLen) {
		f *= 2;
		if (f >= 1) {
			--f;
			if (firstOne == -1) {
				firstOne = bs.size();
			}
			bs.push_back(1);
		}
		else {
			bs.push_back(0);
		}
	}

	//bs has FLen 24 bits
	// firstOne 为第一个bit 1，zlen为整数的二进制长度
	// 设置尾数
	for (int i = 22, j = firstOne + 1;i >= 0;--i, ++j) {
		data[i] = bs[j];
	}
	// 设置指数
	SetE(zlen - 1 - firstOne);

}

Float::operator float() {
	//低效判别法
	bool zero = true;
	for (int i = 0;i < 32;++i) {
		if (data[i]) {
			zero = false;
			break;
		}
	}
	if (zero)return 0;
	//指数域
	int E = GetE();
	//小数域
	float F = GetF();
	float D = F * pow(2.0f, E);

	if (data[31])
		D = -D;
	return D;
}

int Float::GetE() const{
	int E = -127; // 指数，已偏阶
	int b = 1;
	for (int i = 23;i <= 30;++i) {
		if (data[i]) {
			E += b;
		}
		b <<= 1;
	}
	return E;
}

void Float::SetE(int E) {
	E += 127;
	for (int i = 23;i <= 30;++i) {
		data[i] = (E % 2 == 1);
		E /= 2;
	}
}

float Float::GetF() const{
	float F = 1;
	float base = 1.0 / 2;

	for (int i = 22;i >= 0;--i) {
		if (data[i]) {
			F += base;
		}
		base /= 2;
	}

	return F;
}

bool Float::isZero() const{
	for (int i = 0;i < 32;++i) {
		if (data[i])return false;
	}
	return true;
}

Float operator+(const Float &a, const Float &b) {
	if (a.isZero())return b;
	if (b.isZero())return a;
	Int ia, ib;
	for (int i = 0;i < 23;++i) {
		ia[i] = a.get(i);
		ib[i] = b.get(i);
	}
	// 隐藏1
	ia[23] = 1;
	ib[23] = 1;
	//移位
	int ae = a.GetE(), be = b.GetE();
	bool biga = ae > be; // a的指数比b大
	int ne; // 新的指数
	if (biga) {
		ne = ae;
		ib = ib >> (ae - be);
	}
	else {
		ne = be;
		ia = ia >> (be - ae);
	}
	if (a.get(31)) {
		//ia 为负数
		ia = Int() - ia;
	}
	if (b.get(31)) {
		ib = Int() - ib;
	}
	Int ic = ia + ib;
	//共23位
	Float c; // 默认全部为0
	//符号位
	c[31] = ic[31];
	if (ic[31]) {
		//如果结果为负数
		ic = ic.GetSignedNumber();
		//得到SignedNumber
	}
	else {
		//结果为正数
	}
	int j;
	for (j = 24;j >= 0;--j) {
		if (ic[j])break;
	}
	if (j == -1)return Float();
	// j 为 第一个bit 1 的位置
	int E = ne + j - 23;
	c.SetE(E);
	int i;
	for (j = j - 1,i = 22;j >= 0 && i >= 0;--j,--i) {
		c[i] = ic[j];
	}
	return c;
}

Float& Float::operator+=(const Float &b) {
	(*this) = (*this) + b;
	return *this;
}

#endif
