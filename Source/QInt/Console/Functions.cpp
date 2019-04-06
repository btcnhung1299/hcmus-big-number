#include "Functions.h"
vector<string> splitString(string s)
{
	//Chia một chuỗi thành nhiều thành phần ngăn cách nhau bởi dấu ' '
	int index[5] = { -1 };

	// Tìm vị trí của các khoảng trắng
	for (int i = 1; i < 4; i++)
		index[i] = s.find(' ', index[i - 1] + 1);

	int size;
	size = (index[3] != -1) ? 4 : 3;

	vector<string> opera;
	index[size] = s.length();
	for (int i = 0; i < size; i++)
		opera.push_back(s.substr(index[i] + 1, index[i + 1] - index[i] - 1));

	return opera;
}
QInt twoOperand1(QInt operand_1, QInt operand_2, string o)
{
	//Hàm kiểm tra và thực hiện cho + - * / & | ^
	QInt res;
	if (o == "+")
		res = operand_1 + operand_2;
	else if (o == "-")
		res = operand_1 - operand_2;
	else if (o == "*")
		res = operand_1 * operand_2;
	else if (o == "/")
		res = operand_1 / operand_2;
	else if (o == "&")
		res = operand_1 & operand_2;
	else if (o == "|")
		res = operand_1 | operand_2;
	else if (o == "^")
		res = operand_1 ^ operand_2;

	return res;
}
bool twoOperand2(QInt operand_1, QInt operand_2, string o)
{
	//Hàm kiểm tra và thực hiện cho > < >= <= == !=
	bool res;
	if (o == ">")
		res = operand_1 > operand_2;
	else if (o == "<")
		res = operand_1 < operand_2;
	else if (o == ">=")
		res = operand_1 >= operand_2;
	else if (o == "<=")
		res = operand_1 <= operand_2;
	else if (o == "==")
		res = operand_1 == operand_2;
	else if (o == "!=")
		res = operand_1 > operand_2;


	return res;
}
QInt twoOperand3(QInt operand, int k, string o)
{
	//Hàm kiểm tra và thực hiện cho << >> ror rol
	QInt res;

	if (o == "<<")
		res = operand << k;
	else if (o == ">>")
		res = operand >> k;
	else if (o == "ror")
		res = operand.ror(k);
	else if (o == "rol") {
		res = operand.rol(k);
		//		cout << "Tu" << endl;
	}
	return res;
}


int typeOperator(string o)
{
	//Loại 1:  + -*/ &| ^
	//Loại 2: > < >= <= == != 
	//Loại 3: << >> ror rol
	//Loại 4: ~

	if (o == "+" || o == "-" || o == "*" || o == "/" || o == "&" || o == "|" || o == "^")
		return 1;
	else if (o == ">" || o == "<" || o == ">=" || o == "<=" || o == "==" || o == "!=")
		return 2;
	else if (o == ">>" || o == "<<" || o == "ror" || o == "rol")
		return 3;
	else if (o == "~")
		return 4;
	return 0;
}
