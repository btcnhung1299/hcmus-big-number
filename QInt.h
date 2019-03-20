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
	void setData(int data[]);
	int* getData();
public:
	void setBit1(int i);
	string strDiv2(string s);
	bool* complement2(bool* bit);
	bool* unsignedBin(bool* bit);
public:
	void scanQInt();//Cau a)
	bool* decToBin();//Cau c)
	QInt binToDec(bool *bit);//Cau d)

	QInt operator = (QInt &);
	bool operator > (QInt &);
	bool operator < (QInt &);
	bool operator >= (QInt &);
	bool operator <= (QInt &);
	bool operator == (QInt &);


};

#endif