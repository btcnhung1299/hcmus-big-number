#include "QInt.h"
#include <fstream>
#include <string>

int main()
{
	// ifstream in("lshift.txt");
	// ofstream out("output-lshift.txt");
	QInt a;
	int k;

	while (cin >> a)
	{
		cin >> k;
		QInt c = a.rol(k);
		//cout << c << endl;
	}

	// in.close();
	// out.close();
	return 0;
}