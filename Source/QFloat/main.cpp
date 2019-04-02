#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s1 = "7.0";
	a.scanQFloat(s1);
	string s2 = "-4.0";
	b.scanQFloat(s2);
	QFloat c = a + b;

	return 0;
}