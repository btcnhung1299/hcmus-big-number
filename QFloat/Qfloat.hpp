#include "Qfloat.h"

QFloat::QFloat()
{
	for (int i = 0; i < 8; i++)
		data[i] = 0;
}
QFloat::QFloat(const QFloat& another)
{
	for (int i = 0; i < 7; i++)
		data[i] = another.data[i];
}



void QFloat::setBit(int pos)
{
	int index = pos / 32;
	int k = pos % 32;
	data[index] = data[index] | (1 << (31 - k));
}

void QFloat::clearBit(int pos)
{
	int index = pos / 32;
	int k = pos % 32;
	data[index] = data[index] & ~(1 << (31 - k));
}

void QFloat::changeBit(int pos, bool value)
{
	value ? setBit(pos) : clearBit(pos);
}

void QFloat::scanQFloat(string s)
{
/*
- Đưa string về dạng chuẩn.
	* Difficult
	+ Xác định dấu 
	+ Chuyển phần nguyên
	+ Chuyển phần thập phân
- Chuyển về dãy bits[128]
	+ Chuyển phần mũ sang dạng bias
	+ Đưa phần trị vào dãy bits
- binToDec(bits)
*/
	// 1. Đưa về dạng chuẩn.
	// Kiểm tra (+) hay (-)
	bool is_negative = false;
	if (s.front() == '-')
	{
		is_negative = true;
		s.erase(0, 1);
	}

	// 2. Chuyển về dãy bits[128]
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	if (is_negative)
		bits[0] = 1;

	//
	//

	// 3. binToDec
	binToDec(bits);
	delete[] bits;
}

QFloat QFloat::binToDec(bool *bits)
{
/*
Chuyển từ hệ nhị phân sang thập phân (dưới dạng số lớn QFloat)
Công thức chuyển đổi:
- Bit tại vị trí thứ i được lưu tại ô (i / 32). Ví dụ: Bit thứ 31 lưu ở ô 0, bit thứ 32 lưu ở ô 1.
- Trong ô đó, bit thứ i được set giá trị tại vị trí thứ (i % 32). Ví dụ: Bit thứ 0 lưu ở ô thứ 0 tại vị trí 0, Bit thứ 32 lưu ở ô thứ 1 tại vị trí 0.
*/
	for (int i = 0; i < 128; i++)
		changeBit(i, bits[i]);

	return *this;
}