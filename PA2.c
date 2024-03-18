#pragma warning(disable:4996)

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "treasure.h"

typedef struct ITEM
{
	int		x;
	int		y;
}ITEM;

typedef struct PLAYER
{
	char* shape;
	int		x;
	int		y;
	int		life;
	int		score;
	int		flag_count;
	int		time;
	int		move_count;
}PLAYER;



char* _player = "��"; 
char* _item[] = { "  ", "��", "��", "��","��","��","��","��", "��" }; // item ���
char* _itemV[] = { "  ", "��", "��", "��","��",":(","��","S.", "��" }; 

ITEM treasure;

void draw_map(HANDLE hOutput, int(*map)[MAP_W], int visible_mode)
{
	int x, y, type;

	for (y = 0; y < MAP_H; y++)
	{
		for (x = 0; x < MAP_W - 1; x++)
		{
			type = map[y][x];
			if (visible_mode)
				print_str(hOutput, _itemV[type]);
			else
				print_str(hOutput, _item[type]);
		}
		printf("\n");
	}

}
void load_map(int(*map)[MAP_W])
{
	FILE* fp;
	int x, y;
	char ch;

	fp = fopen("map.txt", "rt");	
	if (!fp)
	{
		printf("�ʿ��� ����");
		fclose(fp);
		return;
	}

	for (y = 0; y < MAP_H; y++)
	{
		for (x = 0; x < MAP_W; x++)
		{
			ch = getc(fp);
			if (ch != '\n')
			{
				map[y][x] = ch - 48;
			}
		}
	}
	fclose(fp);
}

void place_item(int(*map)[MAP_W], int type)       //������ ��ġ �Լ�
{
	int x, y;
	while (1)
	{
		x = rand() % (MAP_W - 2) + 1; // 1 ~ 29;
		y = rand() % (MAP_H - 1) + 1; // 1 ~ 19;

		if (map[y][x] == EMPTY && x != START_X && y != START_Y)
		{
			map[y][x] = type;
			if (type == TREASURE)
			{
				treasure.x = x;
				treasure.y = y;
			}
			break;
		}
	}
}
int create_item(int level, int(*map)[MAP_W])
{
	int index = 0;
	int total_count = 0;
	int item_count[] = { 0, 0, 4, 1, 1, 1, 1, 1, 8 }; // �ʱ޴ܰ� ������ ����

	for (int type = 2; type < MAX_TYPE; type++)
	{
		if (type != TREASURE)
		{
			switch (level)
			{
			case 2: item_count[type] = item_count[type] * 2; //�߱޴ܰ� ������ ����
				break;
			case 3: item_count[type] = item_count[type] * 3;
				break;
			}
		}
	}

	for (int type = 2; type < MAX_TYPE; type++)
	{
		for (int i = 0; i < item_count[type]; i++)
		{
			place_item(map, type);
			index++;
		}
		total_count += item_count[type];
	}
	return total_count;
}

void MovePlayer(int keyState, PLAYER* p, int(*map)[MAP_W])
{
	int X = 0, Y = 0;		
	int tempX, tempY;			

	switch (keyState)
	{
	case KBD_LEFT:	
		X -= 2; 
		break;
	case KBD_RIGHT: 
		X += 2; 
		break;
	case KBD_DOWN:	
		Y++; 
		break;
	case KBD_UP:	
		Y--; 
		break;
	}
	tempX = (p->x + X) / 2;		
	tempY = p->y + Y;

	if (map[tempY][tempX] != WALL)
	{
		p->x += X;
		p->y += Y;
		p->move_count--;
	}
}
void message(char* msg)
{
	int x = (MAP_W * 2 - (int)strlen(msg)) / 2;
	print_strXY(GetStdHandle(STD_OUTPUT_HANDLE), x, MAP_H / 2, msg);
	getchar(); 
}

int GetMapXY(int(*map)[MAP_W], PLAYER* p)
{
	return map[p->y][p->x / 2];
}

void SetMapXY(int(*map)[MAP_W], PLAYER* p, int type)
{
	map[p->y][p->x / 2] = type;
}

void InitPlayer(PLAYER* p, char* s, int x, int y, int life, int time, int move_count)
{
	p->shape = s;
	p->x = x;
	p->y = y;
	p->life = life;
	p->time = time;
	p->move_count = move_count;
	p->score = 0;
	p->flag_count = 0;
}

void info()
{
	HANDLE hOp = GetStdHandle(STD_OUTPUT_HANDLE);
	fillCh(hOp, ' ');
	print_strXY(hOp, 20, 1, "[�� �� �� �� �� ȸ �� �� �� ã ��]");
	print_strXY(hOp, 7, 3,  "�̻��� ������ �ٸ����� �ʾ��ٰ� ������� �پ�� �Ͼ�");
	print_strXY(hOp, 7, 5,  "�䳢�� ���� �̻��� ����� ���� �ȴ�. �̻��� ���󿡼���");
	print_strXY(hOp, 7, 7,  "�ű��� �ϵ��� �Ͼ��, ��Ʈ�� ���տ��Լ� ������ ����");
	print_strXY(hOp, 7, 9,  "�ٸ����� ������ ����� ���ư��� ���� �Ͼ� �䳢�� ã����");
	print_strXY(hOp, 7, 11,  "����. ������ �� �Ͼ� �䳢�� �ڽ��� ȸ�߽ð踦 �Ҿ���ȴٸ�");
	print_strXY(hOp, 7, 13,  "ȸ�߽ð踦 ã�ƾ� ���ư� �� �ִٰ� ���Ѵ�. �ٸ����� ȥ��");
	print_strXY(hOp, 7, 15, "ã�⿡ ������� ������ ��� Ʈ���� ī�� ����� �� 3���� Ŭ�ι�");
	print_strXY(hOp, 7, 17, "����鿡�� ȸ�߽ð踦 ã�� �޶�� ��Ź�� �Ѵ�. ȸ�� �ð��");
	print_strXY(hOp, 7, 19, "�������ڿ� ����ְ� �������ڴ� ��� �Ʒ��� �������ִ�.");
	print_strXY(hOp, 7, 21, "����� �������� �ְ� �� �� �ϳ����� ������ ������ ������");
	print_strXY(hOp, 7, 23, "������ ����� ��� ����� ���� �𸥴�. ���� �濡 ���ڸ�");
	print_strXY(hOp, 7, 25, "�����ϸ� ȸ�߽ð踦 ã���� ������. ���ڸ� ���� ������");
	print_strXY(hOp, 7, 25, "Ŭ�ι� ������ �Ѹ� ����ϰ� ���� ������ ���ο� ������ ��Ÿ����");
	print_strXY(hOp, 2, 27, "(Tip.���� �濡 �����̽��ٸ� ������ �Ȱ��� ���� �Ǵµ� �̴� ��ߵ��� � ����� �ϴ� �� �˷��ش�.)");
	getchar();
}


int menu()
{
	int i = 0, key;
	int y[] = { 6, 8, 10, 12, 14 };
	BOOL quit = FALSE;
	HANDLE hOp = GetStdHandle(STD_OUTPUT_HANDLE);

	do
	{
		do
		{
			fillCh(hOp, ' ');
			print_strXY(hOp, 29, 2, "[�Ͼ� �䳢�� ȸ�߽ð� ã��]");
			print_strXY(hOp, 34, 6, "1.�ʱ� ����");
			print_strXY(hOp, 34, 8, "2.�߱� ����");
			print_strXY(hOp, 34, 10, "3.��� ����");
			print_strXY(hOp, 34, 12, "4.���� ����");
			print_strXY(hOp, 34, 14, "5.���� ����");

			print_strXY(hOp, 31, y[i], "��");

			key = getKeyHit();
			if (key)
			{
				switch (key)
				{
				case KBD_UP:	i--; break;
				case KBD_DOWN:	i++; break;
				case KBD_ENTER: quit = TRUE; break;
				}

				if (i < 0)		i = 4;
				else if (i > 4)	i = 0;
			}
			Sleep(50);
		} while (!quit);

		if (i == 3)
		{
			info();
			quit = FALSE;
		}
		else if (i == 4) 
			exit(0);

	} while (!quit);

	return i + 1;
}

int main()
{
	int key, visible_mode, quit;
	int item_count = 0, type, level;
	char info[MAX_STR];
	int map[MAP_H][MAP_W];
	PLAYER player = { _player, START_X, START_Y, 3, 0, 0, 0, 0 };
	int move_count[] = { 300, 380, 450 };	
	int level_time[] = { 50, 70, 80 };		
	clock_t startClock, curClock;		
	int Timer = 0, remain_time;

	srand((unsigned int)time(0));
	HANDLE hOp;
	hOp = GetStdHandle(STD_OUTPUT_HANDLE);
	// Ŀ�� ����
	setCursorVisible(hOp, 1, 0);

	while (1)
	{
		level = menu();
		InitPlayer(&player, _player, START_X, START_Y, 3, level_time[level - 1], move_count[level - 1]);
		quit = FALSE;
		visible_mode = FALSE;

		load_map(map);

		create_item(level, map);        //ȭ�鿡 ������ ����

		startClock = clock();
		while (!quit)
		{
			key = getKeyHit();
			if (key == KBD_ESC) break;
			if (key == KBD_SPACE) visible_mode = !visible_mode;
			if (key) MovePlayer(key, &player, map);

			type = GetMapXY(map, &player);
			switch (type)
			{
			case SCORE:
				player.score++;
				player.flag_count++;
				SetMapXY(map, &player, EMPTY);
				break;
			case HINT:
				if (treasure.y > MAP_H / 2)
					message("[ ȸ�߽ð� ��ġ : ���� �ϴ� ] ");
				else
					message("[ ȸ�߽ð� ��ġ : ���� ��� ] ");
				player.flag_count++;
				SetMapXY(map, &player, EMPTY);
				break;
			case TREASURE:
				player.flag_count++;				
				message("[ ȸ�� �ð踦 �߰��߽��ϴ� ^^ ]");
				quit = TRUE;
				break;
			case RESET_XY:
				message("[ ó�� ��ġ�� �Ф� ] ");
				player.flag_count++;
				player.x = START_X; player.y = START_Y;
				SetMapXY(map, &player, EMPTY);
				break;
			case RESET_FLAG:
				message("[ ��� ���ġ ] ");
				player.x = 2; player.y = 1;
				player.flag_count++;
				load_map(map);
				create_item(level, map);
				break;
			case RESET_SCORE:
				message("[ ���� �ʱ�ȭ ] ");
				player.score = 0;
				player.flag_count++;
				SetMapXY(map, &player, EMPTY);
				break;
			case MINE:
				message("[ ���ڸ� ��ҽ��ϴ� ( ��� - 1 ) ]");
				player.life--;
				SetMapXY(map, &player, EMPTY);
				break;
			}

			curClock = clock();
			Timer = (curClock - startClock) / CLOCKS_PER_SEC; 
			remain_time = player.time - Timer;

			gotoxy(hOp, 0, 0);
			draw_map(hOp, map, visible_mode);
			print_strXY(hOp, player.x, player.y, player.shape);

			sprintf(info, "�� �� �� �� : %d", player.life);			
			print_strXY(hOp, 62, 9, info);
			sprintf(info, "��       �� : %d", player.score);		
			print_strXY(hOp, 62, 10, info);
			sprintf(info, "�� �� �� �� : %-3d", remain_time);		
			print_strXY(hOp, 62, 11, info);
			sprintf(info, "ȹ���� ��� : %d", player.flag_count);	
			print_strXY(hOp, 62, 12, info);
			sprintf(info, "�� �� Ƚ �� : %-3d", player.move_count);	
			print_strXY(hOp, 62, 13, info);

			if (player.life <= 0)
			{
				message("����� ��� ����Ͽ����ϴ�.");
				quit = TRUE;
			}
			else if (remain_time <= 0)
			{
				message("���� �ð��� �� �Ǿ����ϴ�.");
				quit = TRUE;
			}
			else if (player.move_count <= 0)
			{
				message("���̻� ������ �� �����ϴ�.");
				quit = TRUE;
			}
			Sleep(50);
		}
		Sleep(500);
	}

	return 0;
}