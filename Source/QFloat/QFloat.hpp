#include "QFloat.h"
#define MAX_SIZE 4932

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

void QFloat::changeBit(int pos, bool value)
{
	int index = pos / 16;
	int k = pos % 16;
	data[index] = (value ? data[index] | (1 << (15 - k)) : data[index] & ~(1 << (15 - k)));
}

int QFloat::exponent() const
{
	return int(data[0] & ~(1 << 15)) - 16383;
}

enum QFloat::Type QFloat::getType() const {
	bool *bits = this->decToBin();
	int expo = this->exponent();
	enum Type res = NORMAL;

	if (expo == -16383)
	{
		res = ZERO;
		for (int i = 16; i < 128; i++)
		{
			if (bits[i]) {
				res = SUBNORMAL;
				break;
			}
		}
	}
	else if (expo == 16384)
	{
		res = INF;
		for (int i = 16; i < 128; i++)
		{
			if (bits[i])
			{
				res = N_AN;
				break;
			}
		}
	}

	delete[] bits;
	return res;
}

bool* QFloat::setType(bool sign, QFloat::Type type)
{
	bool *bits = new bool[128];
	bits[0] = sign;

	if (type == ZERO)
		for (int i = 1; i < 16; i++)
			bits[i] = 0;
	else
		for (int i = 1; i < 16; i++)
			bits[i] = 1;

	for (int i = 16; i < 128; i++)
		bits[i] = 0;

	if (type == N_AN)
		bits[16] = 1;
	
	return bits;
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
Thực hiện nhân một chuỗi với một số N times lần.
- Khởi tạo biến kết quả chứa tối đa MAX_SIZE chữ số.
- Biến nhớ (carry) trong quá trình nhân.
- Chuẩn hóa chuỗi, bỏ đi những số 0 ở đầu sao cho kết quả đảm bảo luôn có ít nhất width chữ số.
*/
	if (times == 0) return;
	string res(MAX_SIZE, '0');
	int carry = 0, index = MAX_SIZE-1;

	for (int i = s.length() - 1; i >= 0; i--)
		res[index--] = s[i];

	for (int i = 0; i < times; i++)
		for (int j = MAX_SIZE - 1; j >= 0; j--)
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
Thực hiện nhân một chuỗi thập phân cho 2. Chuỗi có dạng: "12345" với ý nghĩa 0.12345 (tương tự như nhân số nguyên).
- Dừng lại khi chuỗi = "0".
- Chuẩn hóa chuỗi: loại bỏ các số 0 ở cuối. VD "12340" -> "1234".
- Trả về kết quả 0 hoặc 1 sau mỗi lần nhân.
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

bool* QFloat::convertToBias(int n)
{
// Đổi số n sang dạng bias bằng cách cộng cho (2^14 - 1), đổi sang dạng nhị phân.
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
	if (s.front() == '-')
	{
		bits[0] = 1;
		s.erase(0, 1);
	}

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
	- Theo quy ước chuẩn hóa, dấu chấm động được dời lên đằng sau số 1 đầu tiên.
	- Do đó, sẽ có (bits.size() - 1) bit ở phần nguyên này được đẩy xuống phần thập phân.
	- Mà ta chỉ có 112 bit để biểu diễn phần thập phân.
	- Nên nếu phần bit lấn qua quá lớn thì phần sau dấu chấm ban đầu sẽ không được lưu.
	- THDB: Nếu số mũ phần nguyên quá lớn, ta không thể biểu diễn được, lưu dưới dạng inf (mũ bằng 11111...1111).
	*/

	bool overflow = (bits_integer.size() - 1 > 16382);
	int exponent;
	
	// Nếu số cần biểu diễn không quá lớn và cũng không quá nhỏ, ta biểu diễn dưới dạng chuẩn
	if (!overflow && !might_denormalized)
	{
		exponent = bits_integer.size() - 1;

		int mantissa_int = min(112, int(bits_integer.size()) - 1);		// Số bit của phần nguyên được biểu diễn ở phần trị
		int mantissa_frac = 112 - mantissa_int;							// Số bit của phần thập phân được biểu diễn ở phần trị
		
		for (int i = 0; i < mantissa_int; i++)
			bits[16 + i] = bits_integer[i + 1];
		
		for (int i = 0; i < mantissa_frac; i++)								
			bits[16 + i + mantissa_int] = fracMul2(s_fractional);
	}

	// Nếu số cần biểu diễn quá lớn, gán giá trị bằng vô cực
	if (overflow) exponent = 16384;

	// Trường hợp số có dạng 0.0000... cần kiểm tra xem nên biểu diễn dưới dạng chuẩn hay không chuẩn
	if (might_denormalized) {
		if (s_fractional == "0") exponent = -16383;
		else
		{
			bool found_one = false;
			vector<bool> bits_fractional;
			exponent = -1;

			while (exponent > -16385 && bits_fractional.size() < 112)
			{
				bool bit = QFloat::fracMul2(s_fractional);

				if (!found_one) {
					bit ? found_one = true : exponent--;
					continue;
				}
				
				bits_fractional.push_back(bit);
			}

			if (bits_fractional.size() == 112)
				for (int i = 0; i < 112; i++)
					bits[16 + i] = bits_fractional[i];
		}
	}
	
	// Đưa phần mũ vào dãy bits
	bool *bias = QFloat::convertToBias(exponent);
	for (int i = 0; i < 15; i++)
		bits[i + 1] = bias[i];

	for (int i = 0; i < 128; i++) {
		if (i == 1 || i == 16) {
			cout << " ";
		}
		cout << bits[i];
	}
	cout << endl;

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

	string res(MAX_SIZE, '0');
	int carry = 0;
	int index_1 = s1.length() - 1, index_2 = s2.length() - 1;
	if (!left_align)
	{
		for (int i = 0; i < MAX_SIZE; i++)
		{
			int local_sum = carry + (index_1 >= 0 ? s1[index_1--] : '0') - '0' + (index_2 >= 0 ? s2[index_2--] : '0') - '0';
			res[MAX_SIZE - 1 - i] = local_sum % 10 + '0';
			carry = local_sum / 10; 
		}

		while (res.front() == '0' && res.length() > 1)
			res.erase(0, 1);
	}
	else
	{
		for (int i = MAX_SIZE-1; i >= 0; i--)
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
	enum QFloat::Type type = this->getType();
	bool is_negative = bits[0];
	string s = "";

	if (type == ZERO)
		s = (is_negative ? "-0.0" : "0.0");
	else if (type == INF)
		s = (is_negative ? "-INF" : "INF");
	else if (type == NAN)
		s = "NAN";
	else if (type == NORMAL)
	{
		// Nhân phần mũ để được dãy trước khi chuẩn hóa. Chia hai trường hợp mũ không âm và mũ âm.
		// bfr_radixpt là phần nguyên trước khi chuẩn hóa.
		// aft_radixpt là phần thập phân trước khi chuẩn hóa.
		vector<bool> bfr_radixpt, aft_radixpt;
		if (expo > 112)
		{
			bfr_radixpt.push_back(1);

			for (int i = 16; i < 128; i++)
				bfr_radixpt.push_back(bits[i]);
			
			for (int i = 112; i < expo; i++)
				bfr_radixpt.push_back(0);

			aft_radixpt.push_back(0);
		}
		else if (expo >= 0)
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

		while (s_fractional.back() == '0' && s_fractional.length() > 1)
			s_fractional.pop_back();

		s = (is_negative ? "-" : "") + s_integer + "." + s_fractional;
	}
	else if (type == SUBNORMAL)
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

bool* QFloat::combineBits(bool sign, int exponent, bool* mantissa, int offset_mantissa, int length_mantissa)
{
	bool *bits = new bool[128];
	bits[0] = sign;
	exponent += 16383;
	
	for (int i = 0; i < 15; i++)
		bits[1 + i] = 1 & (exponent >> (14 - i));

	for (int i = 0; i < 112; i++)
		bits[16 + i] = (offset_mantissa + i < length_mantissa ? mantissa[offset_mantissa + i] : 0);
	
	return bits;
}

bool QFloat::checkOverflow(bool *bits, int start_pos, int default_radixpt_pos, int length, int& exponent)
{
	int shift = -1;
	for (int i = start_pos; i < default_radixpt_pos; i++)
	{
		if (bits[i])
		{
			shift = default_radixpt_pos - i;
			break;
		}
	}

	if (shift == -1) {
		return false;
	}

	exponent += shift;
	if (exponent > 16383)
	{
		exponent = 16384;
		shift = length;
	}

	shiftRight(bits, start_pos, length, shift);
	return true;
}

bool QFloat::checkUnderflow(bool *bits, int default_radixpt_pos, int length, int& exponent)
{
	int shift = -1;
	for (int i = default_radixpt_pos; i < length; i++)
		if (bits[i])
		{
			shift = i - default_radixpt_pos;
			break;
		}

	// Trường hợp đặc biệt: nếu phần trị toàn 0 thì kết quả được chuyển thành số 0 (tức phần mũ bằng 000000000000000000)
	if (shift == -1)
		exponent = -16383;
	else
	{
		exponent -= shift;
		if (exponent < -16382) {
			shift = -16382 - exponent;
			exponent = -16383;
		}
		shiftLeft(bits, default_radixpt_pos, length, shift);
	}

	return true;
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
	int type_Q = this->getType(), type_M = another.getType();
	bool *bits_sum;

	if (type_Q == N_AN || type_M == N_AN)
		bits_sum = setType(0, N_AN);
	else if (type_Q == INF || type_M == INF)
		bits_sum = setType(0, INF);
	else
	{
		// Nếu hai số thực cùng dấu có thể dẫn đến trường hợp tràn, ta lưu vào biến has_same_sign.
		bool has_same_sign = (sign[0] == sign[1]);

		// Tách phần trị và bit ẩn của cả hai số (giả sử ta thực hiện phép cộng trên hai số đều đã chuẩn hóa)
		// và lưu vào mảng mantissa[115] = [2 bit đệm][1 bit ẩn][112 bit trị]
		bool *mantissa[2];
		int length = 115;

		for (int i = 0; i < 2; i++)
		{
			mantissa[i] = new bool[length];
			mantissa[i][0] = mantissa[i][1] = 0;
			mantissa[i][2] = 1;

			for (int j = 0; j < 112; j++)
				mantissa[i][3 + j] = bits[i][16 + j];
		}

		// Đưa về cùng mũ bằng cách dịch bit của số có mũ nhỏ hơn lên (không cần dịch 2 bit đệm).
		int smaller = (exponent[0] > exponent[1]);
		int k = abs(exponent[0] - exponent[1]);
		shiftRight(mantissa[smaller], 2, length, k);

		// Sau khi dịch, kết quả mặc định có số mũ bằng số mũ lớn hơn.
		int exponent_sum = exponent[1 - smaller];

		// Nếu toán hạng nào là đang là âm, chuyển sang dạng bù 2
		// Với trường hợp cả hai cùng âm, ta không cần đổi dấu. Dấu kết quả đã được xác định trước đó.
		for (int i = 0; i < 2; i++)
			if (!has_same_sign && sign[i])
			{
				bool *converted_mantissa = QFloat::convertTo2sComplement(mantissa[i], length);
				delete[] mantissa[i];
				mantissa[i] = converted_mantissa;
			}

		// Thực hiện cộng phần trị (cộng cả bit đệm)
		bool *mantissa_sum = addBitArrays(mantissa[0], mantissa[1], length);

		// Nếu hai số cùng dấu, tổng sẽ mang dấu của cả hai. Ngược lại, dấu sẽ được lưu tại bit đầu tiên.
		bool sign_sum = (has_same_sign ? sign[0] : mantissa_sum[0]);
		
		// Overflow xảy ra khi thực hiện cộng hai số cùng dấu, kết quả bị tràn 1.
		bool overflow = (has_same_sign && mantissa_sum[1] == 1);

		// Nếu kết quả ra âm, chuyển từ dạng bù 2 sang dấu lượng
		if (mantissa_sum[0])
		{
			bool *converted_mantissa_sum = QFloat::convertTo2sComplement(mantissa_sum, length);
			delete[] mantissa_sum;
			mantissa_sum = converted_mantissa_sum;
		}

		// Underflow xảy ra đối với hai số khác dấu, khi bit ẩn không phải là số 1
		bool underflow = (!has_same_sign && mantissa_sum[2] != 1);

		// Chuẩn hóa kết quả, giải quyết overflow / underflow.
		if (overflow)
			checkOverflow(mantissa_sum, 1, 2, length, exponent_sum);
		else if (underflow)
			checkUnderflow(mantissa_sum, 2, length, exponent_sum);

		bits_sum = combineBits(sign_sum, exponent_sum, mantissa_sum, 3);
		delete[] mantissa[0], mantissa[1], mantissa_sum;
	}

	QFloat res;
	res.binToDec(bits_sum);
	delete[] bits[0], bits[1], bits_sum;
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
- Dấu của kết quả là (-) nếu hai số khác dấu. Ngược lại là (+).
- Thực hiện nhân trên phần trị (nhân đầy đủ).
- Chuẩn hóa lại kết quả.
*/
	int exponent_product = this->exponent() + another.exponent();
	bool sign_product = (this->firstBit() != another.firstBit());
	bool *bits_product;
	
	// Tách phần trị và bit ẩn. Do phép nhân được thực hiện trên phần trị là phép nhân đầy đủ nên cần 2 * 113 bit.
	// Như vậy, phần trị có dạng: [113 bit 0][1 bit ẩn][112 bit trị]
	bool *bits[] = { this->decToBin(), another.decToBin() };
	int length = 2 * 113;
	bool prev_Q = 0, sign_A;
	bool *Q = new bool[length], *M = new bool[length], *A = new bool[length];

	enum QFloat::Type type_Q = this->getType();
	enum QFloat::Type type_M = another.getType();

	if ((type_Q == ZERO && type_M == INF) || (type_Q == INF && type_Q == ZERO))
		bits_product = setType(sign_product, N_AN);
	else if (type_Q == N_AN || type_M == N_AN)
		bits_product = setType(sign_product, N_AN);
	else if (type_Q == ZERO || type_M == ZERO)
		bits_product = setType(sign_product, ZERO);
	else if (type_Q == INF || type_M == INF)
		bits_product = setType(sign_product, INF);
	else
	{
		for (int i = 0; i < length; i++) {
			A[i] = 0;
			if (i == 113)
			{
				Q[i] = M[i] = 1;
			}
			else if (i < 113)
			{
				Q[i] = M[i] = 0;	
			}
			else
			{
				Q[i] = bits[0][i - 114 + 16];
				M[i] = bits[1][i - 114 + 16];
			}
		}
		
		// Thực hiện phép nhân đầy đủ trên phần trị bằng thuật toán Booth
		for (int i = 0; i < length; i++)
		{
			if (Q[length - 1] != prev_Q)
			{
				bool *new_A = (Q[length - 1] < prev_Q ? addBitArrays(A, M, length) : subtractBitArrays(A, M, length));
				delete[] A;
				A = new_A;
			}

			prev_Q = Q[length - 1];
			shiftRight(Q, 0, length, 1);
			Q[0] = A[length - 1];
			sign_A = A[0];
			shiftRight(A, 0, length, 1);
			A[0] = sign_A;
		}

		// Mặc định, bit ẩn sẽ nằm ở vị trí 1 trong phần trị tích.
		// Thế nên, nếu bit 1 đầu tiên nằm ở bên trái, ta tăng mũ và dịch phần trị sang phải.
		if (!QFloat::checkOverflow(Q, 0, 1, length, exponent_product)) {
			QFloat::checkUnderflow(Q, 1, length, exponent_product);
		}
		
		bits_product = combineBits(sign_product, exponent_product, Q, 2);
	}

	QFloat res;
	res.binToDec(bits_product);
	delete[] bits[0], bits[1], bits_product, Q, M, A;
	return res;
}

QFloat QFloat::operator/(const QFloat& another) const
{
/*
Phép chia số thực lớn:
- Mũ của kết quả bằng hiệu mũ tổng
- Dấu của kết quả là âm nếu hai số khác dấu. Ngược lại là dương.
- Thực hiện chia trên phần trị như chia số nguyên không dấu.
*/
	int exponent_quotient = this->exponent() - another.exponent();
	bool sign_quotient = (this->firstBit() != another.firstBit());
	bool *bits[] = { this->decToBin(), another.decToBin() };
	int type_Q = this->getType(), type_M = another.getType();
	bool *bits_quotient;

	if (type_M == ZERO)
		bits_quotient = setType(sign_quotient, N_AN);
	else if (type_Q == ZERO)
		bits_quotient = setType(sign_quotient, ZERO);
	else if (type_Q == N_AN || type_M == N_AN)
		bits_quotient = setType(sign_quotient, N_AN);
	else
	{
		// Phần trị có dạng: [1 bit sign][113 bit 0][1 bit ẩn][112 bit trị]
		int length = 2 * 113 + 1;					
		bool *A = new bool[length], *M = new bool[length], *Q = new bool[length];

		for (int i = 0; i < length; i++) {
			A[i] = 0;
			M[i] = 0;
			Q[i] = (i == 0 ? 0 : (i == 1 ? 1 : (i < 113 ? bits[0][i - 2 + 16] : 0)));
		}

		int id = length - 1;
		bool found_one = false;
		for (int i = 127; i >= 16; i--) {
			if (bits[1][i]) found_one = true;
			if (found_one) M[id--] = bits[1][i];
		}

		if (!found_one) M[id--] = 0;
		M[id--] = 1; M[id] = 0;
		int hidden_bit_pos = length - id - 1;
		
		// Thực hiện phép chia
		for (int i = 0; i < length; i++) {
			bool is_negative = A[0];
			shiftLeft(A, 0, length, 1);
			A[length - 1] = Q[0];
			shiftLeft(Q, 0, length, 1);
			
			bool *new_A = (is_negative ? addBitArrays(A, M, length) : subtractBitArrays(A, M, length));
			delete[] A;
			A = new_A;
			Q[length - 1] = !A[0];
		}

		if (!checkOverflow(Q, 0, hidden_bit_pos, length, exponent_quotient))
			checkUnderflow(Q, hidden_bit_pos, length, exponent_quotient);

		bits_quotient = combineBits(sign_quotient, exponent_quotient, Q, hidden_bit_pos + 1, length);
		delete[] Q, M, A;
	}

	QFloat res;
	res.binToDec(bits_quotient);
	delete[] bits[0], bits[1], bits_quotient;
	return res;
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
