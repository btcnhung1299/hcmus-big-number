#include "QInt.h"
#include <fstream>
#include <string>

int main()
{
	ifstream in("division.txt");
	ofstream out("output-division.txt");
	QInt a, b;

	cin >> a;
	cin >> b;
	cout << "A: " << a << endl;
	cout << "B: " << b << endl;
	QInt c = (a / b);
	cout << "A / B: " << c << endl;
	
		

	in.close();
	out.close();
	return 0;
}