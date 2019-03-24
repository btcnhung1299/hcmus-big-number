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

bool* QInt::convertTo2sComplement(bool* unsigned_bits) const
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
void QInt::multiply(int res[], int& resSize, int x)
{
	/*
	- Khởi tạo biến nhớ carry = 0
	- Tạo vòng lặp chạy từ i = 0 -> res_size - 1
		+ Đặt prod = res[i] * x + carry
		+ Lưu chữ số cuối cùng của prod vào res[i] và các chữ số còn lại vào carry.
	- Lưu tất cả chữ số của carry vào res và tăng res_size bằng lượng số chữ số của carry
	*/

	int carry = 0, i = 0, prod = 0;

	for (i = 0; i < resSize; i++)
	{
		prod = res[i] * x + carry;
		res[i] = prod % 10;
		carry = prod / 10;
	}

	while (carry)
	{
		res[resSize++] = carry % 10;
		carry /= 10;
	}
}
string QInt::calculatePowerOf2(int n)
{
	/*
	Tính lũy thừa của 2^n dưới dạng chuỗi theo thuật toán:
	- Tạo 1 mảng res[] với số lượng chữ số tối đa là MAX. 
	- Khởi tạo res_size = 1 và res[0] = 1
	- Tạo vòng lặp chạy từ i = 1 -> n: Thực hiện hàm multiply
	*/

	int res[MAX_NUM_OF_DIGITS];
	int resSize = 1;
	res[0] = 1;
	int i = 1;
	
	for (i = 1; i <= n; i++)
		multiply(res, resSize, 2);


	string result;
	for (i = resSize - 1; i >= 0; i--)
		result.append(to_string(res[i]));
	
	return result;
}
void QInt::swapArr(int a[], int b[], int &aSize, int &bSize)
{
	//Do 2 mảng tĩnh a, b có số lượng phần tử tối đa như nhau nên ta có thể tạm swap bằng cách này
	int i = 0;
	int *tmp = new int[aSize];
	int tmpSize = aSize;

	for (i = 0; i < aSize; i++)
		tmp[i] = a[i];

	for (i = 0; i < bSize; i++)
		a[i] = b[i];
	aSize = bSize;

	for (i = 0; i < tmpSize; i++)
		b[i] = tmp[i];
	bSize = tmpSize;
}
void QInt::add(int res[], int a[], int &resSize, int aSize)
{
	int i = 0, sum = 0, carry = 0;

	//Mặc định chuỗi thứ 1 ngắn hơn chuỗi thứ 2 
	if (resSize > aSize)
		swapArr(res, a, resSize, aSize);

	//Cộng các phần tử của mảng theo đúng thứ tự đơn vị, chục, trăm, v.v
	for (i = 0; i < resSize; i++)
	{
		sum = res[i] + a[i] + carry;
		res[i] = sum % 10;
		carry = sum / 10;
	}

	int tmp = resSize; //Đặt biến tạm tmp để sau đó tăng chiều dài của chuỗi thứ 1 lên (nếu có)
	//Trường hợp 2 chuỗi không cùng chiều dài, cộng thêm các số của chuỗi dài hơn
	for (i = tmp; i < aSize; i++)
	{
		sum = a[i] + carry;
		res[resSize++] = sum % 10;
		carry = sum / 10;
	}

	while (carry)
	{
		res[resSize++] = carry % 10;
		carry /= 10;
	}
}
string QInt::multiplyTwoStrings(string str1, string str2)
{
	/*
	Dựa vào ý tưởng nhân 1 chuỗi với 1 số, mở rộng thành nhân 2 chuỗi:
	- Chuyển 2 chuỗi về 2 mảng số nguyên.
	- Tạo 2 vòng lặp, cho các phần tử của số thứ 1 nhân lần lượt với hàng đơn vị, hàng chục, hàng trăm...
	của số thứ 2 và cộng các chuỗi tích lại.
	*/
	int a[MAX_NUM_OF_DIGITS];
	int b[MAX_NUM_OF_DIGITS];
	int sum[MAX_NUM_OF_DIGITS];
	int aSize = 0, bSize = 0, sumSize = 1; //Sum có 1 phần tử 0
	sum[0] = 0;
	int i = 0, j = 0;

	for (i = str1.length() - 1; i >= 0; i--)
		a[aSize++] = str1[i] - '0';
	
	for (i = str2.length() - 1; i >= 0; i--)
		b[bSize++] = str2[i] - '0';

	/*
	Biến đếm cnt tương ứng với vị trí chữ số của b[i]
	- Sau vòng lặp bên trong, biến cnt tăng lên 10 lần biểu thị b[i] đang tăng từ hàng đơn vị đến chục, trăm..
	*/
	int cnt = 1;
	for (i = 0; i < bSize; i++)
	{
		//Tạo mảng tạm prod chứa tích của chuỗi thứ 1 với từng hàng của chuỗi thứ 2
		int *prod = new int[aSize];
		int prodSize = aSize;
		for (j = 0; j < aSize; j++)
			prod[j] = a[j];

		//for (j = 0; j < aSize; j++)
		multiply(prod, prodSize, b[i] * cnt);

		//Cộng dồn chuỗi vào chuỗi tổng sum
		add(sum, prod, sumSize, prodSize);

		cnt *= 10;
	}
		
	string result;
	for (i = sumSize - 1; i >= 0; i--)
		result.append(to_string(sum[i]));

	return result;
}
string QInt::addTwoStrings(string str1, string str2)
{
	/*
	Ý tưởng tương tự với cộng 2 chuỗi theo kiểu chuyển về int, chỉ khác là thao tác trực tiếp trên chuỗi.
	- Đảo ngược 2 chuỗi.
	- Cộng theo thứ tự từ hàng đơn vị rồi lưu vào chuỗi tổng.
	*/

	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());

	//Quy ước str1 luôn ngắn hơn str2
	if (str1.length() > str2.length())
		swap(str1, str2);

	int carry = 0;
	int i = 0, sum = 0;
	string result;

	for (i = 0; i < str1.length(); i++)
	{
		sum = str1[i] - '0' + str2[i] - '0' + carry;
		result.push_back((sum % 10) + '0');
		carry = sum / 10;
	}
	
	//Xử lý các số còn lại của chuỗi dài hơn
	for (i = str1.length(); i < str2.length(); i++)
	{
		sum = str2[i] - '0' + carry;
		result.push_back((sum % 10) + '0');
		carry = sum / 10;
	}

	while (carry)
	{
		result.push_back((carry % 10) + '0');
		carry /= 10;
	}
	
	//Đảo ngược chuỗi kết quả để thu được tổng
	reverse(result.begin(), result.end());

	while (result[0] == '0')
		result.erase(0, 1);

	return result;
}
bool QInt::isSmaller(string str1, string str2)
{
	int len1 = str1.length();
	int len2 = str2.length();
	
	if (len1 < len2)
		return true;
	else if (len1 > len2)
		return false;

	//Trường hợp len1 = len2
	int i = 0;
	for (i = 0; i < len1; i++)
	{
		if (str1[i] < str2[i])
			return true;
		else if (str1[i] > str2[i])
			return false;
	}
}
string QInt::subtractTwoStrings(string str1, string str2)
{
	/*
	Ý tưởng cũng tương tự hàm addTwoStrings:
	- Đảo 2 chuỗi.
	- Trừ lần lượt từng vị trí của 2 chuỗi.
	- Quy ước str1 đóng vai trò là số bị trừ, str2 là số trừ.
	- Thêm hàm so sánh 2 số str1 và str2 để xác định dấu của chuỗi hiệu.
	- Str2 luôn mang dấu '-', còn str1 do trong bài này là cộng dồn chuỗi s nên sẽ mang dấu '+'
	*/

	string result = "";
	bool neg = false;
	//Quy ước số thứ 1 có trị tuyệt đối lớn hơn số thứ 2
	if (isSmaller(str1, str2))
	{
		neg = true;
		swap(str1, str2);
	}

	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());


	/*
	Thuật toán cho phép trừ:
	- Nếu dif < 0 thì mượn thêm 10 cho dif và ghi nợ 1 đơn vị ở biến nhớ carry.
	- Nếu dif >= 0 thì carry = 0
	*/
	int i = 0;
	int dif = 0, carry = 0;
	for (i = 0; i < str2.length(); i++)
	{
		dif = (str1[i] - '0') - (str2[i] - '0') - carry;

		if (dif < 0)
		{
			dif += 10;
			carry = 1;
		}
		else
			carry = 0;

		result.push_back(dif + '0');
	}

	//Xử lý thao tác cho phần còn lại của chuỗi lớn hơn
	for (i = str2.length(); i < str1.length(); i++)
	{
		dif = (str1[i] - '0') - carry;

		if (dif < 0)
		{
			dif += 10;
			carry = 1;
		}
		else
			carry = 0;
		result.push_back(dif + '0');
	}

	reverse(result.begin(), result.end());

	if (neg)
	{
		while (result[0] == '0')
			result.erase(0, 1);
		result = '-' + result;
	}
	else
	{
		while (result[0] == '0')
			result.erase(0, 1);
	}
	return result;
}
void QInt:: printQInt()
{
	/*
	Để in ra giá trị của số lớn QInt, ta cần xử lý tính toán theo thuật toán:
	- QInt = sigma(bits[i] * 2^(127 - i)) (i = 0 -> 127)
	- với bits là dạng nhị phân của số QInt
	Cần bổ sung 4 hàm:
	- Tính lũy thừa 2^n cho số lớn dạng chuỗi
	- Nhân 2 chuỗi
	- Cộng 2 chuỗi
	- Trừ 2 chuỗi
	*/
	bool *bits = decToBin();
	int i = 0;
	string s = "";

	for (i = 1; i < 128; i++)
		s = addTwoStrings(s, multiplyTwoStrings(calculatePowerOf2(127 - i), to_string(bits[i])));

	if (bits[0] == 0)
		s = addTwoStrings(s, multiplyTwoStrings(calculatePowerOf2(127), to_string(bits[0])));
	else
		s = subtractTwoStrings(s, multiplyTwoStrings(calculatePowerOf2(127), to_string(bits[0])));	

	cout << s << endl;
}
int QInt::binToDec4bits(string bin)
{
	int sum = 0;
	int i = 0;

	for (i = 0; i < 4; i++)
		sum += (bin[i] - '0') * (int)(pow(2, 3 - i));

	return sum;
}
string* QInt::binToHex(bool *bits)
{
	string *hex = new string[32];
	int i = 0, j = 0, k = 0;

	//Tách 128 bits thành 32 cụm, mỗi cụm được lưu vào 1 phần tử của mảng hex

	for (i = 0; i < 127; i += 4)
	{ 
		for (j = i; j < i + 4; j++)
		{
			hex[k].push_back(bits[j] + '0');
		}
		k++;
	}
		

	//Chuyển bit sang giá trị thập phân để đưa về hệ thập lục phân
	for (i = 0; i < 32; i++)
	{
		switch (binToDec4bits(hex[i]))
		{
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
		{
			hex[i] = to_string(binToDec4bits(hex[i]));
			break;
		}
		case 10:
		{
			hex[i] = "A";
			break;
		}
		case 11:
		{
			hex[i] = "B";
			break;
		}
		case 12:
		{
			hex[i] = "C";
			break;
		}
		case 13:
		{
			hex[i] = "D";
			break;
		}
		case 14:
		{
			hex[i] = "E";
			break;
		}
		case 15:
		{
			hex[i] = "F";
			break;
		}
		default:
		{
		}
		}
	}
	return hex;
}
string* QInt::decToHex(QInt x)
{
	//Kết hợp 2 hàm từ decToBin và binToHex
	bool *bits = decToBin();
	string *hex = binToHex(bits);

	return hex;
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

	for (int i = 0; i < 128; i++) {
		if (!bits[i]) continue;
		index = i / 32;
		k = i % 32;
		data[index] = data[index] | (1 << (31 - k));
	}
	
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

void QInt::fillOnes()
{
// Hàm fill bit 1: gán dãy bit 11111...1111 (32 bit) = -1 vào từng ô.
	for (int i = 0; i < 4; i++)
		data[i] = -1;
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
QInt QInt::operator&(const QInt& another) const
{
	/*
	- Phép AND cho kết quả bằng 1 khi cả 2 bit cùng mang giá trị 1
	- Các trường hợp còn lại bằng 0
	*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = another.decToBin();

	bool *bits_res = new bool[128];
	int i = 0;

	for (i = 0; i < 128; i++)
	{
		if (bits_1[i] == 1 && bits_2[i] == 1)
			bits_res[i] = 1;
		else
			bits_res[i] = 0;
	}

	QInt res;
	res.binToDec(bits_res);

	return res;
}
QInt QInt::operator|(const QInt& another) const
{
	/*
	- Phép OR cho giá trị 0 khi cả 2 bit đều bằng 0.
	- Các trường hợp còn lại bằng 1.
	*/

	bool *bits_1 = this->decToBin();
	bool *bits_2 = another.decToBin();

	bool *bits_res = new bool[128];
	int i = 0;

	for (i = 0; i < 128; i++)
	{
		if (bits_1[i] == 0 && bits_2[i] == 0)
			bits_res[i] = 0;
		else
			bits_res[i] = 1;
	}

	QInt res;
	res.binToDec(bits_res);

	return res;
}
QInt QInt::operator^(const QInt& another) const
{
	/*
	- Phép XOR cho kết quả bằng 1 khi 2 bit mang giá trị ngược nhau (tổng = 1)
	*/
	bool *bits_1 = this->decToBin();
	bool *bits_2 = another.decToBin();

	bool *bits_res = new bool[128];
	int i = 0;

	for (i = 0; i < 128; i++)
	{
		if (bits_1[i] + bits_2[i] == 1)
			bits_res[i] = 1;
		else
			bits_res[i] = 0;
	}

	QInt res;
	res.binToDec(bits_res);

	return res;
}
QInt QInt::operator~() const
{
	/*
	Phép NOT là đảo bit
	*/
	bool *bits = this->decToBin();
	int i = 0;

	for (i = 0; i < 128; i++)
		bits[i] = 1 - bits[i];

	QInt res;
	res.binToDec(bits);

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
QInt QInt::operatorrol(int k) const
{
	//Ở phép xoay trái, bit trái cùng (MSB) sẽ được bỏ đi, và đưa về phía phải cùng (LSB).
	
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
QInt QInt::operatorror(int k) const
{
	//Ở phép xoay phải, bit phải cùng (LSB) sẽ được bỏ đi, và đưa về phía trái cùng (MSB).

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

	if (is_negative)
	{
		bool *bits = Q.decToBin();
		bool *converted_bits = Q.convertTo2sComplement(bits);
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

void QInt::printBit() const
{
	bool *bits = this->decToBin();
	for (int i = 0; i < 128; i++)
		cout << bits[i];
	cout << endl;
	delete[] bits;
}

