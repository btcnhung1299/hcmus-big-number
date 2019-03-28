#include "Qfloat.h"

QFloat::QFloat()
{
	s_exponent = 0;
	for (int i = 0; i < 7; i++)
		significand[i] = 0;
}
QFloat::QFloat(const QFloat& another)
{
	s_exponent = another.s_exponent;
	for (int i = 0; i < 7; i++)
		significand[i] = another.significand[i];
}

void QFloat::scanQFloat()
{
/*
- Nhập vào chuỗi ký tự string.
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
}

QFloat QFloat::binToDec(bool *bits)
{
/*
- Lưu phần dấu
- Lưu phần mũ
- Lưu phần trị 
*/

}