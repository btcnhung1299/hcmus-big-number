#include "QInt.h"
#include "QFloat.h"
#include "UI.h"
#include <fstream>
#include <string>
#include <conio.h>
int main()
{
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
