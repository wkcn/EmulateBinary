#ifndef _BITS_H_
#define _BITS_H_

#include <iostream>
using namespace std;

//二进制串，Bits<长度> 
template <int N>
class Bits{
	public:
		Bits();
		Bits(const Bits<N> &b);
		~Bits();
		bool& operator[](int i);
		bool get(int i) const;
		Bits<N>& operator=(const Bits<N> &b);
		Bits<N> operator&(const Bits<N> &b) const;
		Bits<N> operator|(const Bits<N> &b) const;
		Bits<N> operator^(const Bits<N> &b) const;
		Bits<N> operator~() const;
		Bits<N> operator<<(int i) const;
		Bits<N> operator>>(int i) const;
		template <int N2>
		friend ostream& operator<<(ostream &os,const Bits<N2> &b);
	protected:
		bool data[N];
};


template <int N>
Bits<N>::Bits() {
	//data = new bool[N];
	//memset(data, 0, sizeof(data));
	for (int i = 0;i < N;++i)data[i] = 0;
}

template <int N>
Bits<N>::Bits(const Bits<N> &b) {
	//data = new bool[N];
	for (int i = 0;i < N;++i)data[i] = b.data[i];
}

template <int N>
Bits<N>::~Bits() {
	//delete[]data;
}

template <int N>
bool& Bits<N>::operator[](int i) {
	return data[i];
}

template <int N>
bool Bits<N>::get(int i) const {
	return data[i];
}

template <int N>
Bits<N>& Bits<N>::operator=(const Bits<N> &b) {
	if (this != &b) {
		for (int i = 0;i < N;++i)data[i] = b.data[i];
	}
	return *this;
}

template <int N>
Bits<N> Bits<N>::operator&(const Bits<N> &b) const{
	Bits<N> c;
	for (int i = 0;i < N;++i) {
		c[i] = data[i] & b[i];
	}
	return c;
}


template <int N>
Bits<N> Bits<N>::operator|(const Bits<N> &b) const{
	Bits<N> c;
	for (int i = 0;i < N;++i) {
		c[i] = data[i] | b[i];
	}
	return c;
}


template <int N>
Bits<N> Bits<N>::operator^(const Bits<N> &b) const{
	Bits<N> c;
	for (int i = 0;i < N;++i) {
		c[i] = data[i] ^ b[i];
	}
	return c;
}


template <int N>
Bits<N> Bits<N>::operator~() const{
	Bits<N> c;
	for (int i = 0;i < N;++i) {
		c[i] = !data[i];
	}
	return c;
}

template <int N>
Bits<N> Bits<N>::operator<<(int i) const{
	if (i >= N)return Bits<N>();
	Bits<N> c;
	for (int j = N-1;j >= i;--j){
		c.data[j] = data[j - i];
	}
	return c;
}

template <int N>
Bits<N> Bits<N>::operator>>(int i) const{
	Bits<N> c;
	if (i >= N){
		if (data[N-1]){
			for (int j = 0;j < N;++j)c.data[j] = 1;
		}
		return c;
	}
	for (int j = 0;j < N - i;++j){
		c.data[j] = data[j + i];
	}
	if (data[N-1]){
		for (int j = N-i;j < N;++j){
			c.data[j] = 1;
		}
	}
	return c;
}

template <int N>
ostream& operator<<(ostream &os,const Bits<N> &b) {
	static const char alpha[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	int u = (N - 1) / 4;
	//head N-1 to u * 4
	int t = 0;
	int offset = 0;
	for (int j = u * 4;j < N;++j) {
		t |= int(b.data[j]) << offset;
		++offset;
	}
	os << "0x" << alpha[t];
	//tail
	for (int k = u - 1;k >= 0;--k) {
		int y = k << 2;
		t = (int(b.data[y + 3]) << 3) | (int(b.data[y + 2]) << 2) | (int(b.data[y + 1]) << 1) | (int(b.data[y]));
		os << alpha[t];
	}
	return os;
}

typedef Bits<32> bit32;

#endif
