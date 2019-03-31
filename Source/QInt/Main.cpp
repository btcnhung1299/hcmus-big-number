#include "QInt.h"
#include <fstream>
#include <string>
#include <vector>

vector<string> splitString(string s)
{
	//Chia một chuỗi thành nhiều thành phần ngăn cách nhau bởi dấu ' '
	int index[5] = { -1 };

	// Tìm vị trí của các khoảng trắng
	for (int i = 1; i < 4; i++)
		index[i] = s.find(' ', index[i - 1] + 1);

	int size ;
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
		res = operand_1>operand_2;


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
	else if (o == "rol"){
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

	if (o == "+" || o == "-" || o == "*" || o == "/" || o == "&" || o == "|" || o == "^")
		return 1;
	else if (o == ">" || o == "<" || o == ">=" || o == "<=" || o == "==" || o == "!=")
		return 2;
	else if (o == ">>" || o == "<<" || o == "ror" || o == "rol")
		return 3;
	return 0;
}

void solve(string s, ofstream& out)
{
	vector<string> splited = splitString(s);

	if (splited.size()==4)
	{
		//Check cơ số
		int base = stoi(splited[0]);

		if (base == 2)
		{
			int type = typeOperator(splited[2]);//Kiểm tra toán tử thuộc loại nào
			
			if (type == 1 || type == 2)
			{
				QInt o1, o2;
				o1.scanBits(splited[1]);
				o2.scanBits(splited[3]);

				if (type == 1)
				{
					QInt res;
					res = twoOperand1(o1, o2, splited[2]);
					out << res.printBits() << endl;
				}
				else
				{
					bool res;
					res = twoOperand2(o1, o2, splited[2]);
					out << res << endl;
				}
				
			}
			else if (type == 3)
			{
				QInt o1;
				o1.scanBits(splited[1]);

				int  o2 = stoi(splited[3]);
				
				QInt res;
				res = twoOperand3(o1, o2, splited[2]);

				out << res.printBits() << endl;
			}
		}
		else if (base == 10)
		{
			int type = typeOperator(splited[2]);//Kiểm tra toán tử thuộc loại nào

			if (type == 1 || type == 2)
			{
				QInt o1, o2;
				o1.scanQInt(splited[1]);
				o2.scanQInt(splited[3]);

				if (type == 1)
				{
					QInt res;
					res = twoOperand1(o1, o2, splited[2]);
					out << res << endl;
				}
				else
				{
					bool res;
					res = twoOperand2(o1, o2, splited[2]);
					out << res << endl;
				}
			}
			else
			{
				QInt o1;
				o1.scanQInt(splited[1]);

				int  o2 = stoi(splited[3]);

				QInt res;
				res = twoOperand3(o1, o2, splited[2]);
				out << res<< endl;
			}
		}
		else
		{
			int type = typeOperator(splited[2]);//Kiểm tra toán tử thuộc loại nào

			if (type == 1 || type == 2)
			{
				QInt o1, o2;
				o1.scanHexs(splited[1]);
				o2.scanHexs(splited[3]);

				if (type == 1)
				{
					QInt res;
					res = twoOperand1(o1, o2, splited[2]);
					out << res.printHexs() << endl;
				}
				else
				{
					bool res;
					res = twoOperand2(o1, o2, splited[2]);
					out << res << endl;
				}
			}
			else
			{
				QInt o1;
				o1.scanQInt(splited[1]);

				int  o2 = stoi(splited[3]);

				QInt res;
				res = twoOperand3(o1, o2, splited[2]);
				out << res.printHexs() << endl;
			}
		}


	}
	else if (splited.size() == 3)
	{
		if (splited[1]=="~")
		{
			QInt d;
			int base = stoi(splited[0]);
			if (base == 2)
			{
				d.scanBits(splited[2]);
				d = ~d;
				out << d.printBits() << endl;
			}
			else if (base == 10)
			{
				d.scanQInt(splited[2]);
				d = ~d;
				out << d.printQInt() << endl;
			}
			else
			{
				d.scanHexs(splited[2]);
				d = ~d;
				out << d.printHexs() << endl;
			}

		}
		else
		{
			int base = stoi(splited[0]);
			int base_des = stoi(splited[1]);
			if (base == 2)
			{
				string des;
				QInt d;
				d.scanBits(splited[2]);
				if (base_des == 10)
				{
					out << d.printQInt() << endl;
				}
				else if (base_des == 16)
				{
					out << d.printHexs() << endl;
				}
			}
			else if (base == 10)
			{
				QInt d;
				d.scanQInt(splited[2]);
				if (base_des == 2)
				{
					out << d.printBits() << endl;
				}
				else if (base_des == 16)
				{
					out << d.printHexs() << endl;
				}
			}
			else if (base == 16)
			{
				QInt d;
				d.scanHexs(splited[2]);
				if (base_des == 2)
				{
					out << d.printBits() << endl;
				}
				else if (base_des == 10)
				{
					out << d.printQInt()<< endl;
				}
			}
		}
	}
}



int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Erorr!!" << endl;
		return 0;
	}

	ifstream in(argv[1]);
	ofstream out(argv[2]);

	QInt a;
	int k;

	if (!in)
	{
		cout << "Not found!" << endl;
		return 0;
	}

	// Đọc file input vào vector
	vector<string> input;
	while (!in.eof())
	{
		char tmp[250];
		in.getline(tmp,225);
		// Còn 1 chút bug nếu file input có hàng trống.
		input.push_back(tmp);
	}


	for (int i = 0; i < input.size(); i++)
	{
		out << input[i] << endl;
		solve(input[i], out);
		out << "===================" << endl;
	}



	
	
	in.close();
	out.close();
	return 0;
}