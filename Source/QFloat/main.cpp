#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s1 = "-82738172832183.09102930213";
	a.scanQFloat(s1);
	string s2 = "82738172832183.09102930213";
	b.scanQFloat(s2);
	QFloat c = a + b;

	return 0;
}