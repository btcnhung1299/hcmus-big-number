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
public:
	void setBit(int pos);
	void clearBit(int pos);
	void changeBit(int pos, bool value);
	void strDiv2(string &) const;
	bool strMul2(string &) const;
	bool* tran_right_nbit(bool*, int);
	bool* tran_left_nbit(bool*, int);
		
	bool* convertInterger(string s, int& );	
	bool* convertDecimal(string s, int&);
	bool* convertBias(int);

public:
	QFloat();
	QFloat(const QFloat&);

	void scanQFloat(string s);
	QFloat binToDec(bool*);


};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
