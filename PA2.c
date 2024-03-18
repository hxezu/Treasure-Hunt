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



char* _player = "♣"; 
char* _item[] = { "  ", "■", "▶", "▶","▶","▶","▶","▶", "ⓜ" }; // item 모양
char* _itemV[] = { "  ", "■", "▶", "Η","★",":(","◁","S.", "ⓜ" }; 

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
		printf("맵열기 실패");
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

void place_item(int(*map)[MAP_W], int type)       //아이템 배치 함수
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
	int item_count[] = { 0, 0, 4, 1, 1, 1, 1, 1, 8 }; // 초급단계 아이템 개수

	for (int type = 2; type < MAX_TYPE; type++)
	{
		if (type != TREASURE)
		{
			switch (level)
			{
			case 2: item_count[type] = item_count[type] * 2; //중급단계 아이템 개수
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
	print_strXY(hOp, 20, 1, "[하 얀 토 끼 의 회 중 시 계 찾 기]");
	print_strXY(hOp, 7, 3,  "이상한 나라의 앨리스가 늦었다고 허둥지둥 뛰어가는 하얀");
	print_strXY(hOp, 7, 5,  "토끼를 따라 이상한 나라로 들어가게 된다. 이상한 나라에서는");
	print_strXY(hOp, 7, 7,  "신기한 일들이 일어났고, 하트의 여왕에게서 재판이 끝난");
	print_strXY(hOp, 7, 9,  "앨리스는 원래의 세계로 돌아가기 위해 하얀 토끼를 찾으러");
	print_strXY(hOp, 7, 11,  "간다. 만나게 된 하얀 토끼는 자신의 회중시계를 잃어버렸다며");
	print_strXY(hOp, 7, 13,  "회중시계를 찾아야 돌아갈 수 있다고 말한다. 앨리스는 혼자");
	print_strXY(hOp, 7, 15, "찾기에 무리라고 생각이 들어 트럼프 카드 병장들 중 3명의 클로버");
	print_strXY(hOp, 7, 17, "병장들에게 회중시계를 찾아 달라고 부탁을 한다. 회중 시계는");
	print_strXY(hOp, 7, 19, "보물상자에 들어있고 보물상자는 깃발 아래에 숨겨져있다.");
	print_strXY(hOp, 7, 21, "깃발은 여러개가 있고 그 중 하나에만 보물이 숨겨져 있으며");
	print_strXY(hOp, 7, 23, "나머지 깃발은 어떠한 기능을 할지 모른다. 가는 길에 지뢰를");
	print_strXY(hOp, 7, 25, "조심하며 회중시계를 찾으러 떠난다. 지뢰를 밟을 때마다");
	print_strXY(hOp, 7, 25, "클로버 병장은 한명씩 사망하고 죽을 때마다 새로운 병장이 나타난다");
	print_strXY(hOp, 2, 27, "(Tip.가는 길에 스페이스바를 누르면 안경을 쓰게 되는데 이는 깃발들이 어떤 기능을 하는 지 알려준다.)");
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
			print_strXY(hOp, 29, 2, "[하얀 토끼의 회중시계 찾기]");
			print_strXY(hOp, 34, 6, "1.초급 시작");
			print_strXY(hOp, 34, 8, "2.중급 시작");
			print_strXY(hOp, 34, 10, "3.고급 시작");
			print_strXY(hOp, 34, 12, "4.게임 정보");
			print_strXY(hOp, 34, 14, "5.게임 종료");

			print_strXY(hOp, 31, y[i], "☞");

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
	// 커서 숨김
	setCursorVisible(hOp, 1, 0);

	while (1)
	{
		level = menu();
		InitPlayer(&player, _player, START_X, START_Y, 3, level_time[level - 1], move_count[level - 1]);
		quit = FALSE;
		visible_mode = FALSE;

		load_map(map);

		create_item(level, map);        //화면에 아이템 생성

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
					message("[ 회중시계 위치 : 맵의 하단 ] ");
				else
					message("[ 회중시계 위치 : 맵의 상단 ] ");
				player.flag_count++;
				SetMapXY(map, &player, EMPTY);
				break;
			case TREASURE:
				player.flag_count++;				
				message("[ 회중 시계를 발견했습니다 ^^ ]");
				quit = TRUE;
				break;
			case RESET_XY:
				message("[ 처음 위치로 ㅠㅠ ] ");
				player.flag_count++;
				player.x = START_X; player.y = START_Y;
				SetMapXY(map, &player, EMPTY);
				break;
			case RESET_FLAG:
				message("[ 깃발 재배치 ] ");
				player.x = 2; player.y = 1;
				player.flag_count++;
				load_map(map);
				create_item(level, map);
				break;
			case RESET_SCORE:
				message("[ 점수 초기화 ] ");
				player.score = 0;
				player.flag_count++;
				SetMapXY(map, &player, EMPTY);
				break;
			case MINE:
				message("[ 지뢰를 밟았습니다 ( 목숨 - 1 ) ]");
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

			sprintf(info, "남 은 목 숨 : %d", player.life);			
			print_strXY(hOp, 62, 9, info);
			sprintf(info, "점       수 : %d", player.score);		
			print_strXY(hOp, 62, 10, info);
			sprintf(info, "남 은 시 간 : %-3d", remain_time);		
			print_strXY(hOp, 62, 11, info);
			sprintf(info, "획득한 깃발 : %d", player.flag_count);	
			print_strXY(hOp, 62, 12, info);
			sprintf(info, "이 동 횟 수 : %-3d", player.move_count);	
			print_strXY(hOp, 62, 13, info);

			if (player.life <= 0)
			{
				message("목숨을 모두 사용하였습니다.");
				quit = TRUE;
			}
			else if (remain_time <= 0)
			{
				message("제한 시간이 다 되었습니다.");
				quit = TRUE;
			}
			else if (player.move_count <= 0)
			{
				message("더이상 움직일 수 없습니다.");
				quit = TRUE;
			}
			Sleep(50);
		}
		Sleep(500);
	}

	return 0;
}