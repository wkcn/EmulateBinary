#ifndef _FLOAT_H_
#define _FLOAT_H_

#include <vector>
#include <stack>
#include <cmath>
using namespace std;
#include "Bits.h"
#include "Int.h" // ʹ��Int���ж����Ƽӷ�����

//32λ�����ȸ�������1λ����λ��8λָ����β��23λ��ƫ��127
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
	if (num == 0)return; // 0Ҫ���⴦��
	//long long is 64 bits;
	bool minus = false;
	if (num < 0) {
		minus = true;
		num = -num;
	}

	//���÷���λ
	data[31] = minus; 
	long long z = num; // ��ȡ��������
	float f = num - z; // ��ȡС������
	vector<bool> bs;
	stack<bool> st; // ջ��Ϊ�˽���λ����ǰ��
	const int FLen = 23 + 1; // β��23λ������������λ��24λ
	while (z > 0 && st.size() < FLen) {
		st.push(z % 2 == 1);
		z /= 2;
	}
	//st �Ķ����������� 0
	while (!st.empty()) {
		bs.push_back(st.top());
		st.pop();
	}
	int zlen = bs.size(); // �������ȣ�zlen ����ȡ 0
	int firstOne = (zlen == 0 ? -1 : 0);
	// ��firstOne Ϊ -1 ʱ����Ϊ�Ѿ��ų�num == 0����������f�ز�Ϊ0���ض���firstOne
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
	// firstOne Ϊ��һ��bit 1��zlenΪ�����Ķ����Ƴ���
	// ����β��
	for (int i = 22, j = firstOne + 1;i >= 0;--i, ++j) {
		data[i] = bs[j];
	}
	// ����ָ��
	SetE(zlen - 1 - firstOne);

}

Float::operator float() {
	//��Ч�б�
	bool zero = true;
	for (int i = 0;i < 32;++i) {
		if (data[i]) {
			zero = false;
			break;
		}
	}
	if (zero)return 0;
	//ָ����
	int E = GetE();
	//С����
	float F = GetF();
	float D = F * pow(2.0f, E);

	if (data[31])
		D = -D;
	return D;
}

int Float::GetE() const{
	int E = -127; // ָ������ƫ��
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
	// ����1
	ia[23] = 1;
	ib[23] = 1;
	//��λ
	int ae = a.GetE(), be = b.GetE();
	bool biga = ae > be; // a��ָ����b��
	int ne; // �µ�ָ��
	if (biga) {
		ne = ae;
		ib = ib >> (ae - be);
	}
	else {
		ne = be;
		ia = ia >> (be - ae);
	}
	if (a.get(31)) {
		//ia Ϊ����
		ia = Int() - ia;
	}
	if (b.get(31)) {
		ib = Int() - ib;
	}
	Int ic = ia + ib;
	//��23λ
	Float c; // Ĭ��ȫ��Ϊ0
	//����λ
	c[31] = ic[31];
	if (ic[31]) {
		//������Ϊ����
		ic = ic.GetSignedNumber();
		//�õ�SignedNumber
	}
	else {
		//���Ϊ����
	}
	int j;
	for (j = 24;j >= 0;--j) {
		if (ic[j])break;
	}
	if (j == -1)return Float();
	// j Ϊ ��һ��bit 1 ��λ��
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
