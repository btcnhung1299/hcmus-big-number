#include "Windows.h"
#include "Functions.h"
#include <conio.h>
#define bgColor 176
#define bgFontColor 23
#define fontColor 15

typedef char str[31];
//Định nghĩa kiểu dữ liệu tương ứng với phím nhập vào
enum state { UP, DOWN, LEFT, RIGHT, ENTER, ESC};
state key(int tmp); //Lấy ký tự do người dùng nhập vào để điều khiển menu
int menu(str operation[], int n); //Tạo menu

void fixConsoleWindow();
void showConsoleCursor(bool showFlag);
void removeScrollbar();
void gotoXY(int column, int line);
void menuQInt(int oper);
void menuQFloat(int oper);
void clrscr();
