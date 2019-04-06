#include "QInt.h"
#include "UI.h"
#include <fstream>
#include <string>
#include <conio.h>
int main()
{
	showConsoleCursor(false);

	str operation[10] = { "BASE 2 ONLY", "BASE 10 ONLY","BASE 16 ONLY", "BASE 2 TO BASE 10", "BASE 2 TO BASE 16", "BASE 10 TO BASE 2", "BASE 10 TO BASE 16", "BASE 16 TO BASE 2", "BASE 16 TO BASE 10", "EXIT"};
	//Ẩn nút phóng to
	fixConsoleWindow();
	//Ẩn cursor của màn hình console
	showConsoleCursor(true);

	//Ẩn thanh kéo dọc (Không cho người dùng thay đổi kích thước)
	removeScrollbar();
	
	while (1)
	{
		int oper = menu(operation);
		char c = toupper(_getch());

		menuCalculator(oper);
		char tmp = toupper(_getch());
		clrscr();
	}
	_getch();
	return 0;
}
