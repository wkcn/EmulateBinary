#include <iostream>
#include <limits>
#include "Int.h"
#include "Float.h"

using namespace std;

void FloatMode();
void IntMode();
void TestIntMode();

//编译模式，选择模式：0,Int模式：1，Float模式：2 
#define MODE 0

int main(){
	
#if MODE == 0	
	cout << "请输入要进入的模式，1为Int模式，2为Float模式" << endl;
	int u;
	while (true){
		cin >> u;
		if (u == 1 || u == 2)break;
		cout << "请重新输入！" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
	}
	
	if (u == 1){
		IntMode();
	}else if (u == 2){
		FloatMode();
	}
#else

//重定向 
// freopen("in.txt","r",stdin);
// freopen("out.txt","w",stdout);

#if MODE == 1
	IntMode();
#else

#if MODE == 2
	FloatMode();
#endif

#endif

#endif

	
	return 0;
}

void IntMode(){
	while (!cin.eof()) {
		int a, b;
		cin >> a >> b;
		Int ia = a;
		Int ib = b;
		cout << ia.GetSignedNumber() << endl;
		cout << ia.Get1sComplement() << endl;
		cout << ia.Get2sComplement() << endl;
		cout << ib.GetSignedNumber() << endl;
		cout << ib.Get1sComplement() << endl;
		cout << ib.Get2sComplement() << endl;
		cout << int(ia + ib) << " " << int(ia*ib) << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
	}
}

void FloatMode(){
	while (!cin.eof()) {
		float a, b;
		cin >> a >> b;
		Float fa = a;
		Float fb = b;
		cout << fa << endl;
		cout << fb << endl;
		cout << fa + fb << endl;
		#if MODE == 0 
		cout << "Check: " << a << " + " << b << " = " << a + b << endl;
		cout << "Your Answer: " << float(fa) << " + " << float(fb) << " = " << float(fa + fb) << endl;
		#endif
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
	}
}

void TestIntMode(){
	int N = 128;
	for (int i = -N;i < N;++i) {
		for (int j = -N;j < N;++j) {
			Int a = i + j;
			Int b = i - j;
			Int c = i * j;
			if (int(a) != i + j)cout << "ha" << endl;
			if (int(b) != i - j)cout << "ha" << endl;
			if (int(c) != i * j)cout << "ha" << endl;
		}
	}
}
