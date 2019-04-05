#ifndef QFLOAT_H_
#define QFLOAT_H_
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;


class QFloat
{
	private:
		enum Type { ZERO, INF, N_AN, NORMAL, SUBNORMAL };
		unsigned short data[8];			// Theo thứ tự: 1 bit dấu, 15 bit mũ, 112 bit trị

	public:
		QFloat();
		QFloat(const QFloat&);

	private:
		int exponent() const;
		bool firstBit() const;
		void changeBit(int pos, bool value);
		enum QFloat::Type getType() const;
		static bool* setType(bool sign, QFloat::Type type);
		
		static void strDiv2(string& s);
		static void strMulN(string& s, int times, int n, int width = 1);
		static bool fracMul2(string& frac);
		
		static bool* convertTo2sComplement(bool* unsigned_bits, int length);
		static bool* convertToBias(int n);
		static bool* combineBits(bool sign, int exponent, bool* mantissa, int offset_mantissa, int length_mantissa = 112);
		
		static void shiftRight(bool* bits, int start_pos, int length, int k);
		static void shiftLeft(bool* bits, int start_pos, int length, int k);
		
		static bool* addBitArrays(bool *bits_1, bool *bits_2, int length);
		static bool* subtractBitArrays(bool *bits_1, bool *bits_2, int length);
		static string addStrings(const string& s1, const string& str, bool left_align = false);

		static bool checkUnderflow(bool *bits, int default_radixpt_pos, int length, int& exponent);
		static bool checkOverflow(bool *bits, int default_radixpt_pos, int start_pos, int length, int& exponent);

	public:
		QFloat operator+(const QFloat &) const;
		QFloat operator-(const QFloat &) const;
		QFloat operator*(const QFloat &) const;
		QFloat operator/(const QFloat& another) const;
		friend istream& operator>>(istream&, QFloat &);
		friend ostream& operator<<(ostream&, QFloat);

	public:
		QFloat& binToDec(bool *bits);
		bool* decToBin() const;
		void scanBits(string s);
		string printBits();
		void scanQFloat(string s);
		string printQFloat();
};

#include "QFloat.hpp"
#endif
