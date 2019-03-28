#ifndef QFLOAT_H_
#define QFLOAT_H_
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

/*
Còn 1 số lỗi để lát tối Tú về Tú fix lại nha
- Chưa scan được số có dạng 0.xxxx
- Chưa xử lý trường hợp dạng không chuẩn
- Cần xử lý một vài chỗ trong quá trình chuyển phần thập phân với phần nguyên.
*/

class QFloat
{
	private:
		short data[8];// Theo thứ tự: 1 bit dấu, 15 bits mũ, 112 bits trị
	public:
		void setBit(int pos);
		void clearBit(int pos);
		void changeBit(int pos, bool value);
		void strDiv2(string &) const;
		bool strMul2(string &) const;
			
		bool* convertInterger(string s);	// Cần tối ưu các bits 0
		bool* convertDecimal(string s);		// Cần mở rộng thêm cho mảng bits
		bool* convertBias(int);

	public:
		QFloat();
		QFloat(const QFloat&);

		void scanQFloat(string s);
		QFloat binToDec(bool*);


};


#include "Qfloat.hpp"
#endif QFLOAT_H_ 
