#ifndef QINT_H_
#define QINT_H_
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class QInt
{
	private:
		int data[4];
	
	public:
		QInt();
		QInt(const QInt&);
		void setData(int *another);
		int* getData();
		void clearData();
		void printBit() const;
		

	private:
		bool lastBit() const;
		bool firstBit() const;
		void setBit(int pos);
		void clearBit(int pos);
		void changeBit(int pos, bool value);
		void fillOnes();

		void strDiv2(string& s) const;
		void strMul2(string& s, int times) const;
		bool* convertTo2sComplement(bool* unsigned_bits) const;
		
	public:
	 	QInt& operator=(const QInt &);
	 	bool operator>(const QInt &) const;
	 	bool operator<(const QInt &) const;
	 	bool operator>=(const QInt &) const;
	 	bool operator<=(const QInt &) const;
	 	bool operator==(const QInt &) const;
		bool operator!=(const QInt &) const;

	public:
		friend istream& operator>>(istream& is, QInt& qi);
		friend ostream& operator<<(ostream& os, const QInt& qi);
		void scanQInt(string s);
		string printQInt() const;
		QInt operator+(const QInt &) const;
		QInt operator-(const QInt &) const;
		QInt operator*(const QInt &) const;
		QInt operator/(const QInt &) const;
		QInt operator%(const QInt &) const;
		QInt operator&(const QInt &) const;
		QInt operator|(const QInt &) const;
		QInt operator^(const QInt &) const;
		QInt operator~() const;
		QInt operator>>(int k) const;
		QInt operator<<(int k) const;
		QInt rol(int k) const;
		QInt ror(int k) const;	

	public:
		bool* decToBin() const;
		QInt& binToDec(bool *bits);
		string binToHex(bool *bits);
		string decToHex();
		static bool* addBitArrays(const bool*, const bool*);
		static string addStrings(const string& s1, const string& str);
};

#include "QInt.hpp"
#endif 		// QINT_H_ 