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

void QFloat::strDiv2(string& s)
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

void QFloat::strMulN(string& s, int times, int n, int width)
{
/*
Thực hiện nhân một chuỗi với N times lần.
- Khởi tạo biến kết quả chứa tối đa 115 chữ số.
- Biến nhớ (carry) trong quá trình nhân.
- Chuẩn hóa chuỗi, bỏ đi những số 0 ở đầu.
*/
	if (times == 0) return;
	string res(115, '0');
	int carry = 0;
	int index = 114;

	for (int i = s.length() - 1; i >= 0; i--)
		res[index--] = s[i];

	for (int i = 0; i < times; i++)
		for (int j = 114; j >= 0; j--)
		{
			int local_product = carry + (res[j] - '0') * n;
			res[j] = (local_product % 10) + '0';
			carry = local_product / 10;
		}

	while (res.front() == '0' && res.length() > width)
		res.erase(0, 1);

	s = res;
}

bool QFloat::fracMul2(string &s)
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
	
	while (s_fractional.back() == '0' && s_fractional.length() > 1)
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
			exponent = -16383;		
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

string QFloat::addStrings(const string& s1, const string& s2, bool left_align)
{
	/*
	- Vì tổng sigma (1/2)^n tiến về 1 khi n tiến về vô cùng => phần nguyên chỉ có 1 chữ số
	=> Chỉ cần thêm đủ số 0 vào phần thập phân để cộng từ cuối lên đầu (không ảnh hưởng kết quả)
	Cộng 2 chuỗi được thực hiện theo thứ tự từ cuối lên đầu.
	- Biến nhớ (carry) trong quá trình cộng.
	* Mặc định hai chuỗi là đều dương.
	*/

	string res(115, '0');
	int carry = 0;
	int index_1 = s1.length() - 1, index_2 = s2.length() - 1;
	if (!left_align)
	{
		for (int i = 0; i < 115; i++)
		{
			int local_sum = carry + (index_1 >= 0 ? s1[index_1--] : '0') - '0' + (index_2 >= 0 ? s2[index_2--] : '0') - '0';
			res[114 - i] = local_sum % 10 + '0';
			carry = local_sum / 10; 
		}

		while (res.front() == '0' && res.length() > 1)
			res.erase(0, 1);
	}
	else
	{
		for (int i = 114; i >= 0; i--)
		{
			int local_sum = carry + (i <= index_1 ? s1[i] : '0') - '0' + (i <= index_2 ? s2[i] : '0') - '0';
			res[i] = local_sum % 10 + '0';
			carry = local_sum / 10; 
		}
	}
	
	return res;
}

string QFloat::printQFloat() 
{
	int expo = this->exponent();
	bool *bits = this->decToBin();
	bool is_negative = bits[0];
	
	// Xử lý các trường hợp đặc biệt:
	// +0: 0 | 0 | 0						
	// -0: 1 | 0 | 0
	// denormalized: any | 0 | <> 0
	// inf: any | 2^14  | 0
	// NaN: any | 2^14  | <> 0
	// Thấy cái exp của inf với NaN sai sai... 16384 chứ nhỉ

	cout << expo << endl;
	int type_number = (expo == -16383 ? 1 : (expo == 32767 ? 2 : 3));
	string s = "Denormalized";
	bool denormalized = false;


//	cout << "Type: "<<type_number << endl;
	if (type_number < 3)
	{
		bool has_one = false;
		for (int i = 16; i < 128 && !has_one; i++)
			if (bits[i])
				has_one = true;

		if (has_one && type_number == 1)
			denormalized = true;
		else
			s = (has_one ? "NaN" : (type_number == 1 ? "0.0" : "inf"));
	}
	else
	{
		// Nhân phần mũ để được dãy trước khi chuẩn hóa. Chia hai trường hợp mũ không âm và mũ âm.
		// bfr_radixpt là phần nguyên trước khi chuẩn hóa.
		// aft_radixpt là phần thập phân trước khi chuẩn hóa.
		vector<bool> bfr_radixpt, aft_radixpt;
		if (expo >= 0)
		{
			bfr_radixpt.push_back(1);
			for (int i = 16; i < 128; i++)
				(i - 16 < expo) ? bfr_radixpt.push_back(bits[i]) : aft_radixpt.push_back(bits[i]);
		}
		else
		{
			bfr_radixpt.push_back(0);
			for (int i = 0; i < -expo - 1; i++)
				aft_radixpt.push_back(0);
			aft_radixpt.push_back(1);		
			for (int i = 16; i < 128; i++)
				aft_radixpt.push_back(bits[i]);
		}
//		for (int i = 0; i < aft_radixpt.size(); i++)
//			cout << aft_radixpt[i];
//		cout << endl;

		string s_integer = "0";
		string power_of_two = "1";
		int power = 0;

		for (int i = 0; i < bfr_radixpt.size(); i++)
		{
			if (bfr_radixpt[bfr_radixpt.size() - 1 - i] == 1)
			{
				QFloat::strMulN(power_of_two, i - power, 2);
				power = i;
				s_integer = addStrings(s_integer, power_of_two);
			}
		}


		/*
		- Tính phần thập phân bằng công thức: 2^-1 + 2^-2 +...
		Lại có 2^-n = 1 / 2^n => 10^n / 2^n = 5^n và dịch dấu phẩy sang trái n đơn vị
		Như vậy, thay vì tính 2^-n, ta tính 5^n và dời dấu phẩy sang trái n đơn vị rồi cộng chuỗi
		*/
		string s_fractional = "0";
		string power_of_five = "5";
		power = 1;

		for (int i = 1; i <= aft_radixpt.size(); i++)
		{
			if (aft_radixpt[i - 1] == 1)
			{
				QFloat::strMulN(power_of_five, i - power, 5, i);
				power = i;
				s_fractional = addStrings(s_fractional, power_of_five, true);
			}
		}

		s = (is_negative ? "-" : "") + s_integer + "." + s_fractional;
	}

	if (denormalized == 1)
	{
		
		expo = -16382;
		int exp = 0;
		vector<bool> aft_radixpt;

		string s_fractional = "0";
		string power_of_five = "5";
		int power = 1;
		
		for (int i = 16; i < 128; i++)
			aft_radixpt.push_back(bits[i]);

		for (int i = 1; i <= aft_radixpt.size(); i++)
		{
			if (aft_radixpt[i - 1] == 1)
			{
				QFloat::strMulN(power_of_five, i - power, 5, i);
				power = i;
				s_fractional = addStrings(s_fractional, power_of_five, true);
			}
		}

		while (s_fractional.back() == '0' && s_fractional.length() > 1)
			s_fractional.pop_back();

		
		string res(41, '0');	
		if (s_fractional.size() > 20)
			s_fractional.erase(20, s_fractional.size() - 20);

		exp = -s_fractional.size();

		int n = 16382;
		for (int i = 0; i < n; i++)
		{
			int carry = 0;
			int index = 40;

			for (int i = s_fractional.length() - 1; i >= 0; i--)
				res[index--] = s_fractional[i];

			for (int j = 40; j >= 0; j--)
			{
				int local_product = carry + (res[j] - '0') * 5;
				res[j] = (local_product % 10) + '0';
				carry = local_product / 10;
			}
			s_fractional = res;

			if (s_fractional.front() != '0')
			{
				string tmp = "00000000000000000000" + s_fractional;
				tmp.erase(41, 20);
				exp += 20;
				s_fractional = tmp;
			}

		}

		int tmp = 0;
		while (s_fractional.front() == '0'&&s_fractional.length() > 1)
		{
			s_fractional.erase(0, 1);
			tmp++;
		}
		s_fractional.insert(1, 1, '.');
		exp = exp + 40 - tmp - n;
		s = s_fractional;

		s.erase(17, s.size() - 17);

		string result;
		ostringstream convert;
		convert << exp;
		result = convert.str();

		s = s + "e" + result;

	}


	delete[] bits;
	return s;
}

bool* QFloat::convertTo2sComplement(bool* unsigned_bits, int length)
{
	bool *res = new bool[length];
	int carry = 0;
	
	for (int i = length - 1; i >= 0; i--)
 	{
		int local_diff = abs(0 - unsigned_bits[i] - carry);
		res[i] = local_diff % 2;
		carry = (unsigned_bits[i] + carry) > 0;
	}

	return res;
}

void QFloat::shiftRight(bool* bits, int start_pos, int length, int k)
{
	for (int i = length - 1; i >= start_pos; i--)
		bits[i] = (i - k >= start_pos ? bits[i - k] : 0);
}

void QFloat::shiftLeft(bool *bits, int start_pos, int length, int k)
{
	for (int i = start_pos; i < length; i++)
		bits[i] = (i + k < length ? bits[i + k] : 0);
}

bool* QFloat::addBitArrays(bool *bits_1, bool *bits_2, int length)
{
	bool *bits_sum = new bool[length];
	int local_sum, carry = 0;

	for (int i = length - 1; i >= 0; i--)
	{
		local_sum = bits_1[i] + bits_2[i] + carry;
		bits_sum[i] = local_sum % 2;
		carry = local_sum / 2;
	}

	return bits_sum;
}

bool *QFloat::subtractBitArrays(bool *bits_1, bool *bits_2, int length)
{
	bool *converted = QFloat::convertTo2sComplement(bits_2, length);
	bool *res = addBitArrays(bits_1, converted, length);
	delete[] converted;
	return res;
}

bool* QFloat::combineBits(bool sign, int exponent, bool* mantissa, int offset_mantissa) const
{
	bool *bits = new bool[128];
	bits[0] = sign;
	exponent += 16383;
	
	for (int i = 0; i < 15; i++)
		bits[1 + i] = 1 & (exponent >> (14 - i));

	for (int i = 0; i < 112; i++)
		bits[16 + i] = mantissa[offset_mantissa + i];
	
	return bits;
}

QFloat QFloat::operator+(const QFloat& another) const
{
/*
Phép cộng hai số thực lớn: tham khảo https://www.cs.colostate.edu/~cs270/.Fall18/resources/FloatingPointExample.pdf
- Chuyển số thực về cùng mũ (số thực có mũ nhỏ hơn được đẩy lên để giống với mũ của số còn lại).
- Thực hiện cộng trên phần trị.
- Chuẩn hóa lại kết quả.
*/
	bool *bits[] = { this->decToBin(), another.decToBin() };
	int exponent[] = { this->exponent(), another.exponent() };
	bool sign[] = { this->firstBit(), another.firstBit() };

	// Nếu hai số thực cùng dấu có thể dẫn đến trường hợp tràn, ta lưu vào biến has_same_sign.
	bool has_same_sign = (sign[0] == sign[1]);

	// Tách phần trị và bit ẩn của cả hai số (giả sử ta thực hiện phép cộng trên hai số đều đã chuẩn hóa)
	// và lưu vào mảng mantissa[115] = [2 bit đệm][1 bit ẩn][112 bit trị]
	bool *mantissa[2];
	for (int i = 0; i < 2; i++)
	{
		mantissa[i] = new bool[115];
		mantissa[i][0] = mantissa[i][1] = 0;
		mantissa[i][2] = 1;

		for (int j = 0; j < 112; j++)
			mantissa[i][3 + j] = bits[i][16 + j];
	}

	// Đưa về cùng mũ bằng cách dịch bit của số có mũ nhỏ hơn lên (không cần dịch 2 bit đệm).
	int smaller = (exponent[0] > exponent[1]);
	int k = abs(exponent[0] - exponent[1]);
	shiftRight(mantissa[smaller], 2, 115, k);

	// Sau khi dịch, kết quả mặc định có số mũ bằng số mũ lớn hơn.
	int exponent_sum = exponent[1 - smaller];

	// Nếu toán hạng nào là đang là âm, chuyển sang dạng bù 2
	// Với trường hợp cả hai cùng âm, ta không cần đổi dấu. Dấu kết quả đã được xác định trước đó.
	for (int i = 0; i < 2; i++)
		if (!has_same_sign && sign[i])
		{
			bool *converted_mantissa = QFloat::convertTo2sComplement(mantissa[i], 115);
			delete[] mantissa[i];
			mantissa[i] = converted_mantissa;
		}

	// Thực hiện cộng phần trị (cộng cả bit đệm)
	bool *mantissa_sum = addBitArrays(mantissa[0], mantissa[1], 115);

	// Nếu hai số cùng dấu, tổng sẽ mang dấu của cả hai. Ngược lại, dấu sẽ được lưu tại bit đầu tiên.
	bool sign_sum = (has_same_sign ? sign[0] : mantissa_sum[0]);
	
	// Overflow xảy ra khi thực hiện cộng hai số cùng dấu, kết quả bị tràn 1 tại bit ẩn.
	bool overflow = (has_same_sign && mantissa[0][2] + mantissa[1][2] != mantissa_sum[2]);

	// Nếu kết quả ra âm, chuyển từ dạng bù 2 sang dấu lượng
	if (mantissa_sum[0])
	{
		bool *converted_mantissa_sum = QFloat::convertTo2sComplement(mantissa_sum, 115);
		delete[] mantissa_sum;
		mantissa_sum = converted_mantissa_sum;
	}

	// Underflow xảy ra đối với hai số khác dấu, khi bit ẩn không phải là số 1
	bool underflow = (!has_same_sign && mantissa_sum[2] != 1);

	// Chuẩn hóa kết quả: nếu overflow thì dịch phần trị sang phải 1 bit, tăng mũ, 1 bị tràn trở thành bit ẩn mới.
	if (overflow)
	{
		exponent_sum++;
		shiftRight(mantissa_sum, 2, 115, 1);
		mantissa_sum[2] = 1;
	}

	// Chuẩn hóa kết quả: nếu underflow thì tìm bit đầu tiên là 1 ở bên phải, dịch phần trị sang trái và giảm mũ.
	if (underflow)
	{
		int shift = -1;
		for (int i = 0; i < 112 && shift != -1; i++)
			if (mantissa_sum[2 + i])
				shift = i;

		// Trường hợp đặc biệt: nếu phần trị toàn 0 thì kết quả được chuyển thành số 0 (tức phần mũ bằng 000000000000000000)
		if (shift == -1)
			exponent_sum = -16383;
		else
		{
			exponent_sum -= shift;
			shiftLeft(mantissa_sum, 2, 115, shift);
		}
	}

	bool *bits_sum = combineBits(sign_sum, exponent_sum, mantissa_sum, 3);
	QFloat res;
	res.binToDec(bits_sum);
	delete[] bits[0], bits[1], bits_sum, mantissa[0], mantissa[1], mantissa_sum;
	return res;
}

QFloat QFloat::operator-(const QFloat& another) const
{
	QFloat temp(another);
	temp.changeBit(0, 1 - temp.firstBit());			// Đổi dấu số trừ
	QFloat res = *this + temp;
	return res;
}

QFloat QFloat::operator*(const QFloat& another) const
{
/*
Phép nhân hai số thực lớn:
- Mũ của kết quả bằng tổng mũ.
- Dấu của kết quả là âm nếu hai số khác dấu. Ngược lại là dương.
- Thực hiện nhân trên phần trị (nhân đầy đủ).
- Chuẩn hóa lại kết quả.
*/

	int exponent_product = this->exponent() + another.exponent();
	bool sign_product = (this->firstBit() != another.firstBit());
	
	// Tách phần trị và bit ẩn. Do phép nhân được thực hiện trên phần trị là phép nhân đầy đủ nên cần 2 * 113 bit.
	// Như vậy, phần trị có dạng: [113 bit 0][1 bit ẩn][112 bit trị]
	bool *bits[] = { this->decToBin(), another.decToBin() };
	bool *mantissa[2];
	int length = 2 * 113;

	for (int i = 0; i < 2; i++)
	{
		mantissa[i] = new bool[length];
		mantissa[i][113] = 1;

		for (int j = 0; j < 112; j++)
			mantissa[i][114 + j] = bits[i][16 + j];
	}

	// Thực hiện phép nhân đầy đủ trên phần trị bằng thuật toán Booth
	bool prev_Q = 0;
	bool *Q = mantissa[0], *M = mantissa[1];
	bool *A = new bool[length];
	for (int i = 0; i < length; i++) A[i] = 0;

	for (int i = 0; i < length; i++) {
		if (Q[length - 1] != prev_Q) {
			bool *new_A = (Q[length - 1] < prev_Q ? addBitArrays(A, M, length) : subtractBitArrays(A, M, length));
			delete[] A;
			A = new_A;
		}

		prev_Q = Q[length - 1];
		shiftRight(Q, 0, length, 1);
		Q[0] = A[length - 1];
		bool sign_A = A[0];
		shiftRight(A, 0, length, 1);
		A[0] = sign_A;
	}

	// Tìm bit 1 đầu tiên của phần trị tích.
	int shift = -1;
	for (int i = 0; i < length && shift == -1; i++)
		if (Q[i]) shift = i;
	
	if (shift == -1) 		// Nếu phần trị toàn số 0 thì đưa về số 0, tức mũ bằng 000000000000000.
	{
		exponent_product = -16383;
	}
	else
	{
		// Mặc định, bit ẩn sẽ nằm ở vị trí số 1 trong phần trị tích.
		// Thế nên, nếu bit 1 đầu tiên nằm ở bên trái, ta tăng mũ và dịch phần trị sang phải.
		// Ngược lại, nếu bit 1 đầu tiên nằm ở bên phải, ta giảm mũ và dịch phần trị sang trái.
		if (shift < 1)
		{
			exponent_product += (1 - shift);
			shiftLeft(Q, 0, length, 1 - shift);
		}
		else if (shift > 1)
		{
			exponent_product -= (shift - 1);
			shiftRight(Q, 0, length, shift - 1);
		}
	}

	bool *bits_product = combineBits(sign_product, exponent_product, Q, 0);
	QFloat res;
	res.binToDec(bits_product);
	delete[] bits[0], bits[1], bits_product, mantissa[0], mantissa[1], A;
	return res;
}

QFloat QFloat::operator/(const QFloat& another) const
{
/*
Phép chia số thực lớn:
- Mũ của kết quả bằng hiệu mũ tổng
- Dấu của kết quả là âm nếu hai số khác dấu. Ngược lại là dương.
- Thực hiện chia trên phần trị như chia số nguyên không dấu.
- Chuẩn hóa lại kết quả.
*/

	int exponent_quotient = this->exponent() - another.exponent();
	bool sign_quotient = (this->firstBit() != another.firstBit());
	
	// Phần trị có dạng: [1 bit padding][1 bit ẩn][112 bit trị]
	bool *bits[] = { this->decToBin(), another.decToBin() };
	bool *mantissa[2];
	int length = 114;

	for (int i = 0; i < 2; i++)
	{
		mantissa[i] = new bool[length];
		mantissa[i][0] = 0;
		mantissa[i][1] = 1;

		for (int j = 0; j < 112; j++)
			mantissa[i][2 + j] = bits[i][16 + j];
	}

}

istream& operator>>(istream& is, QFloat& f)
{
	string s;
	is >> s;
	f.scanQFloat(s);

	return is;
}
ostream& operator<<(ostream& os, QFloat f)
{
	os << f.printQFloat();
	return os;
}
void QFloat::scanBits(string s)
{
	//Đưa một chuỗi 0 1 vào.
	bool* bits = new bool[128];
	for (int i = 0; i < 128 - s.length(); i++)
		bits[i] = 0;

	for (int i = 0; i < s.length(); i++)
		bits[128 - s.length() + i] = (s[i] - '0');

	binToDec(bits);
	delete[]bits;

}
string QFloat::printBits()
{
	//Hiện thị sang dạng 1 dãy bit
	string s = "";
	bool* bits = this->decToBin();

	for (int i = 0; i < 128; i++)
	{
		char tmp = bits[i] + '0';
		s += tmp;
	}
	delete[]bits;
	return s;
}
