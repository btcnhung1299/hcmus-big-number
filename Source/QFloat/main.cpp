#include "QFloat.h"

int main()
{
	QFloat a, b;
	string s = "2.25";
	a.scanQFloat(s);
	string s1 = "134.0625";
	b.scanQFloat(s1);

	return 0;
}


