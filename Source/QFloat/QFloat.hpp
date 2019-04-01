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

bool* QFloat::tran_right_nbit(bool* bits, int n)
{
	/*
	Dịch 1 dãy bit sang bên phải n bit
	*/
	bool* des = new bool[128];
	for (int i = 0; i < 128; i++)
	{
		if (i < n)
			des[i] = 0;
		else
			des[i] = bits[i - n];
	}

	return des;
}

bool* QFloat::tran_left_nbit(bool* bits, int n)
{
	/*
	Dịch 1 dãy bit sang bên trái n bit
	*/
	bool* des = new bool[128];
	for (int i = 0; i < 128; i++)
	{
		if (i < 128 - n)
			des[i] = bits[n + i];
		else
			des[i] = 0;
	}

	return des;
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


bool* QFloat::convertInterger(string s, int& exp_int)
{
/*
Chuyển phần nguyên của một số sang dạng nhị phân
- Tạo dãy bits[128]
- Tiến hành chia cho 2
- Nếu bé hơn 128 bits thì chia bình thường.
- Nếu vượt quá 128 bits thì: dịch mỗi lần 10 bits sang phải, tiếp tục chia. Lặp lại
- Xử lý dãy bits có được.

*/
	exp_int = 0;
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;
	int len_bit = 10 * s.length() / 3;	// Số lượng bits dự đoán cần để lưu số nguyên lớn s
	if (len_bit <= 128)
	{
		for (int i = 0; i < 128; i++)
		{
			int last_digit = s.back() - '0';
			bits[127 - i] = (last_digit % 2 == 1 ? 1 : 0);
			QFloat::strDiv2(s);
		}
	}
	else // Nếu lượng bits cần lưu vượt quá 128
	{
		int tmp = len_bit - 128;
		int i = 0;
		int j = 0;
		while (s != "0")
		{
			//Bỏ đi len_bit - 128 bits cuối cùng, không xét vào dãy bits.
			if (i < tmp)
			{
				QFloat::strDiv2(s);
				exp_int++;
				i++;
			}
			else
			{
				// Nếu qua vẫn còn hơn 128 bits (do len_bit chỉ là số dự đoán gần đúng)
				// Mỗi lần ta dịch 10 bits sang phải, vì chỉ cần lấy 112 bits trong phần trị nên không ảnh hưởng.
				if (i >= len_bit && j == 0)
				{
					// Trường hợp số rất rất lớn dẫn đến sai số bits dự đoán mới xảy  ra trường hợp này
					bool* trans_bit = QFloat::tran_right_nbit(bits, 10);
					delete[]bits;
					bits = trans_bit;
					j = 10;
					exp_int += 10;
				}
				int last_digit = s.back() - '0';
				bits[len_bit - 1 - i] = (last_digit % 2 == 1 ? 1 : 0);
				QFloat::strDiv2(s);
				(j == 0) ? i++ : j--;
			}
		}
	}

	// Xử lý
	bool is_zero = 1;
	for (int i = 0; i < 128; i++)
	{
		if (bits[i] == 1)
		{
			exp_int = exp_int + 127 - i;
			bool* tmp = QFloat::tran_left_nbit(bits, i+1);
			delete[]bits;
			bits = tmp;
			is_zero = 0;
			break;
		}
	}
	if (is_zero == 1)
		exp_int = -1;

	return bits;
}

bool* QFloat::convertDecimal(string s, int& exp_dec)
{
/*
Chuyển phần thập phân của một số sang dạng nhị phân
- Tiến hành chia s
- Bỏ hết các số 0 ở đầu dãy và số 1 đầu tiên trong dãy, biến exp_dec lưu số số 0
- Lưu các bit còn lại vào dãy

*/
	exp_dec = 0;
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	int i = 0;
	bool tmp ;
	bool flag = 0;
	bool is_zero = 1;
	while (s != "0" && i<128)
	{
		tmp = QFloat::strMul2(s);
		if (flag == 0)
		{
			if (tmp == 0)
			{
				exp_dec++;
				continue;
			}
			else
			{
				flag = 1;
				is_zero = 0;
			}
		}
		else
			bits[i ++] = tmp;
	}

	if (is_zero == 1)
		exp_dec = -1;

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
	int exp_int = 0;
	int exp_dec = 0;
	int exp = 0;
	string s_interger;
	string s_decimal;
	
	if (index_dot == -1)
	{
		s_interger = s;
		s_decimal = "0";
	}
	else
	{
		s_interger = s.substr(0, index_dot);
		s_decimal = s.substr(index_dot + 1, s.length() - index_dot - 1);
	}
	bool* interger = QFloat::convertInterger(s_interger, exp_int);
	bool* decimal = QFloat::convertDecimal(s_decimal, exp_dec);

	// 2. Chuyển về dãy bits[128]
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;

	if (is_negative)
		bits[0] = 1;
	// Chuyển phần nguyên và phần thập phân vào dãy bits.
	if (exp_int == -1 && exp_dec == -1)
	{
		exp = 16385;
		bits[0] = 0;
	}
	else if (exp_int>112)
	{
		exp = exp_int;
		for (int i = 0; i < 112; i++)
			bits[16 + i] = interger[i];
	}
	else if (exp_int == -1)
	{
		exp = -1 - exp_dec;
		for (int i = 0; i < 112; i++)
			bits[16 + i] = decimal[i];
	}
	else
	{
		if (exp_dec == -1)
		{
			exp = exp_int;
			for (int i = 0; i < exp_int; i++)
				bits[16 + i] = interger[i];
		}
		else
		{
			exp = exp_int;
			for (int i = 0; i < exp_int; i++)
				bits[16 + i] = interger[i];

			for (int i = 0; i < exp_dec; i++)
				bits[16 + exp_int + i] = 0;

			bits[16 + exp_int + exp_dec] = 1;

			for (int i = 16 + exp_int + exp_dec + 1; i < 112; i++)
				bits[i] = decimal[i - 16 - exp_int - exp_dec - 1];
		}
	}
	
	// Đưa phần mũ vào dãy bits
	bool *bias = QFloat::convertBias(exp);
	for (int i = 0; i < 15; i++)
		bits[i + 1] = bias[i];

	cout << exp << endl;
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


void QFloat::scan(string s)
{
/*
- Chuyển phần nguyên
- Nếu size-1>=112 -> exp =  size-1 ->đưa hết vào dãy bit  ->out
- Nếu size==0 -> xử lý phần thập phân  ->out				
- Nếu 0<size-1<112 -> exp = size-1 -> đưa vào dãy bit -> đưa dãy thập phân vào

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
	int exp = 0;
	string s_interger;
	string s_decimal;

	if (index_dot == -1)
	{
		s_interger = s;
		s_decimal = "0";
	}
	else
	{
		s_interger = s.substr(0, index_dot);
		s_decimal = s.substr(index_dot + 1, s.length() - index_dot - 1);
	}

	// 2. Chuyển về dãy bits[128]
	// Khởi tạo dãy bit
	bool* bits = new bool[128];
	for (int i = 0; i < 128; i++)
		bits[i] = 0;
	//Đưa dấu vào dãy bit
	if (is_negative)
		bits[0] = 1;
	// Xóa các số 0 thừa ở phần nguyên và kiểm tra xem phần nguyên có = 0 hay không?
	bool is_int_zero = 0;

	while (s_interger.back() == '0' && s_interger.length() > 1)
		s_interger.erase(s_interger.length() - 1, 1);

	if (s_interger == "0")
		is_int_zero = 1;

	// Tiến hành chuyển phần nguyên sang dãy bit
	vector<bool> interger;
	bool bit;
	while (s_interger != "0")
	{
		int last_digit = s_interger.back() - '0';
		bit = (last_digit % 2 == 1 ? 1 : 0);
		interger.push_back(bit);
		QFloat::strDiv2(s_interger);
	}
	cout << interger.size() << endl;

	// Nếu phần nguyên = 0, ta xét phần thập phân 
	if (is_int_zero == 1)
	{
		cout << "Tu 1" << endl;
		if (s_decimal == "0")// Số 0
		{
			exp = 16385;	// Để cho biễu diễn bias có dạng 0000 0000 0000 000
		}
		else{
			vector<bool> decimal;
			int count = 0;
			bool flag = 0;
			exp--;
			while (count < 112)
			{
				bool bit = QFloat::strMul2(s_decimal);
				if (bit == 0 && flag == 0)
					exp--;
				else if (flag == 1)
				{
					decimal.push_back(bit);
					count++;
				}
				else
					flag = 1;
			}

			for (int i = 0; i < 112; i++)
				bits[16 + i] = decimal[i];
		}

	}
	// Nếu phần nguyên lớn hơn 112 bit
	else if (interger.size() - 1 >= 112)
	{
		cout << "Tu 2" << endl;
		exp = interger.size() - 1;
		for (int i = 0; i < 112; i++)
			bits[16 + i] = interger[interger.size() - 2 - i];
	}
	else if (interger.size() >= 1)
	{
		cout << "Tu 3" << endl;
		exp = interger.size() - 1;
		for (int i = 0; i < interger.size() - 1; i++)
			bits[16 + i] = interger[interger.size() - 2 - i];
		
		int j = 0;
		while (s_decimal != "0"  && j < 112 - interger.size() - 1)
		{
			bool tmp = QFloat::strMul2(s_decimal);
			bits[15 + interger.size() + j++] = tmp;

		}
	}
	
	// Đưa phần mũ vào dãy bits
	bool *bias = QFloat::convertBias(exp);
	for (int i = 0; i < 15; i++)
		bits[i + 1] = bias[i];

	cout << exp << endl;
	for (int i = 0; i < 128; i++){
		cout << bits[i];
		if (i % 4 == 3) cout << " ";
	}
	cout << endl;


	// 3. binToDec
	binToDec(bits);
	delete[] bits;

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
			//Bổ sung sau
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

bool* QFloat::decToBin()
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
