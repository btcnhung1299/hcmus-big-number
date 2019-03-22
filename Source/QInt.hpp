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

void QInt::setData(int *another)
{
	for (int i = 0; i < 4; i++)
		data[i] = another[i];
}

int* QInt::getData()
{
	return data;
}


void QInt::strDiv2(string& s)
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

bool* QInt::convertTo2sComplement(bool* unsigned_bits)
{
/*
Biểu diễn số âm của dãy bit[128] dưới dạng bù 2
- Chuyển sang dạng bù 1 bằng cách đảo tất cả các bit.
- Cộng kết quả trên với 1.
*/
	bool *res = new bool[128];
	for (int i = 0; i < 128; i++)
		res[i] = !unsigned_bits[i];

	int carry = 1;
	for (int i = 127; i >= 0; i--)
	{
		if (res[i] + carry == 1)
		{
			res[i] = 1;
			return res;
		}
		else res[i] = 0;
	}

	return res;
}

// bool* QInt::unsignedBin(bool* bit)
// {
// /*
// Chuyen tu dang bu 2  sang dang unsigned:
// - Tru day bit ban dau cho 1,
// - Dao bit de chuyen sang dang unsigned: 0->1, 1->0
// */
	
// 	bool *tmp = new bool[128];

// 	int re = 1;
// 	for (int i = 127; i >= 0; i--)
// 	{
// 		if ((bit[i]) < re)
// 		{
// 			bit[i] = 1;
// 			re = 1;
// 		}
// 		else
// 		{
// 			bit[i] = bit[i] - re;
// 			break;
// 		}
// 	}
	
// 	for (int i = 0; i < 128; i++)
// 		tmp[i] = !bit[i];
// 	return tmp;
// }

void QInt::scanQInt()
{
/*
Dữ liệu được nhập vào dưới dạng string
-> Kiểm tra (+) hay (-), lưu vào một biến sign
-> Lấy phần giá trị chuyển sang nhị phân
- Nếu là âm, chuyển sang dạng bù 2.
-> Đưa vào data.
*/
	string s;
	cin >> s;
	
	// Kiểm tra (+) hay (-)
	bool isNegative = false;
	if (s.front() == '-')
	{
		isNegative = true;
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
	if (isNegative) {
		bool *converted_bits = QInt::convertTo2sComplement(bits);
		delete[] bits;
		bits = converted_bits;
	}

	// Đưa dãy bit vào QInt
	binToDec(bits);
	delete[] bits;
}

bool* QInt::decToBin()
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

QInt QInt::binToDec(bool *bits)
{
/*
Chuyển từ hệ nhị phân sang thập phân (dưới dạng số lớn QInt)
Công thức chuyển đổi:
- Bit tại vị trí thứ i được lưu tại ô (i / 32). Ví dụ: Bit thứ 31 lưu ở ô 0, bit thứ 32 lưu ở ô 1.
- Trong ô đó, bit thứ i được set giá trị tại vị trí thứ (i % 32). Ví dụ: Bit thứ 0 lưu ở ô thứ 0 tại vị trí 0, Bit thứ 32 lưu ở ô thứ 1 tại vị trí 0.
*/
	int index = 0, k = 0;

	for (int i = 0; i < 128; i++) {
		if (!bits[i]) continue;
		index = i / 32;
		k = i % 32;
		data[index] = data[index] | (1 << (31 - k));
	}
	
	return *this;
}


QInt QInt::operator=(QInt& another)
{
// Phép gán: Thực hiện gán từng giá trị vào mảng data.
	for (int i = 0; i < 4; i++)
		data[i] = another.data[i];

	return *this;
}

bool QInt::operator>(QInt& rhs)
{
/*
So sánh >:
- Kiểm tra dấu, nếu khác thì lập tức kết luận.
- Nếu cùng dấu, xét từng cặp bit (không tính bit dấu).
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();

	if (bits_1[0] != bits_2[0])
		return bits_1[0] == 0;		// Hai số khác dấu, số lớn hơn sẽ mang dấu (+)

	for (int i = 1; i < 128 ; i++)
		if (bits_1[i] != bits_2[i])
			return bits_1[i] == 1;

	return false;
}

bool QInt::operator<(QInt& rhs)
{
/*
So sánh <:
- Kiểm tra dấu, nếu khác thì lập tức kết luận.
- Nếu cùng dấu, xét từng cặp bit (không tính bit dấu).
*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = rhs.decToBin();

	if (bits_1[0] != bits_2[0])
		return bits_1[0] == 1;		// Hai số khác dấu, số nhỏ hơn sẽ mang dấu (-)

	for (int i = 1; i < 128 ; i++)
		if (bits_1[i] != bits_2[i])
			return bits_1[i] == 0;

	return false;
}

bool QInt::operator>=(QInt& rhs)
{
// Phép >= là phủ của phép <
	return !(*this < rhs);
}

bool QInt::operator<=(QInt& rhs)
{
// Phép <= là phủ của phép >
	return !(*this > rhs);
}

bool QInt::operator==(QInt& rhs)
{
// Hai số QInt bằng nhau khi và chỉ khi tất cả các giá trị biểu diễn của chúng đều bằng nhau.
	for (int i = 0; i < 4; i++)
		if (data[i] != rhs.data[i]) return false;

	return true;
}