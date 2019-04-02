#ifndef QFLOAT_H_
#define QFLOAT_H_
#include <bits/stdc++.h>
using namespace std;

class QFloat
{
	private:
		unsigned short data[8];			// Theo thứ tự: 1 bit dấu, 15 bit mũ, 112 bit trị

	public:
		QFloat();
		QFloat(const QFloat&);

	private:
		bool firstBit() const;
		void setBit(int pos);
		void clearBit(int pos);
		void changeBit(int pos, bool value);
		void strDiv2(string& s) const;
		void strMul2(string& s, int times) const;
		void strMul5(string& s, int times) const;
	 	bool fracMul2(string& frac) const;
		bool* convertTo2sComplement(bool* unsigned_bits) const;
		bool* convertToBias(int n) const;
		string addStrings(string s1, string s2);
		int exponent() const;
		

	public:
		void scanQFloat(string s);
		QFloat operator+(const QFloat &) const;
		//QFloat operator-(const QFloat &) const;

	public:
		QFloat& binToDec(bool *bits);
		bool* decToBin() const;
		void printQFloat();
		

};

#include "QFloat.hpp"
#endif
