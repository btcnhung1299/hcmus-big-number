#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s1 = "6.96875";
	a.scanQFloat(s1);
	string s2 = "-0.3418";
	b.scanQFloat(s2);
	QFloat c = a * b;

	return 0;
}