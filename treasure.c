#include "treasure.h"

void gotoxy(HANDLE out, int x, int y)
{
	COORD XY = { x, y };
	SetConsoleCursorPosition(out, XY);
}

void printCh(HANDLE out, char ch)  // �������
{
	DWORD num;
	WriteFile(out, &ch, 1, &num, NULL);
}

void printChXY(HANDLE out, int x, int y, char ch)  // ��ǥ�� �������
{
	gotoxy(out,x, y);
	printCh(out, ch);
}

void fillCh(HANDLE out, char ch)
{
	COORD XY = { 0, 0 };
	DWORD num, size = MAX_BUFFER_W * MAX_BUFFER_H;

	FillConsoleOutputCharacter(out, ch, size, XY, &num);
	SetConsoleCursorPosition(out, XY);
}

void setCursorVisible(HANDLE out, DWORD size, int visible)
{
	CONSOLE_CURSOR_INFO curInfo = { size, visible };
	SetConsoleCursorInfo(out, &curInfo);
}

void print_str(HANDLE out, char* str)  // ���ڿ� ���
{
	DWORD num;
	WriteFile(out, str, strlen(str), &num, NULL);
}

void print_strXY(HANDLE out, int x, int y, char* str) // ��ǥ�� ���ڿ� ���
{
	gotoxy(out, x, y);
	print_str(out, str);
}

int getKeyHit()
{
	int key = 0;

	if (kbhit())
	{
		switch (getch())
		{
		case LEFT:  key = KBD_LEFT; 
			break;
		case RIGHT: key = KBD_RIGHT; 
			break;
		case DOWN: 	key = KBD_DOWN; 
			break;
		case UP:	key = KBD_UP;	
			break;
		case ENTER: key = KBD_ENTER; 
			break;
		case SPACE: key = KBD_SPACE; 
			break;
		case ESC:   key = KBD_ESC; 
			break;
		}
	}

	return key;
}
