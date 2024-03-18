#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define ESC			27 
#define LEFT		75 
#define RIGHT		77 
#define DOWN		80 
#define UP			72 
#define SPACE		32 
#define ENTER		13 

#define KBD_LEFT 	1000
#define KBD_RIGHT	1001
#define KBD_DOWN 	1002
#define KBD_UP	    1003
#define KBD_SPACE	1004
#define KBD_ENTER	1005
#define KBD_CTRL 	1006
#define KBD_ESC  	1007

#define MAX_STR		128
#define MAP_W			31		// ∏ ≈©±‚
#define MAP_H			24
#define START_X			2
#define START_Y			1

#define MAX_BUFFER_W	120
#define MAX_BUFFER_H	100


#define EMPTY			0
#define WALL			1
#define SCORE		    2
#define HINT		    3	
#define TREASURE	    4	
#define RESET_XY		5	
#define RESET_FLAG		6	
#define RESET_SCORE		7	
#define MINE			8
#define MAX_TYPE		9

int  getKeyHit();
void print_strXY(HANDLE out, int x, int y, char* str);
void print_str(HANDLE out, char* str);
void fillCh(HANDLE out, char ch);
void printCh(HANDLE out, char ch);
void setCursorVisible(HANDLE out, DWORD size, int visible);
void gotoxy(HANDLE out, int x, int y);
void printChXY(HANDLE out, int x, int y, char ch);