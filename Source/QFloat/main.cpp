#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s1 = "-123.75";
	a.scanQFloat(s1);
	string s2 = "-999999.125";
	b.scanQFloat(s2);
	QFloat c = a + b;

	return 0;
}