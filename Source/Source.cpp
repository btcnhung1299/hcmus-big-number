#include "QInt.h"
#include <fstream>
#include <string>

int main()
{
	ifstream in("division.txt");
	ofstream out("output-division.txt");
	QInt a, b;

	while (in >> a)
	{
		in >> b;
		QInt c = (a / b);
		out << c << endl;
	}

	in.close();
	out.close();
	return 0;
}