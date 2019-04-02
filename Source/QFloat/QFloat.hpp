#include "QFloat.h"

QFloat::QFloat()
{
	for (int i = 0; i < 8; i++)
		data[i] = 0;
}

QFloat::QFloat(const QFloat& another)
{
	for (int i = 0; i < 8; i++)
		data[i] = another.data[i];
}

bool QFloat::firstBit() const
{
	return (data[0] >> 15) & 1;
}

void QFloat::setBit(int pos)
{
	int index = pos / 16;
	int k = pos % 16;
	data[index] = data[index] | (1 << (15 - k));
}

void QFloat::clearBit(int pos)
{
	int index = pos / 16;
	int k = pos % 16;
	data[index] = data[index] & ~(1 << (15 - k));
}

void QFloat::changeBit(int pos, bool value)
{
	value ? setBit(pos) : clearBit(pos);
}

int QFloat::exponent() const
{
	return int(data[0] & ~(1 << 15)) - 16383;
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

	while (s.front() == '0' && s.length() > 1)
		s.erase(0, 1);
}

bool QFloat::fracMul2(string &s) const
{
/*
Thực hiện nhân một chuỗi thập phân cho 2. Chuỗi có dạng: "12345" với ý nghĩa 0.12345
- Biến nhớ (carry) trong quá trình nhân
- Thực hiện nhân từng ký tự trong chuỗi với 2, gán lại vào chuỗi. 
- Dừng lại khi chuỗi = "0".
- Chuẩn hóa chuỗi: loại bỏ các số 0 ở cuối. VD "12340" -> "1234"
- Trả về kết quả 0 hoặc 1 sau 1 lần nhân.
*/
	int carry = 0;
	
	for (int i = s.length() - 1; i >= 0; i--)
	{
		int local_product = carry + (s[i] - '0') * 2 ;
		s[i] = (local_product % 10) + '0';
		carry = local_product / 10; 
	}

	while (s.back() == '0' && s.length() > 1)
		s.pop_back();

	return carry;
}

bool* QFloat::convertToBias(int n) const
{
// Đổi số n sang dạng bias bằng cách cộng cho (2^14 - 1) và đổi sang dạng nhị phân.
	bool *bits = new bool[15];
	int decimal = 16383 + n;

	for (int i = 0; i < 15; i++)
		bits[i] = 1 & (decimal >> (14 - i));
	
	return bits;
}

void QFloat::scanQFloat(string s)
{
/*
- Dữ liệu được nhập vào dưới dạng string
-> Kiểm tra (+), (-), lưu vào biến dấu.
-> Dựa vào dấu chấm để tách phần nguyên và phần thập phân.
- Chuyển phần nguyên sang nhị phân + chuẩn hóa.
- Chuyển phần thập phân sang nhị phân + chuẩn hóa.
- Biểu diễn phần mũ dưới dạng bias.
-> Đưa vào data.
*/	

	bool *bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	// Kiểm tra (+) hay (-), lưu vào bit đầu tiên
	bool is_negative = false;
	if (s.front() == '-')
	{
		is_negative = true;
		s.erase(0, 1);
	}

	bits[0] = is_negative;

	// Tách phần nguyên và phần thập phân
	int radixpt_pos = s.find('.');
	string s_integer, s_fractional;
	
	if (radixpt_pos == -1)
	{
		s_integer = s;
		s_fractional = "0";
	}
	else
	{
		s_integer = s.substr(0, radixpt_pos);
		s_fractional = s.substr(radixpt_pos + 1);
	}

	// Xóa các chữ số 0 thừa ở phần nguyên và phần thập phân
	while (s_integer.front() == '0' && s_integer.length() > 1)
		s_integer.erase(0, 1);
	
	while (s_fractional.back() == '0' && s_integer.length() > 1)
		s_fractional.pop_back();

	// Nếu phần nguyên bằng 0, tức số có dạng 0., ta có thể cần biểu diễn dưới dạng không chuẩn
	bool might_denormalized = (s_integer == "0");

	// Chuyển phần nguyên sang nhị phân
	vector<bool> bits_integer;
	do
	{
		int last_digit = s_integer.back() - '0';
		bits_integer.push_back(last_digit % 2);
		QFloat::strDiv2(s_integer);
	} while (s_integer != "0");

	reverse(bits_integer.begin(), bits_integer.end());

	/* Chuẩn hóa phần nguyên:
	Theo quy ước chuẩn hóa, dấu chấm động được dời lên đằng sau số 1 đầu tiên.
	Do đó, sẽ có (bits.size() - 1) bit ở phần nguyên này được đẩy xuống phần thập phân.
	Mà ta chỉ có 112 bit để biểu diễn phần thập phân.
	Nên nếu phần bit lấn qua quá lớn thì phần sau dấu chấm ban đầu sẽ không được lưu.
	*/

	int mantissa_int = min(112, int(bits_integer.size()) - 1);		// Số bit của phần nguyên được biểu diễn ở phần trị
	int mantissa_frac = 112 - mantissa_int;							// Số bit của phần thập phân được biểu diễn ở phần trị
	int exponent = 0;

	for (int i = 0; i < mantissa_int; i++)
		bits[16 + i] = bits_integer[i + 1];

	// Nếu chỉ cần biểu diễn dưới dạng chuẩn
	if (!might_denormalized) {
		exponent = bits_integer.size() - 1;
		
		// Nếu bit cần để biểu diễn phần nguyên quá lớn thì không cần biểu diễn phần thập phân ban đầu.
		if (mantissa_frac != 0)							
			for (int i = 0; i < mantissa_frac; i++)								
				bits[16 + i + mantissa_int] = fracMul2(s_fractional);
	}
	else
	{
		// Trường hợp đặc biệt số 0 được biểu diễn [0/1][0000...][000000000....]
		if (s_fractional == "0")
			exponent = -16385;		
		else
		{
			bool found_one = false;
			vector<bool> bits_fractional;
			exponent--;

			while (bits_fractional.size() < 112)
			{
				if (exponent == -16385)
					found_one = true;
				bool bit = QFloat::fracMul2(s_fractional);
				if (bit == 0 && !found_one)
					exponent--;
				else if (found_one)
					bits_fractional.push_back(bit);
				else
					found_one = true;
			}

			for (int i = 0; i < 112; i++)
				bits[16 + i] = bits_fractional[i];
		}
	}
	
	// Đưa phần mũ vào dãy bits
	bool *bias = QFloat::convertToBias(exponent);
	for (int i = 0; i < 15; i++)
		bits[i + 1] = bias[i];

	binToDec(bits);
	delete[] bias, bits;
}

QFloat& QFloat::binToDec(bool *bits)
{
/*
Chuyển từ hệ nhị phân sang thập phân (dưới dạng số lớn QFloat)
Công thức chuyển đổi:
- Bit tại vị trí thứ i được lưu tại ô (i / 16). Ví dụ: Bit thứ 15 lưu ở ô 0, bit thứ 16 lưu ở ô 1.
- Trong ô đó, bit thứ i được set giá trị tại vị trí thứ (i % 16). Ví dụ: Bit thứ 0 lưu ở ô thứ 0 tại vị trí 0, Bit thứ 16 lưu ở ô thứ 1 tại vị trí 0.
*/
	for (int i = 0; i < 128; i++)
		changeBit(i, bits[i]);

	return *this;
}

bool* QFloat::decToBin() const
{
/*
Chuyển từ hệ thập phân sang nhị phân(dưới dạng mảng bool[128])
	- Bit thứ j tại ô thứ i chính là bit thứ 16 * i + j trong mảng.
	- Lấy bit thứ j của một số x bằng cách 1 & (x >> (15 - j)).
*/
	bool *bins = new bool[128];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 16; j++)
			bins[16 * i + j] = 1 & (data[i] >> (15 - j));

	return bins;
}

void QFloat::strMul5(string &s, int times) const
{
	/*
	Thực hiện nhân một chuỗi với 5 times lần.
	- Khởi tạo biến kết quả chứa tối đa 40 chữ số.
	- Biến nhớ (carry) trong quá trình nhân.
	- Chuẩn hóa chuỗi, bỏ đi những số 0 ở đầu.
	*/
	if (times == 0) return;
	string res(90, '0');
	int carry = 0;
	int index = 89;

	for (int i = s.length() - 1; i >= 0; i--)
		res[index--] = s[i];

	int index_dot = res.find('.');
	if (index_dot != -1)
		res.erase(index_dot, 1);

	for (int i = 0; i < times; i++)
		for (int j = 89; j >= 0; j--)
		{
			int local_product = carry + (res[j] - '0') * 5;
			res[j] = (local_product % 10) + '0';
			carry = local_product / 10;
		}

	//Dịch dấu phẩy sang trái times lần
	if (index_dot != -1)
		res.insert(index_dot - times, ".");

	while (res[0] == '0' && res[1] == '0' && res.length() > 1)
		res.erase(0, 1);

	s = res;
}


string QFloat::addStrings(string s1, string s2)
{
	/*
	- Vì tổng sigma (1/2)^n tiến về 1 khi n tiến về vô cùng => phần nguyên chỉ có 1 chữ số
	=> Chỉ cần thêm đủ số 0 vào phần thập phân để cộng từ cuối lên đầu (không ảnh hưởng kết quả)
	Cộng 2 chuỗi được thực hiện theo thứ tự từ cuối lên đầu.
	- Biến nhớ (carry) trong quá trình cộng.
	* Mặc định hai chuỗi là đều dương.
	*/

	string res(90, '0');
	int carry = 0;

	while (s2.length() > s1.length())
		s1.push_back('0');
	
	while (s1.length() > s2.length())
		s2.push_back('0');
	
	//int index_1 = s1.length() - 1, index_2 = s2.length() - 1;
	int index = s1.length() - 1;

	for (int i = 0; i < 90; i++)
	{
		if (s1[index] == '.')
		{
			res[i] = '.';
			continue;
		}

		int local_sum = carry + (index >= 0 ? s1[index--] : '0') - '0' + (index >= 0 ? s2[index--] : '0') - '0';
		res[89 - i] = local_sum % 10 + '0';
		carry = local_sum / 10;
	}

	while (res[0] == '0' && res[1] == '0' && res.length() > 1)
		res.erase(0, 1);

	return res;
}

void QFloat::strMul2(string& s, int times) const
{
	/*
	Thực hiện nhân một chuỗi với 2 times lần.
	- Khởi tạo biến kết quả chứa tối đa 5000 chữ số.
	- Biến nhớ (carry) trong quá trình nhân.
	- Chuẩn hóa chuỗi, bỏ đi những số 0 ở đầu.
	*/
	if (times == 0) return;

	string res(5000, '0');

	int carry = 0;
	int index = 4999;

	for (int i = s.length() - 1; i >= 0; i--)
		res[index--] = s[i];

	int index_dot = res.find('.');
	if (index_dot != -1)
		res.erase(index_dot, 1);

	for (int i = 0; i < times; i++)
		for (int j = 4999; j >= 0; j--)
		{
			int local_product = carry + (res[j] - '0') * 2;
			res[j] = (local_product % 10) + '0';
			carry = local_product / 10;
		}

	if (index_dot != -1)
		res.insert(index_dot, ".");

	while (res.front() == '0' && res[1] == '0' && res.length() > 1)
		res.erase(0, 1);

	s = res;
}

void QFloat::printQFloat()
{
	/*
	Xử lý in các số:
	- Số 0
	- Số NaN
	- Infinity
	- Chuẩn 
	- Không chuẩn
	*/
	bool *bits = decToBin();
	bool is_negative = bits[0];
	int stored_exp = 0;
	string s;

	for (int i = 1; i < 16; i++)
		stored_exp += bits[i] * pow(2, 15 - i);

	/*
	- Tính phần định trị bằng công thức: M = 1.xxxxxx... = 1 + 2^-1 + 2^-2 +...
	Lại có 2^-n = 1 / 2^n = 10^n / 2^n = 5^n và dịch dấu phẩy sang trái n đơn vị 
	==> Thay vì tính 2^n, ta tính 5^n và dời dấu phẩy sang trái n đơn vị rồi cộng chuỗi
	*/
	string power_of_five = "1";
	int power = 15;

	for (int i = 16; i < 128; i++)
	{
		if (bits[i] == 1)
		{
			strMul5(power_of_five, i - power);
			power = i;
			s = addStrings(s, power_of_five);
		}
	}

	if (stored_exp == 32767) //2^15 - 1
	{
		s = (s == "0") ? "INFINITY" : "NaN";
	}

	else if (stored_exp == 0)
	{
		//Trường hợp số không chuẩn = (-1)^ bit dấu * 2^(-16382) * 0.M
		if (s != "0") 
		{
			strMul5(s, 16382);
		}
	}
		
	else //Trường hợp số dạng chuẩn = (-1)^bit dấu * 2^(stored_exp - 16383) * 1.M 
	{
		s = addStrings(s, "1.0");
		strMul2(s, stored_exp - 16383);
	}

	if (is_negative)
		s.insert(0, "-");

	cout << s << endl;
}


bool* QFloat::convertTo2sComplement(bool* unsigned_bits) const
{
	bool *res = new bool[115];
	int carry = 0;
	
	for (int i = 114; i >= 0; i--)
 	{
		int local_diff = abs(0 - unsigned_bits[i] - carry);
		res[i] = local_diff % 2;
		carry = (unsigned_bits[i] + carry) > 0;
	}

	return res;
}

QFloat QFloat::operator+(const QFloat& another) const
{
/*
Phép cộng hai số thực lớn: tham khảo https://www.cs.colostate.edu/~cs270/.Fall18/resources/FloatingPointExample.pdf
- Chuyển số thực về cùng mũ (số thực có mũ nhỏ hơn được đẩy lên để giống với mũ của số còn lại).
- Thực hiện cộng trên phần trị.
- Nếu phép cộng trên có dư, phần mũ của kết quả được tăng lên 1, đồng thời phần trị cũng dịch sang phải 1 bit.
*/
	
	bool *bits[] = { this->decToBin(), another.decToBin() };
	int exponent[] = { this->exponent(), another.exponent() };
	int sign[] = { this->firstBit(), another.firstBit() };

	// Nếu hai số thực cùng dấu có thể dẫn đến trường hợp tràn, ta lưu vào biến has_same_sign.
	// Dấu của kết quả sẽ bằng dấu của cả hai số nếu thực hiện cộng trên hai số cùng dấu, ngược lại ta mặc định (+).
	bool has_same_sign = (sign[0] == sign[1]);

	// Tách phần trị và bit ẩn của cả hai số (bằng 1 nếu phần mũ lúc chuẩn hóa >= 0, ngược lại bằng 0).
	bool *mantissa[2];
	for (int i = 0; i < 2; i++)
		mantissa[i] = new bool[115];
	
	mantissa[0][0] = mantissa[1][0] = mantissa[0][1] = mantissa[1][1] = 0;
	mantissa[0][2] = (exponent[0] >= 0);
	mantissa[1][2] = (exponent[1] >= 0);

	for (int i = 0; i < 112; i++)
	{
		mantissa[0][3 + i] = bits[0][16 + i];
		mantissa[1][3 + i] = bits[1][16 + i];
	}

	// Đưa về cùng mũ bằng cách dịch bit của số thực có mũ nhỏ hơn lên.
	// Mặc định số mũ của kết quả bằng số mũ lớn hơn.
	int smaller = (exponent[0] > exponent[1]);
	int k = abs(exponent[0] - exponent[1]);
	int exponent_sum = exponent[1 - smaller];

	for (int i = 114; i - k >= 2; i--)
		mantissa[smaller][i] = mantissa[smaller][i - k];

	for (int i = 0; i < k; i++)
		mantissa[smaller][2 + i] = 0;

	// Nếu toán hạng nào là đang là âm, chuyển sang dạng bù 2
	for (int i = 0; i < 2; i++)
		if (sign[i])
		{
			bool *converted_mantissa = QFloat::convertTo2sComplement(mantissa[i]);
			delete[] mantissa[i];
			mantissa[i] = converted_mantissa;
		}

	// Thực hiện cộng phần trị
	bool *mantissa_sum = new bool[115];
	int local_sum, carry = 0;

	for (int i = 114; i >= 0; i--)
	{
		local_sum = mantissa[0][i] + mantissa[1][i] + carry;
		mantissa_sum[i] = local_sum % 2;
		carry = local_sum / 2;
	}

	int sign_sum = (has_same_sign ? sign[0] : mantissa_sum[0]);
	bool overflow = (has_same_sign && mantissa[0][1] + mantissa[1][1] != mantissa_sum[1]);

	// Nếu kết quả ra âm, chuyển từ dạng bù 2 sang dấu lượng
	if (mantissa_sum[0])
	{
		bool *converted_mantissa_sum = QFloat::convertTo2sComplement(mantissa_sum);
		delete[] mantissa_sum;
		mantissa_sum = converted_mantissa_sum;
	}

	bool underflow = (!overflow && mantissa_sum[2] != 1);

	// Chuẩn hóa nếu kết quả nếu bị overflow hay underflow
	if (overflow)
	{
		exponent_sum++;
		for (int i = 114; i >= 3; i--)
			mantissa_sum[i] = mantissa_sum[i - 1];
		mantissa_sum[2] = 1;
	}

	if (underflow)
	{
		int shift_left = 0;
		for (int i = 0; i < 112; i++)
			if (mantissa_sum[3 + i])
			{
				shift_left = 1 + i; 
				break;
			}

		exponent_sum -= shift_left;
		for (int i = 3; i < 114; i++)
			mantissa_sum[i] = (i + shift_left < 114 ? mantissa_sum[i + shift_left] : 0);
	}

	cout << "Mantissa s: " << endl;
	for (int i = 0; i < 115; i++)
		cout << mantissa_sum[i];
	cout << endl;
	
	// Đưa kết quả vào dãy bit[128]
	bool *bits_sum = new bool[128];
	bits_sum[0] = sign_sum;
	exponent_sum += 16383;
	
	for (int i = 0; i < 15; i++)
		bits_sum[1 + i] = 1 & (exponent_sum >> (14 - i));

	for (int i = 0; i < 112; i++)
		bits_sum[16 + i] = mantissa_sum[3 + i];

	cout << "--------------------------------" << endl;
	for (int i = 0; i < 128; i++)
	{
		if (i == 1 || i == 16) cout << " ";
		cout << bits_sum[i];
	}

	cout << endl;

	QFloat res;
	res.binToDec(bits_sum);
	
	for (int i = 0; i < 2; i++)
		delete[] bits[i], mantissa[i];
	delete[] bits_sum, mantissa_sum;

	return res;
}