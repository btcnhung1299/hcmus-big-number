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

void QFloat::strDiv2(string& s) const
{
	/*
	Thực hiện chia (nguyên) một chuỗi cho 2.
	- Biến nhớ (carry) trong quá trình chia.
	- Thực hiện chia từng ký tự trong chuỗi với 2, gán lại vào chuỗi.
	- Chuẩn hóa chuỗi: loại bỏ đi các số 0 ở đầu. Ví dụ chuỗi "012" chuẩn hóa thành "12".
	*/
	int carry = 0;

	for (int i = 0; i < s.length(); i++)
	{
		int remain = s[i] - '0' + carry * 10;
		carry = (remain % 2 == 1 ? 1 : 0);
		s[i] = (remain / 2) + '0';
	}

	if (s.front() == '0' && s.length() > 1)
		s.erase(0, 1);
}

bool QFloat::strMul2(string &s) const
{
	/*
	Thực hiện nhân một chuỗi cho 2. Chuỗi có dạng: "12345" với ý nghĩa 0.12345
	- Biến nhớ (carry) trong quá trình nhân
	- Thực hiện nhân từng ký tự trong chuỗi với 2, gán lại vào chuỗi. 
	- Dừng lại khi chuỗi = "0"
	- Chuẩn hóa chuỗi: loại bỏ các số 0 ở cuối. VD "12340" -> "1234"
	- Trả về kết quả 0 hoặc 1 sau 1 lần nhân.
	*/
	int carry = 0;
	
	for (int i = s.length()-1 ; i >=0; i--)
	{
		int remain = (s[i] - '0') * 2 + carry;
		carry = (remain / 10 == 1 ? 1 : 0); 
		s[i] = (remain % 10) + '0';
	}

	while (s.back() == '0' && s.length() > 1)
		s.erase(s.length()-1, 1);

	return carry;
}


bool* QFloat::convertInterger(string s)
{
/*
Chuyển phần nguyên của một số sang dạng nhị phân

*/
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
	{
		int last_digit = s.back() - '0';
		bits[127 - i] = (last_digit % 2 == 1 ? 1 : 0);
		QFloat::strDiv2(s);
	}

	return bits;
}

bool* QFloat::convertDecimal(string s)
{
/*
Chuyển phần thập phân của một số sang dạng nhị phân

*/
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	int i = 0;
	while (s != "0" && i<128)
	{
		bits[i++] = QFloat::strMul2(s);
	}

	return bits;
}

bool* QFloat::convertBias(int i)
{
/*
Chuyển một số nguyên về dạng bias.
*/
	bool * bits = new bool[15];
	int bias = 16383 + i;

	for (int j = 0; j < 15; j++)
	{
		bits[14 - j] = bias% 2;
		bias = bias / 2;		
	}
	
	return bits;
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
	// Chuyển phần nguyên và phần thập phân
	int index_dot = s.find('.');
	string s_interger = s.substr(0, index_dot);
	bool* interger = QFloat::convertInterger(s_interger);
	
	string s_decimal = s.substr(index_dot + 1, s.length() - index_dot - 1);
	bool* decimal = QFloat::convertDecimal(s_decimal);

	// 2. Chuyển về dãy bits[128]
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	if (is_negative)
		bits[0] = 1;

	int exp;
	int flag = 0 ;
	int index;//Vị trí bắt đầu lưu phần thập phân
	// Đưa phần nguyên vào dãy bit.
	for (int i = 0; i < 128; i++)
	{
		if (flag == 0 && interger[i] == 1)
		{
			flag = 1;
			index = i;
			continue;
		}
		if (flag == 1)
		{
			bits[15 + i - index] = interger[i];
		}
	}
	// Đưa phần mũ vào dãy bits
	exp = 127 - index;
	bool *bias = QFloat::convertBias(exp);
	for (int i = 0; i < 15; i++)
		bits[i + 1] = bias[i];

	// Đưa phần thập phân vào dãy bits
	for (int i = 0; i < 112 - exp; i++)
	{
		bits[16 + exp + i] = decimal[i];
	}
	
	for (int i = 0; i < 128; i++){
		cout << bits[i];
		if (i % 4 == 3) cout << " ";
	}
	cout << endl;

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