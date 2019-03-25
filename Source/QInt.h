#ifndef QINT_H_
#define QINT_H_
#define MAX_NUM_OF_DIGITS 40
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
		
		
		void printBit() const;
		void fillOnes();

	private:
		void strDiv2(string& s) const;
		bool* convertTo2sComplement(bool* bits) const;
		bool lastBit() const;
		bool firstBit() const;
		void setBit(int pos);
		void clearBit(int pos);
		void changeBit(int pos, bool value);
		bool* unsignedBin(bool* bit);
		string calculatePowerOf2(int n);
		void multiply(int res[], int& resSize, int x);
		string multiplyTwoStrings(string str1, string str2);
		string addTwoStrings(string str1, string str2);
		
	public:
	 	QInt& operator=(const QInt &);
	 	bool operator>(const QInt &) const;
	 	bool operator<(const QInt &) const;
	 	bool operator>=(const QInt &) const;
	 	bool operator<=(const QInt &) const;
	 	bool operator==(const QInt &) const;
		bool operator!=(const QInt &) const;

	public:
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
		QInt operatorrol(int k) const;
		QInt operatorror(int k) const;	

	public:
		void scanQInt();
		void printQInt();
		bool* decToBin() const;
		QInt& binToDec(bool *bits);
		string binToHex(bool *bits);
		string decToHex();
		static bool* addBitArrays(const bool*, const bool*);
};

#include "QInt.hpp"
#endif QINT_H_ 
