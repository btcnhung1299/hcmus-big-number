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
		void strDiv2(string& s);
		bool* convertTo2sComplement(bool* bits);
	// 	bool* unsignedBin(bool* bit);

	
	public:
	 	QInt& operator=(const QInt &);
	 	bool operator>(const QInt &);
	 	bool operator<(const QInt &);
	 	bool operator>=(const QInt &);
	 	bool operator<=(const QInt &);
	 	bool operator==(const QInt &);

	public:
		QInt operator+(const QInt &);
		QInt operator-(const QInt &);
		QInt& operator>>(int k);
		QInt& operator<<(int k);

	public:
		void scanQInt();
		bool* decToBin() const;
		QInt binToDec(bool *bits);
};

#include "QInt.hpp"
#endif	/* QINT_H_ */