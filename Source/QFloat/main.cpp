#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s1 = "-99999999999999999999999.12398329938123891283912831283924";
	a.scanQFloat(s1);
	string s2 = "2596148429267413814265248164610049.0";
	b.scanQFloat(s2);
	bool *bit = a.decToBin();

	for (int i = 0; i < 128; i++) {
		if (i == 1 || i == 16) {
			cout << " ";
		}
		cout << bit[i];
	}
	cout << endl;
	a.printQFloat();
	b.printQFloat();

	return 0;
}
