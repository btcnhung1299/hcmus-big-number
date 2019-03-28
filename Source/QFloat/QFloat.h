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
		
	bool* convertInterger(string s);	// Chưa xử lý số có giá trị cực lớn 
	bool* convertDecimal(string s);		// Chưa xử lý số có giá trị cực nhỏ
	bool* convertBias(int);

public:
	QFloat();
	QFloat(const QFloat&);

	void scanQFloat(string s);
	QFloat binToDec(bool*);


};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
