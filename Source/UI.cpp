#include "UI.h"
#include <iostream>
using namespace std;
void clrscr()
{
	/*POINT p;
	GetCursorPos(&p);

	gotoXY(0, 15);
	cout << "                                                                                                                                 ";
	gotoXY(0, 16);
	cout << "                                                                                                                                 ";
	gotoXY(0, 17);
	cout << "                                                                                                                                 ";
	gotoXY(0, 18);
	cout << "                                                                                                                                 ";
	gotoXY(0, 15);*/

	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0,0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
void fixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void removeScrollbar()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}
void gotoXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
state key(int tmp)
{
	//Thao tac up, down, left, right deu co gia tri kep, 1 kieu int, 1 kieu char
	if (tmp == 224)
	{
		char c;
		c = _getch();

		if (c == 72)
			return UP;
		if (c == 80)
			return DOWN;
		if (c == 77)
			return RIGHT;
		if (c == 75)
			return LEFT;
	}
	else if (tmp == 13)
		return ENTER;
	return ENTER;
}
int menu(str operation[], int n)
{
	int oper = -1; //Chỉ ra đang ở thao tác thứ mấy
	int *color = new int[10];
	for (int i = 0; i < n; i++)
		color[i] = fontColor;


	color[0] = bgColor;
	while (1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fontColor);
		gotoXY(50, 2);
		cout << "BIG NUMBER CALCULATOR" << endl;

		gotoXY(80, 4);
		cout << "----Select option----";

		//In ra man hinh giao dien cua menu
		for (int i = 0; i < n; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color[i]);
			gotoXY(80, i + 5);
			cout << (char)254 << " " << operation[i] << endl;
		}

		int tmp = _getch();
		state s = key(tmp);

		switch (s)
		{
		case UP:
		{
			//Khi nhấn phím mũi tên lên hoài thì nó sẽ quay về thao tác cuối
			if (oper == 0)
				oper = n - 1;
			else
				oper--;
			break;
		}
		case DOWN:
		{
			//Khi nhấn phím mũi tên xuống hoài thì nó sẽ quay về thao tác đầu
			if (oper == n - 1)
				oper = 0;
			else
				oper++;
			break;
		}
		case ENTER:
		{
			return oper;
			break;
		}
		case ESC:
		{
			system("cls");
			break;
		}
		}
		//reset lại màu sau khi chọn thao tác
		for (int i = 0; i < n; i++)
			color[i] = fontColor;
		color[oper] = bgColor;
	}
}

void menuQFloat(int oper)
{
	switch (oper)
	{
	case 0:
	{
		string operation;

		gotoXY(1, 15);
		cout << "Choose operation: ";
		cin >> operation;

		QFloat o1, o2;
		string s1, s2;
		cout << "Input 1st binary: ";
		cin >> s1;
		cout << "Input 2nd binary: ";
		cin >> s2;

		o1.scanBits(s1);
		o2.scanBits(s2);

		QFloat res;
		res = twoOperand1(o1, o2, operation);
		cout << "Result = " << res.printBits() << endl;
		break;
	}
	case 1:
	{
		string operation;

		gotoXY(1, 15);
		cout << "Choose operation: ";
		cin >> operation;

		QFloat o1, o2;
		string s1, s2;
		cout << "Input 1st decimal: ";
		cin >> s1;
		cout << "Input 2nd decimal: ";
		cin >> s2;

		o1.scanQFloat(s1);
		o2.scanQFloat(s2);

		QFloat res;
		res = twoOperand1(o1, o2, operation);
		cout << "Result = " << res << endl;
		break;
	}
	case 2:
	{
		string s;
		QFloat o;
		cout << "Input a binary number: ";
		cin >> s;
		o.scanBits(s);

		cout << "Result = " << o.printQFloat() << endl;
		break;
	}
	case 3:
	{
		string s;
		QFloat o;
		cout << "Input a decimal: ";
		cin >> s;
		o.scanQFloat(s);

		cout << "Result = " << o.printBits() << endl;
		break;
	}
	case 4:
	{
		exit(0);
	}
	}
}
void menuQInt(int oper)
{
	switch (oper)
	{
	case 0:
	{
		string operation;

		gotoXY(1, 15);
		cout << "Choose operation: ";
		cin >> operation;

		int type = typeOperator(operation);//Kiểm tra toán tử thuộc loại nào

		if (type == 1 || type == 2)
		{
			QInt o1, o2;
			string s1, s2;
			cout << "Input 1st binary: ";
			cin >> s1;
			cout << "Input 2nd binary: ";
			cin >> s2;

			o1.scanBits(s1);
			o2.scanBits(s2);

			if (type == 1)
			{
				QInt res;
				res = twoOperand1(o1, o2, operation);
				cout << "Result = " << res.printBits() << endl;
			}
			else
			{
				bool res;
				res = twoOperand2(o1, o2, operation);
				cout << "Result = " << res << endl;
			}

		}
		else if (type == 3)
		{
			QInt o1;
			string s1;
			cout << "Input a binary number: ";
			cin >> s1;

			o1.scanBits(s1);

			int o2;
			cout << "Input a number to shift/rotate: ";
			cin >> o2;

			QInt res;
			res = twoOperand3(o1, o2, operation);

			cout << "Result = " << res.printBits() << endl;
		}
		else if (type == 4)
		{
			QInt o;
			string s1;
			cout << "Input a binary number: ";
			cin >> s1;

			o.scanBits(s1);
			o = ~o;
			cout << "Result = " << o.printBits() << endl;
		}
		break;
	}
	case 1:
	{
		string operation;

		gotoXY(1, 15);
		cout << "Choose operation: ";
		cin >> operation;

		int type = typeOperator(operation);//Kiểm tra toán tử thuộc loại nào

		if (type == 1 || type == 2)
		{
			QInt o1, o2;
			string s1, s2;
			cout << "Input 1st decimal: ";
			cin >> s1;
			cout << "Input 2nd decimal: ";
			cin >> s2;

			o1.scanQInt(s1);
			o2.scanQInt(s2);

			if (type == 1)
			{
				QInt res;
				res = twoOperand1(o1, o2, operation);
				cout << "Result = " << res << endl;
			}
			else
			{
				bool res;
				res = twoOperand2(o1, o2, operation);
				cout << "Result = " << res << endl;
			}
		}
		else if (type == 3)
		{
			QInt o1;
			string s1;
			cout << "Input a decimal: ";
			cin >> s1;
			o1.scanQInt(s1);

			int o2;
			cout << "Input a number to shift/rotate: ";
			cin >> o2;

			QInt res;
			res = twoOperand3(o1, o2, operation);
			cout << "Result = " << res << endl;
		}
		else if (type == 4)
		{
			QInt o;
			string s1;
			cout << "Input a decimal: ";
			cin >> s1;

			o.scanQInt(s1);
			o = ~o;
			cout << "Result = " << o.printQInt() << endl;
		}
		break;
	}
	case 2:
	{
		string operation;

		gotoXY(1, 15);
		cout << "Choose operation: ";
		cin >> operation;
		int type = typeOperator(operation);//Kiểm tra toán tử thuộc loại nào

		if (type == 1 || type == 2)
		{
			QInt o1, o2;
			string s1, s2;
			cout << "Input 1st hexadecimal: ";
			cin >> s1;
			cout << "Input 2nd hexadecimal: ";
			cin >> s2;

			o1.scanHexs(s1);
			o2.scanHexs(s2);

			if (type == 1)
			{
				QInt res;
				res = twoOperand1(o1, o2, operation);
				cout << "Result = " << res.printHexs() << endl;
			}
			else
			{
				bool res;
				res = twoOperand2(o1, o2, operation);
				cout << "Result = " << res << endl;
			}
		}
		else if (type == 3)
		{
			QInt o1;
			string s1;
			cout << "Input a hexadecimal: ";
			cin >> s1;
			o1.scanQInt(s1);

			int o2;
			cout << "Input a number to shift/rotate: ";
			cin >> o2;

			QInt res;
			res = twoOperand3(o1, o2, operation);
			cout << "Result = " << res.printHexs() << endl;
		}
		else if (type == 4)
		{
			QInt o;
			string s1;
			cout << "Input a hexadecimal: ";
			cin >> s1;

			o.scanHexs(s1);
			o = ~o;
			cout << "Result = " << o.printHexs() << endl;
		}
		break;
	}
	case 3:
	{
		string s;
		QInt o;
		cout << "Input a 128-bit binary number: ";
		cin >> s;
		o.scanBits(s);
		cout << "Convert to decimal => " << o.printQInt() << endl;
		break;
	}
	case 4:
	{
		string s;
		QInt o;
		cout << "Input a 128-bit binary number: ";
		cin >> s;
		o.scanBits(s);
		cout << "Convert to hexadecimal => " << o.printHexs() << endl;
		break;
	}
	case 5:
	{
		string s;
		QInt o;
		cout << "Input a decimal: ";
		cin >> s;
		o.scanQInt(s);
		cout << "Convert to binary number => " << o.printBits() << endl;
		break;
	}
	case 6:
	{
		string s;
		QInt o;
		cout << "Input a decimal: ";
		cin >> s;
		o.scanQInt(s);
		cout << "Convert to hexadecimal => " << o.printHexs() << endl;
		break;
	}
	case 7:
	{
		string s;
		QInt o;
		cout << "Input a hexadecimal: ";
		cin >> s;
		o.scanHexs(s);
		cout << "Convert to binary number => " << o.printBits() << endl;
		break;
	}
	case 8:
	{
		string s;
		QInt o;
		cout << "Input a hexadecimal: ";
		cin >> s;
		o.scanHexs(s);
		cout << "Convert to decimal => " << o.printQInt() << endl;
		break;
	}
	case 9:
	{
		exit(0);
	}
	}
}