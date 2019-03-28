#include "QInt.h"
#include <fstream>
#include <string>

int main()
{
	ifstream in("../../Test/input/lshift.txt");
	ofstream out("../../Test/output/output-lshift.txt");
	QInt a;
	int k;

	if (!in)
	{
		cout << "Not found!" << endl;
		return 0;
	}

	while (in >> a)
	{
		in >> k;
		QInt c = (a << k);
		out << c << endl;
	}

	in.close();
	out.close();
	return 0;
}