#ifndef QFLOAT_H_
#define QFLOAT_H_

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class QFloat
{
private:
	short data[8];// Theo thứ tự: 1 bit dấu, 15 bits mũ, 112 bits trị
private:
	void setBit(int pos);
	void clearBit(int pos);
	void changeBit(int pos, bool value);

public:
	QFloat();
	QFloat(const QFloat&);

	void scanQFloat(string s);
	QFloat binToDec(bool*);


};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
