#ifndef _INT_H_
#define _INT_H_

#include "Bits.h"

//32位Int,内部二进制串使用补码
class Int : public Bits<32>{
public:
	Int();
	Int(int num);
	Int(const Int &b);
	template <int M>
	Int(const Bits<M> &d);
	operator int();
	Int& operator=(int num);
	//Int& operator=(const Int &b);
	friend Int operator+(const Int &a,const Int &b);
	friend Int operator-(const Int &a,const Int &b);
	friend Int operator*(const Int &a, const Int &b);
	Int& operator+=(const Int &b);
	Int& operator-=(const Int &b);
	Int& operator*=(const Int &b);
	Int GetSignedNumber() const;
	Int Get1sComplement() const;
	Int Get2sComplement() const;
private:
	Int Complement1() const;
	Int Complement2() const;
};

Int::Int() {

}

Int::Int(int num) {
	(*this) = num;
}

Int::Int(const Int &b) {
	(*this) = b;
}

template <int M>
Int::Int(const Bits<M> &d) {
	if (M >= 32) {
		for (int i = 0;i < 32;++i)data[i] = d.get(i);
	}
	else {
		for (int i = 0;i < M;++i)data[i] = d.get(i);
		bool u = d.get(M - 1);
		for (int i = M;i < 32;++i)data[i] = u;
	}
}

Int::operator int() {
	int u = 0;
	for (int i = 0;i < 31;++i) {
		u += data[i] << i;
	}
	u += data[31] * (-1) * (1 << 31);
	return u;
}

Int& Int::operator=(int num) {
	bool minus = num < 0;
	if (minus) num = -num;
	data[31] = 0;
	for (int i = 0;i < 31;++i) {
		data[i] = num % 2 == 1;
		num /= 2;
	}
	if (minus) {
		(*this) = Complement2();
	}
	return *this;
}

/*
Int& Int::operator=(const Int &b) {
	for (int i = 0;i < 32;++i) {
		data[i] = b.data[i];
	}
	return *this;
}
*/

Int operator+(const Int &a, const Int &b) {
	Int c;
	bool t = 0;
	for (int i = 0;i < 32;++i) {
		bool u = a.data[i] ^ b.data[i] ^ t;
		t = bool(bool(a.data[i] & b.data[i]) | bool((a.data[i] | b.data[i]) & t)); // 是否进位 
		c.data[i] = u;
	}
	return c;
}

Int operator-(const Int &a,const Int &b) {
	Int bComplement2 = b.Complement2();
	return a + bComplement2;
}

Int operator*(const Int &a, const Int &b) {
	Int c = a;
	Int result;
	for (int i = 0;i < 32;++i) {
		//从乘数低位开始判断
		if (b.data[i]) {
			result += c;
		}
		c = c << 1;
	}
	return result;
}

Int& Int::operator+=(const Int&b) {
	(*this) = (*this) + b;
	return *this;
}

Int& Int::operator-=(const Int&b) {
	(*this) = (*this) - b;
	return *this;
}

Int& Int::operator*=(const Int&b) {
	(*this) = (*this) * b;
	return *this;
}

Int Int::Complement1() const{
	return ~(*this);//Int(~(Bits<32>(*this)));
}

Int Int::Complement2() const{
	const Int one(1);
	return Complement1() + one;
}

Int Int::GetSignedNumber() const {
	if (!data[31])return *this;
	Int c = Complement2();
	c[31] = 1;
	return c;
}

Int Int::Get1sComplement() const {
	if (data[31]){
		return (*this) - 1;
	}
	else {
		return *this;
	}
}

Int Int::Get2sComplement() const {
	return *this;
}

#endif
