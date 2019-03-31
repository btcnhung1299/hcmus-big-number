#ifndef QFLOAT_H_
#define QFLOAT_H_

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
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
	
	void strMul2(string& s, int times) const;
	string strMul5(string s, int times) const;
	string addStrings(string s1, string s2);
	string mulStrings(string s1, string s2);

	
public:
	QFloat();
	QFloat(const QFloat&);

	void scanQFloat(string s);
	void scan(string s);
	QFloat binToDec(bool*);
	void printQFloat();
	bool* decToBin();

};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
