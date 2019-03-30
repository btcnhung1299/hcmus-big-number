#include "Qfloat.h"

void main()
{
	QFloat a;
	string s;
	string s_copy;
	cin >> s;

	QFloat b;
	b.scanQFloat(s);

	s_copy = s;
	a.scan(s);

	/*
	bool* bits = new bool[128];
	int t = 0;
	bits = a.convertDecimal(s,t);
	
	for (int i = 0; i < 128; i++){
		cout << bits[i];
		if (i % 4 == 3) cout << " ";
	}
	cout << endl;
	//test

	bool* test = new bool[128];
	int i = 0;
	while (s != "0" && i<128)
	{
		test[i++] = a.strMul2(s_copy);
	}
	for (int i = 0; i < 128; i++){
		cout << test[i];
		if (i % 4 == 3) cout << " ";
	}

	cout << endl;

	*/


	// Theo quy ước chuẩn hóa, dấu chấm động được dời lên đằng sau số 1 đầu tiên.
	// Do đó, sẽ có (bits.length() - 1) bit ở phần nguyên này được đẩy xuống phần thập phân.
	// Mà ta chỉ có 112 bit để biểu diễn phần thập phân.
	// Nên nếu phần bit lấn qua quá lớn thì phần sau dấu chấm ban đầu sẽ không được lưu.


	// Trong trường hợp nói trên, cũng vì ta chỉ có 112 bit để biểu diễn phần trị.
	// Ta cũng chỉ lấy được 112 bit trong phần bit lấn qua.
	// Những bit không được lấy sẽ được tính dưới dạng mũ cơ số 2.
}


