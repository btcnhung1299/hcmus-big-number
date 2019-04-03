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
		int exponent() const;
		bool firstBit() const;
		void setBit(int pos);
		void clearBit(int pos);
		void changeBit(int pos, bool value);

		void strDiv2(string& s) const;
		bool fracMul2(string& frac) const;
		void strMul2(string& s, int times) const;
		void strMul5(string& s, int times) const;
		
		bool* convertTo2sComplement(bool* unsigned_bits, int length) const;
		bool* convertToBias(int n) const;
		bool* combineBits(bool sign, int exponent, bool* mantissa, int offset_mantissa) const;
		
		void shiftRight(bool* bits, int start_pos, int length, int k) const;
		void shiftLeft(bool* bits, int start_pos, int length, int k) const;
		
		bool* addBitArrays(bool *bits_1, bool *bits_2, int length) const;
		bool *subtractBitArrays(bool *bits_1, bool *bits_2, int length) const;
		string addStrings(string s1, string s2);
		

	public:
		void scanQFloat(string s);
		QFloat operator+(const QFloat &) const;
		QFloat operator-(const QFloat &) const;
		QFloat operator*(const QFloat &) const;

	public:
		QFloat& binToDec(bool *bits);
		bool* decToBin() const;
		void printQFloat();
		

};

#include "QFloat.hpp"
#endif
