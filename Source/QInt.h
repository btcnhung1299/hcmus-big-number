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

	private:
		void strDiv2(string& s) const;
		bool* convertTo2sComplement(bool* bits) const;
	// 	bool* unsignedBin(bool* bit);

	
	public:
	 	QInt& operator=(const QInt &);
	 	bool operator>(const QInt &) const;
	 	bool operator<(const QInt &) const;
	 	bool operator>=(const QInt &) const;
	 	bool operator<=(const QInt &) const;
	 	bool operator==(const QInt &) const;

	public:
		QInt operator+(const QInt &) const;
		QInt operator-(const QInt &) const;
		QInt operator>>(int k) const;
		QInt operator<<(int k) const;

	public:
		void scanQInt();
		bool* decToBin() const;
		QInt& binToDec(bool *bits);
};

#include "QInt.hpp"
#endif	/* QINT_H_ */