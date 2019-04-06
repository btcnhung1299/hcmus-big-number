#include "QInt.h"
#include "QFloat.h"
#include "UI.h"
#include <fstream>
#include <string>
#include <conio.h>


void solveQInt(string s, ofstream& out)
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


void solveQFloat(string s, ofstream& out)
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
	
	
	if (argc != 4)
	{
		cout << "Erorr!!" << endl;
		return 0;
	}

	ifstream in(argv[1]);
	ofstream out(argv[2]);
	

	if (!in)
	{
		cout << "Not found!" << endl;
		return 0;
	}

	int typeQ = stoi(argv[3]);

	if (typeQ == 1)
	{

		// Đọc file input vào vector
		vector<string> input;
		while (!in.eof())
		{
			char tmp[250];
			in.getline(tmp, 225);
			// Còn 1 chút bug nếu file input có hàng trống.
			input.push_back(tmp);
		}

		for (int i = 0; i < input.size(); i++)
		{
			solveQInt(input[i], out);
		}
	}
	else if (typeQ == 2)
	{
		// Đọc file input vào vector
		vector<string> input;
		while (!in.eof())
		{
			char tmp[300];
			in.getline(tmp, 300);

			input.push_back(tmp);
		}

		for (int i = 0; i < input.size(); i++)
		{
			solveQFloat(input[i], out);
		}
	}
	else
		cout << "Loai khong hop le!!" << endl;




	in.close();
	out.close();
	
	return 0;
	
	showConsoleCursor(false);

	str operationInt[10] = { "BASE 2 ONLY", "BASE 10 ONLY","BASE 16 ONLY", "BASE 2 TO BASE 10", "BASE 2 TO BASE 16", "BASE 10 TO BASE 2", "BASE 10 TO BASE 16", "BASE 16 TO BASE 2", "BASE 16 TO BASE 10", "EXIT"};
	str operationFloat[5] = { "BASE 2 ONLY", "BASE 10 ONLY", "BASE 2 TO BASE 10", "BASE 10 TO BASE 2", "EXIT"};
	//Ẩn nút phóng to
	fixConsoleWindow();
	//Ẩn cursor của màn hình console
	showConsoleCursor(true);

	//Ẩn thanh kéo dọc (Không cho người dùng thay đổi kích thước)
	removeScrollbar();
	
	
	string choice;
	cout << "QInt or QFloat?" << endl;
	cin >> choice;

	while (1)
	{
		char c = toupper(_getch());
		clrscr();
		if (choice == "QInt" || choice == "qint" || choice == "Qint")
		{
			int oper = menu(operationInt, 10);
			c = toupper(_getch());
			menuQInt(oper);
		}
		else if (choice == "QFloat" || choice == "Qfloat" || choice == "qfloat")
		{
			int oper = menu(operationFloat, 5);
			c = toupper(_getch());
			menuQFloat(oper);
		}
	}
	_getch();
	
	return 0;
}