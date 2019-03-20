#include "QInt.h"

QInt::QInt()
{
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

QInt::QInt(const QInt& qi)
{
	for (int i = 0; i < 4; i++)
		data[i] = qi.data[i];
}

void QInt::setData(int d[4])
{
	for (int i = 0; i < 4; i++)
		data[i] = d[i];
}

int* QInt::getData()
{
	return data;
}


void QInt::setBit1(int i)
{
/* 
Bat bit 1 tai vi tri i cua bien x:
- Xac dinh vi tri do thuoc phan tu thu may cua mang data[],[0,31]-> 1; [32,63]->2; [64,95]->3; [96,128]->4.
- Sau do bat bit 1 bang cong thuc x=(1<<(127-i))|x
*/
	int index;
	if (i < 32)
		index = 0;
	else if (i < 64)
		index = 1;
	else if (i < 96)
		index = 2;
	else if (i < 128)
		index = 3;
	data[index] = (1 << (127 - i)) | data[index];
}


string QInt::strDiv2(string s)
{
/*
Thuc hien chia mot chuoi cho 2:
- Tao bien nho 're'
- Xet tung ky tu trong chuoi: Dua ve dang so -> (+) bien nho 're' -> chia cho 2 -> dua ve dang chuoi.
- Xoa ki tu '0' truoc chuoi: '0123' -> '123' 
*/
	int re = 0;

	for (int i = 0; i < s.length(); i++)
	{
		char t = s[i] - 48 + re * 10;
		if (t % 2 == 1) re = 1;
		else re = 0;
		s[i] = t / 2 + 48;
	}
	
	if (s[0] == '0' && s.length() > 1)
		s.erase(0, 1);
	return s;
}

bool* QInt::complement2(bool* bit)
{
/*
Chuyen tu dang unsigned sang dang bu 2:
- Chuyen sang bu 1: 0->1, 1->0
- Cong them 1
*/

	bool * tmp = new bool[128];
	for (int i = 0; i < 128; i++)
		tmp[i] = !bit[i];

	int re = 1;
	for (int i = 127; i >= 0; i--)
	{

		if ((tmp[i] + re) == 1)
		{
			tmp[i] = 1;
			return tmp;
		}
		else
		{
			re = 1;
			tmp[i] = 0;
		}
	}
	return tmp;
}

bool* QInt::unsignedBin(bool* bit)
{
/*
Chuyen tu dang bu 2  sang dang unsigned:
- Tru day bit ban dau cho 1,
- Dao bit de chuyen sang dang unsigned: 0->1, 1->0
*/
	
	bool *tmp = new bool[128];

	int re = 1;
	for (int i = 127; i >= 0; i--)
	{
		if ((bit[i]) < re)
		{
			bit[i] = 1;
			re = 1;
		}
		else
		{
			bit[i] = bit[i] - re;
			break;
		}
	}
	
	for (int i = 0; i < 128; i++)
		tmp[i] = !bit[i];
	return tmp;
}

void QInt::scanQInt()
{
/*
Nhap du lieu:
- Nhap so nguyen lon vao duoi dang string
- Kiem tra so do (-) hay (+)
- Chuyen tu chuoi sang dang nhi phan
- Chuyen ve dang bu 2 neu la so am(-)
- Dua day bit vao dang QInt
*/

	string s;
	cin >> s;
	//Kiem tra dau (-) hay (+)
	bool sign = 0;
	if (s[0] == '-')
	{
		sign = 1;
		s.erase(0, 1);
	}
	//Chuyen tu chuoi sang dang nhi phan
	bool* bit = new bool[128];
	for (int i = 0; i < 128; i++){
		if ((s[s.length() - 1] - 48) % 2 == 1){
			bit[127 - i] = 1;
		}
		else
			bit[127 - i] = 0;
		s = QInt::strDiv2(s);
	}
	//Chuyen ve dang bu 2 neu la so am(-)
	if (sign == 1)
		bit = QInt::complement2(bit);
	//Dua day bit vao dang QInt
	binToDec(bit);
}

bool* QInt::decToBin()
{
/*
Chuyen tu he 10 sang he 2 duoi dang bool[128]:
- Ap dung cong thuc lay bit thu i cua bien x: bit = (x>>(128-1-j)) & 1
*/
	bool *bin;
	bin = new bool[128];
	for (int i = 0; i < 4; i++)
	for (int j = 0; j < 32; j++)
	{
		bin[32 * i + j] = (data[i] >> (127 - j)) & 1;
	}
	return bin;
}

QInt QInt::binToDec(bool *bit)
{
/*
Chuyen tu he 2 sang he 10 duoi dang QInt:
- Truy set day bit, neu bit tai vi tri thu i=1 thi dung ham setBit1(i) de bat bit 1 tai vi tri i len.
*/
	for (int i = 127; i >= 0; i--)
	{
		if (bit[i] == 1)
			setBit1(i);
	}
	return *this;
}


QInt QInt::operator = (QInt & other)
{
	for (int i = 0; i<4; i++)
		data[i] = other.data[i];
	return *this;
}

bool QInt::operator >(QInt &other)
{
	return true;
}
bool QInt::operator < (QInt &other)
{
	return true;
}
bool QInt::operator >= (QInt &other)
{
	return false;
}
bool QInt::operator <= (QInt &other)
{
	return false;
}
bool QInt::operator == (QInt &other)
{
	for (int i = 0; i < 4;i++)
	if (data[i] != other.data[i])
		return false;
	return true;
}