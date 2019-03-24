#include "QInt.h"
#include <fstream>
#include <string>

int main()
{
	QInt a, b;
	cout << "Enter a: ";
	a.scanQInt();
	cout << "Enter b: ";
	b.scanQInt();

	cout << "binary a: ";
	bool *bits_1 = a.decToBin();
	for (int i = 0; i < 128; i++) {
		cout << bits_1[i];
	}
	cout << endl;
	cout << "binary b: ";
	bool *bits_2 = b.decToBin();
	for (int i = 0; i < 128; i++) {
		cout << bits_2[i];
	}
	cout << endl;

	QInt c = a % b;
	bool *bits = c.decToBin();
	for (int i = 0; i < 128; i++) {
		cout << bits[i];
	}
	cout << endl;

	delete[] bits_1, bits_2, bits;


	/*//Test QInt -> binary
	QInt a;
	a.scanQInt();
	int *tmp = a.getData();
	for (int i = 0; i < 4; i++)
		cout << *(tmp + i) << endl;

	bool *bit = new bool[128];
	bit = a.decToBin();

	for (int i = 0; i < 128; i++)
		cout << bit[i];
	cout << endl;
	//Test binary -> QInt
	QInt b;
	b=b.binToDec(bit);

	int *t = b.getData();
	for (int i = 0; i < 4; i++)
		cout << *(t + i) << endl;
	*/
	return 0;
}