#include "QInt.h"
#include <bits/stdc++.h>

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

void QInt::setData(int *another)
{
	for (int i = 0; i < 4; i++)
		data[i] = another[i];
}

int* QInt::getData()
{
	return data;
}

void QInt::clearData()
{
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

bool QInt::firstBit() const
{
	return (data[0] >> 31) & 1;
}

bool QInt::lastBit() const
{
	return data[3] & 1;
}

void QInt::setBit(int pos)
{
	int index = pos / 32;
	int k = pos % 32;
	data[index] = data[index] | (1 << (31 - k));
}

void QInt::clearBit(int pos)
{
	int index = pos / 32;
	int k = pos % 32;
	data[index] = data[index] & ~(1 << (31 - k));
}

void QInt::changeBit(int pos, bool value)
{
	value ? setBit(pos) : clearBit(pos);
}

void QInt::fillOnes()
{
// Hàm fill bit 1: gán dãy bit 11111...1111 (32 bit) = -1 vào từng ô.
	for (int i = 0; i < 4; i++)
		data[i] = -1;
}

void QInt::strDiv2(string& s) const
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

void QInt::strMul2(string& s, int times) const
{
/*
Thực hiện nhân một chuỗi với 2 times lần.
- Khởi tạo biến kết quả chứa tối đa 40 chữ số.
- Biến nhớ (carry) trong quá trình nhân.
- Chuẩn hóa chuỗi, bỏ đi những số 0 ở đầu.
*/
	if (times == 0) return;
	string res(40, '0');
	int carry = 0;
	int index = 39;

	for (int i = s.length() - 1; i >= 0; i--)
		res[index--] = s[i];

	for (int i = 0; i < times; i++)
		for (int j = 39; j >= 0; j--)
		{
			int local_product = carry + (res[j] - '0') * 2;
			res[j] = (local_product % 10) + '0';
			carry = local_product / 10;
		}

	while (res.front() == '0' && res.length() > 1)
		res.erase(0, 1);

	s = res;
}

bool* QInt::convertTo2sComplement(bool* unsigned_bits) const
{
/*
Biểu diễn số âm của dãy bit[128] dưới dạng bù 2 bằng cách lấy 0 trừ cho dãy bit unsigned.
*/
	bool *res = new bool[128];
	int carry = 0;

 	for (int i = 127; i >= 0; i--)
 	{
		int local_diff = abs(0 - unsigned_bits[i] - carry);
		res[i] = local_diff % 2;
		carry = (unsigned_bits[i] + carry) > 0;
	}

	return res;
}

void QInt::scanQInt(string s)
{
/*
Dữ liệu được nhập vào dưới dạng string
-> Kiểm tra (+) hay (-), lưu vào một biến sign
-> Lấy phần giá trị chuyển sang nhị phân
- Nếu là âm, chuyển sang dạng bù 2.
-> Đưa vào data.
*/	
	// Kiểm tra (+) hay (-)
	bool is_negative = false;
	if (s.front() == '-')
	{
		is_negative = true;
		s.erase(0, 1);
	}

	// Phần giá trị được chuyển sang nhị phân, lưu trong mảng bit[128]
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
	{
		int last_digit = s.back() - '0';
		bits[127 - i] = (last_digit % 2 == 1 ? 1 : 0);
		QInt::strDiv2(s);
	}

	// Nếu là số âm, chuyển về dạng bù 2
	if (is_negative) {
		bool *converted_bits = QInt::convertTo2sComplement(bits);
		delete[] bits;
		bits = converted_bits;
	}

	// Đưa dãy bit vào QInt
	binToDec(bits);
	delete[] bits;
}

string QInt::addStrings(const string& s1, const string& s2)
{
/*
Cộng 2 chuỗi được thực hiện theo thứ tự từ cuối lên đầu.
- Biến nhớ (carry) trong quá trình cộng.
* Mặc định hai chuỗi là đều dương.
*/

	string res(40, '0');
	int carry = 0;
	int index_1 = s1.length() - 1, index_2 = s2.length() - 1;

	for (int i = 0; i < 40; i++)
	{
		int local_sum = carry + (index_1 >= 0 ? s1[index_1--] : '0') - '0' + (index_2 >= 0 ? s2[index_2--] : '0') - '0';
		res[39 - i] = local_sum % 10 + '0';
		carry = local_sum / 10; 
	}

	while (res.front() == '0' && res.length() > 1)
		res.erase(0, 1);
	
	return res;
}

string QInt::printQInt() const
{
/*
In ra giá trị của số lớn.
- Công thức chuyển đổi:
dec = sigma(bits[217 - i] * 2^i) (i = 0 -> 127)
- Để tính nhanh lũy thừa 2, ta sẽ dựa vào chuỗi kết quả của lần tính lũy thừa 2 trước
và nhân thêm i - power lần với 2 (power là lũy thừa đã tính được ở lần trước).
* Với số âm, chuyển từ dạng bù 2 về unsigned, tính toán tương tự và thêm dấu trừ vào.
*/
	bool *bits = this->decToBin();
	bool is_negative = bits[0];
	string s = "0";

	if (is_negative) {
		bool *converted_bits = QInt::convertTo2sComplement(bits);
		delete[] bits;
		bits = converted_bits;
	}
	
	string power_of_two = "1";
	int power = 0;

	for (int i = 0; i < 128; i++)
	{
		if (bits[127 - i] == 1)
		{
			QInt::strMul2(power_of_two, i - power);
			power = i;
			s = addStrings(s, power_of_two);
		}
	}
	
	if (is_negative)
		s.insert(0, "-");

	delete[] bits;
	return s;
}

istream& operator>>(istream& is, QInt &qi)
{
	string s;
	is >> s;
	qi.clearData();
	qi.scanQInt(s);
	return is;
}

ostream& operator<<(ostream& os, const QInt& qi)
{
	os << qi.printQInt();
	return os;
}

bool* QInt::decToBin() const
{
/*
Chuyển từ hệ thập phân sang nhị phân (dưới dạng mảng bool[128])
- Bit thứ j tại ô thứ i chính là bit thứ 32 * i + j trong mảng.
- Lấy bit thứ j của một số x bằng cách 1 & (x >> (32 - j)).
*/
	bool *bins = new bool[128];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 32; j++)
			bins[32 * i + j] = 1 & (data[i] >> (31 - j));

	return bins;
}

QInt& QInt::binToDec(bool *bits)
{
/*
Chuyển từ hệ nhị phân sang thập phân (dưới dạng số lớn QInt)
Công thức chuyển đổi:
- Bit tại vị trí thứ i được lưu tại ô (i / 32). Ví dụ: Bit thứ 31 lưu ở ô 0, bit thứ 32 lưu ở ô 1.
- Trong ô đó, bit thứ i được set giá trị tại vị trí thứ (i % 32). Ví dụ: Bit thứ 0 lưu ở ô thứ 0 tại vị trí 0, Bit thứ 32 lưu ở ô thứ 1 tại vị trí 0.
*/
	int index = 0, k = 0;

	for (int i = 0; i < 128; i++)
		changeBit(i, bits[i]);
	
	return *this;
}

string QInt::binToHex(bool *bits)
{
/*
Chuyển từ hệ nhị phân sang thập lục phân (dưới dạng chuỗi)
Công thức chuyển đổi:
- Gom từng nhóm 4 bits lại chuyển thành 1 kí tự thập lục phân.
*/
	string hex_str = "";
	char hex_code;

	for (int i = 0; i < 128; i = i + 4)
	{
		int temp = bits[i] * 8 + bits[i + 1] * 4 + bits[i + 2] * 2 + bits[i + 3];
		hex_code = ((temp < 10) ? temp + '0' : temp + 'A'- 10);
		hex_str += hex_code;
	}

	return hex_str;
}

string QInt::decToHex()
{
/*
Chuyển từ hệ thập phân sang thập lục phân (dưới dạng chuỗi)
Công thức chuyển đổi:
- Chuyển từ thập phân sang nhị phân
- Chuyển từ nhị phân sang thập lục phân
*/
	bool *bits = this->decToBin();
	string hex_str = QInt::binToHex(bits);
	delete[] bits;
	return hex_str;
}

QInt& QInt::operator=(const QInt& another)
{
// Phép gán: Thực hiện gán từng giá trị vào mảng data.
	for (int i = 0; i < 4; i++)
		data[i] = another.data[i];

	return *this;
}

bool QInt::operator>(const QInt& rhs) const
{
/*
So sánh >:
- Kiểm tra dấu, nếu khác thì lập tức kết luận.
- Nếu cùng dấu, xét từng cặp bit (không tính bit dấu).
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool greater_than = false;

	if (bits_1[0] != bits_2[0])
		greater_than = (bits_1 == 0);
	else
		for (int i = 1; i < 128; i++)
			if (bits_1[i] != bits_2[i])
			{
				greater_than = (bits_1[i] == 1);
				break;
			}
	
	delete[] bits_1, bits_2;
	return greater_than;
}

bool QInt::operator<(const QInt& rhs) const
{
/*
So sánh <:
- Kiểm tra dấu, nếu khác thì lập tức kết luận.
- Nếu cùng dấu, xét từng cặp bit (không tính bit dấu).
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool less_than = false;

	if (bits_1[0] != bits_2[0])
		less_than = (bits_1[0] == 1);		// Hai số khác dấu, số nhỏ hơn sẽ mang dấu (-)
	else
		for (int i = 1; i < 128 ; i++)
			if (bits_1[i] != bits_2[i])
				{
					less_than = (bits_1[i] == 0);
					break;
				}

	delete[] bits_1, bits_2;
	return less_than;
}

bool QInt::operator>=(const QInt& rhs) const
{
// Phép >= là phủ của phép <
	return !(*this < rhs);
}

bool QInt::operator<=(const QInt& rhs) const
{
// Phép <= là phủ của phép >
	return !(*this > rhs);
}

bool QInt::operator==(const QInt& rhs) const
{
// Hai số QInt bằng nhau khi và chỉ khi tất cả các giá trị biểu diễn của chúng đều bằng nhau.
	for (int i = 0; i < 4; i++)
		if (data[i] != rhs.data[i])
			return false;

	return true;
}

bool QInt::operator!=(const QInt& rhs) const
{
	return !(*this == rhs);
}

bool* QInt::addBitArrays(const bool* bits_1, const bool* bits_2)
{
/*
Cộng hai dãy bit được thực hiện bằng cách cộng từng cặp bit và bit nhớ hiện tại theo thứ tự từ cuối về đầu.
- Bit thứ i trong dãy bit tổng bằng tổng từng cặp bit thứ i và bit nhớ chia dư cho 2.
- Nhớ 1 cho lần sao nếu tổng từng cặp bit và bit nhớ lớn hơn 2.
*/
	bool *bits_sum = new bool[128];
	int local_sum, carry = 0;
	
	for (int i = 127; i >= 0; i--)
	{
		local_sum = bits_1[i] + bits_2[i] + carry;
		bits_sum[i] = local_sum % 2;
		carry = local_sum / 2;
	}

	return bits_sum;
}

QInt QInt::operator+(const QInt& rhs) const
{
/*
Cộng hai số lớn:
- Chuyển hai số thành hai dãy bit 128.
- Cộng hai dãy bit.
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool *bits_sum = QInt::addBitArrays(bits_1, bits_2);
	QInt res;
	res.binToDec(bits_sum);

	delete[] bits_1, bits_2, bits_sum;
	return res;
}

QInt QInt::operator-(const QInt& rhs) const
{
/*
Phép trừ hai số lớn (a - b) = a + (-b).
- Chuyển hai số thành hai dãy bit 128.
- Chuyển số trừ về dạng bù hai.
- Thực hiện phép cộng trên hai dãy bit vừa tìm được. 
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool *converted_bits = QInt::convertTo2sComplement(bits_2);
	delete[] bits_2;
	bool *bits_diff = QInt::addBitArrays(bits_1, converted_bits);
	QInt res;
	res.binToDec(bits_diff);

	delete[] bits_1, converted_bits, bits_diff;
	return res;
}

QInt QInt::operator&(const QInt& rhs) const
{
/*
- Phép AND cho kết quả bằng 1 khi cả 2 bit cùng mang giá trị 1
- Có thể tính nhanh bằng cách nhân hai bit.
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool *bits_and = new bool[128];

	for (int i = 0; i < 128; i++)
		bits_and[i] = bits_1[i] * bits_2[i];

	QInt res;
	res.binToDec(bits_and);
	delete[] bits_1, bits_2, bits_and;
	return res;
}

QInt QInt::operator|(const QInt& rhs) const
{
// Phép OR cho giá trị 0 khi tổng 2 bit bằng 0, ngược lại bằng 1.

	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();
	bool *bits_or = new bool[128];

	for (int i = 0; i < 128; i++)
		bits_or[i] = !(bits_1[i] + bits_2[i] == 0);

	QInt res;
	res.binToDec(bits_or);
	delete[] bits_1, bits_2, bits_or;
	return res;
}

QInt QInt::operator^(const QInt& another) const
{
// Phép XOR cho kết quả bằng 1 khi 2 bit mang giá trị ngược nhau (tổng = 1)

	bool *bits_1 = this->decToBin();
	bool *bits_2 = another.decToBin();
	bool *bits_xor = new bool[128];

	for (int i = 0; i < 128; i++)
		bits_xor[i] = (bits_1[i] != bits_2[i]);

	QInt res;
	res.binToDec(bits_xor);
	delete[] bits_1, bits_2, bits_xor;
	return res;
}

QInt QInt::operator~() const
{
// Phép NOT: đảo bit
	bool *bits = this->decToBin();
	for (int i = 0; i < 128; i++)
		bits[i] = 1 - bits[i];

	QInt res;
	res.binToDec(bits);
	delete[] bits;
	return res;
}

QInt QInt::operator>>(int k) const
{
/* Phép dịch trái k bit
- Chuyển số thành dãy bit 128.
- Lưu lại bit dấu.
- Duyệt ngược từ cuối dãy bit, gán bit thứ i - k cho bit thứ i.
- Gán lại bit dấu, thêm padding trùng với bit dấu ở k bit đầu.
*/
	k = k % 128;
	bool *bits = this->decToBin();
	bool sign_bit = bits[0];

	for (int i = 127; i - k >= 0; i--) {
		bits[i] = bits[i - k];
	}

	for (int i = 0; i <= k; i++) {
		bits[i] = sign_bit;
	}

	QInt res;
	res.binToDec(bits);
	delete[] bits;
	return res;
}

QInt QInt::operator<<(int k) const
{
/* Phép dịch phải k bit
- Chuyển số thành dãy bit 128.
- Lưu lại bit dấu.
- Duyệt xuôi từ đầu dãy bit, gán bit thứ i + k cho bit thứ i.
- Thêm padding 0 ở k bit cuối.
* Bit dấu bị ảnh hưởng trong quá trình dịch.
*/
	k = k % 128;
	bool *bits = this->decToBin();
	bool sign_bit = bits[0];

	for (int i = 0; i + k < 128; i++) {
		bits[i] = bits[i + k];
	}

	for (int i = 0; i < k; i++) {
		bits[127 - i] = 0;
	}

	QInt res;
	res.binToDec(bits);
	delete[] bits;
	return res;
}

QInt QInt::rol(int k) const
{
// Ở phép xoay trái, bit trái cùng (MSB) sẽ được bỏ đi, và đưa về phía phải cùng (LSB).
	bool *bits = this->decToBin();
	int i = 0;

	while (k > 0)
	{
		bool tmp = bits[0];
		for (i = 0; i < 127; i++)
			bits[i] = bits[i + 1];

		bits[127] = tmp;
		k--;
	}
	
	for (i = 0; i < 128; i++)
		cout << bits[i];

	cout << endl;
	QInt res;
	res.binToDec(bits);
	return res;
}

QInt QInt::ror(int k) const
{
// Ở phép xoay phải, bit phải cùng (LSB) sẽ được bỏ đi, và đưa về phía trái cùng (MSB).

	bool *bits = this->decToBin();
	int i = 0;

	while (k > 0)
	{
		bool tmp = bits[127];
		for (i = 127; i > 0; i--)
			bits[i] = bits[i - 1];

		bits[0] = tmp;
		k--;
	}

	for (i = 0; i < 128; i++)
		cout << bits[i];

	cout << endl;
	QInt res;
	res.binToDec(bits);
	return res;
}

QInt QInt::operator*(const QInt& rhs) const
{
/*
Phép nhân hai số lớn: sử dụng thuật toán Booth
- Chia 3 TH:
+ 10: A = A - M
+ 01: A = A + M
+ 00: Không làm gì
Sau đó dịch phải 1 bit.
256 bit kết quả chỉ lấy 128 bit cuối.
*/
	QInt A;
	QInt Q = *this;
	QInt M = rhs;
	bool prev_Q = 0;

	for (int i = 0; i < 128; i++)
	{
		if (Q.lastBit() != prev_Q)
			A = (Q.lastBit() < prev_Q ? A + M : A - M);
		
		prev_Q = Q.lastBit();
		Q = Q >> 1;
		Q.changeBit(0, A.lastBit());
		A = A >> 1;
	}

	return Q;
}

QInt QInt::operator/(const QInt& rhs) const
{
/*
Phép chia hai số lớn: sử dụng thuật được đề xuất tại https://www.cise.ufl.edu/~mssz/CompOrg/CDA-arith.html
- Thương sẽ là số âm nếu số chia và số bị chia khác dấu.
*/
	QInt A, prev_A;
	QInt Q = *this;
	QInt M = rhs;
	QInt zero;
	
	if (Q.firstBit() == 1)
		A.fillOnes();

	bool is_negative = (Q.firstBit() != M.firstBit());

	for (int i = 0; i < 128; i++)
	{
		A = A << 1;
		A.changeBit(127, Q.firstBit());
		Q = Q << 1;
		bool sign_bit = A.firstBit();
		prev_A = A;
		A = (A.firstBit() != M.firstBit() ? A + M : A - M);
		
		if (sign_bit == A.firstBit())
			Q.changeBit(127, 1);
		else 
			if (A == zero && Q == zero)
				Q.changeBit(127, 1);
			else A = prev_A;
	}

	if (is_negative)
	{
		bool *bits = Q.decToBin();
		bool *converted_bits = QInt::convertTo2sComplement(bits);
		Q.binToDec(converted_bits);
		delete[] bits, converted_bits;
	}

	return Q;
}

QInt QInt::operator%(const QInt& rhs) const
{
/*
Phép chia lấy dư hai số lớn thực hiện theo ý tưởng của phép chia.
- A là phần dư (có thể âm). 
*/
	QInt A, prev_A;
	QInt Q = *this;
	QInt M = rhs;
	
	if (Q.firstBit() == 1)
		A.fillOnes();

	bool is_negative = (Q.firstBit() != M.firstBit());

	for (int i = 0; i < 128; i++)
	{
		bool sign_bit = A.firstBit();
		A = A << 1;
		A.changeBit(127, Q.firstBit());
		Q = Q << 1;
		prev_A = A;
		A = (A.firstBit() != M.firstBit() ? A + M : A - M);
		
		if (sign_bit == A.firstBit())
			Q.changeBit(127, 1);
		else A = prev_A;
	}

	return A;
}

void QInt::printBits() const
{
	bool *bits = this->decToBin();
	for (int i = 0; i < 128; i++)
		cout << bits[i];
	cout << endl;
	delete[] bits;
}