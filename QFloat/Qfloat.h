#ifndef QFLOAT_H_
#define QFLOAT_H_

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class QFloat
{
private:
	short s_exponent;
	short significand[7];

private:


public:
	QFloat();
	QFloat(const QFloat&);

	void scanQFloat();
	QFloat binToDec(bool*);


};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
