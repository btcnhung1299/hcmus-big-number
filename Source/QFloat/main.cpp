#include "Qfloat.h"

void main()
{
	QFloat a;
	string s;
	string s_copy;
	cin >> s;

	s_copy = s;
	a.scanQFloat(s);
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

}