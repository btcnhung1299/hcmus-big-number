#include "QFloat.h"

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

QFloat twoOperand1(QFloat operand_1, QFloat operand_2, string o)
{
	//Hàm kiểm tra và thực hiện cho + - * / & | ^
	QFloat res;
	if (o == "+")
		res = operand_1 + operand_2;
	else if (o == "-")
		res = operand_1 - operand_2;
	else if (o == "*")
		res = operand_1 * operand_2;
	else if (o == "/")
		res = operand_1 / operand_2;

	return res;
}

void solve(string s, ofstream& out)
{
	vector<string> splited = splitString(s);

	if (splited.size() == 4)
	{
		//Check cơ số
		int base = stoi(splited[0]);

		if (base == 2)
		{
			QFloat o1, o2;
			o1.scanBits(splited[1]);
			o2.scanBits(splited[3]);

			QFloat res;
			res = twoOperand1(o1, o2, splited[2]);
			out << res.printBits() << endl;
		}
		else if (base == 10)
		{
			QFloat o1, o2;
			o1.scanQFloat(splited[1]);
			o2.scanQFloat(splited[3]);

			QFloat res;
			res = twoOperand1(o1, o2, splited[2]);
			out << res << endl;
		}
	}
	else if (splited.size() == 3)
	{
		int base = stoi(splited[0]);
		int base_des = stoi(splited[1]);
		if (base == 2)
		{
			string des;
			QFloat d;
			d.scanBits(splited[2]);

			if (base_des == 10)
				out << d.printQFloat() << endl;
		}
		else if (base == 10)
		{
			QFloat d;
			d.scanQFloat(splited[2]);
			if (base_des == 2)
				out << d.printBits() << endl;
		}
	}
}

int main(int argc, char *argv[])
{
	string s1, s2;
	QFloat a, b;
	cout << "A: ";
	cin >> s1;
	a.scanQFloat(s1);
	cout << "A: " << a.printQFloat() << endl;
	cout << "B: ";
	cin >> s2;
	b.scanQFloat(s2);	
	cout << "B: " << b.printQFloat() << endl;
	QFloat c = a / b;
	cout << "C: " << c.printQFloat() << endl;

	return 0;

	//Note argc = 4
	// if (argc != 3)
	// {
	// 	cout << "Error!!" << endl;
	// 	return 0;
	// }

	// ifstream in(argv[1]);
	// ofstream out(argv[2]);

	// QFloat a;
	// int k;

	// if (!in)
	// {
	// 	cout << "Not found!" << endl;
	// 	return 0;
	// }

	// // Đọc file input vào vector
	// vector<string> input;
	// while (!in.eof())
	// {
	// 	char tmp[250];
	// 	in.getline(tmp, 225);

	// 	input.push_back(tmp);
	// }

	// for (int i = 0; i < input.size(); i++)
	// {
	// 	out << input[i] << endl;
	// 	solve(input[i], out);
	// 	out << "===================" << endl;
	// }

	// in.close();
	// out.close();
	
	return 0;
}
