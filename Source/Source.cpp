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