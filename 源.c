#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<Windows.h>
#include<time.h>											
#define HIGH 23    //背景高
#define WIDE   31    //背景宽
#define LEFT  9     //游戏区左边界
#define RIGHT 21    //游戏区右边界
int g_score;
int g_speed=40;// 速度
int g_time;
bool g_flag=true;
bool g_pause; // 游戏状态标志

char g_background[HIGH][WIDE];// 背景数组
// 方块状态
char g_NextSquare = 0;
char g_PayingSquare = 0; // 移动中的方块

// 方块的位置(红块位置)
char Square_row;//行
char Square_line;//列
char g_State = 0;//旋转角度

void SetCursorPosition(int row,int line);// 让控制台光标移动到第row行，第line列,从零行零列开始
void meanHead(void);// 头部信息
void Frame(void);// 界面框架
void LeftInfo(void);// 左侧提示
void RightInfo(void);// 右侧提示

void  FirstSqure(void); // 开始时的方块种类
void  NextSqure(void);// 下一个的方块种类
void  InitSqure(void);// 初始化

void DrewNextSqure(void);
void DrewplayingSqure(char* sq);//"□" "■" "  "
void Fall(void);
void Landing(char row);// 整体降落一行
void KeyState(void);
void Left_(void);
void Right_(void);
void Change(void);
bool CanMove(char State, char row, char line);//移动检测

void  StopSqure();
void Initgame(void);
bool CanChange(char state);
void GetRow(char *low,char *deeper);// 获取要判断的行
void Score();
bool IsFull(char row);
void ClearSquqre(char row);
void DrewScore();
bool Isdead();
void ClearRow(char row);

void QuitGame(void);

int main() {

	Initgame();

	while (1) 
	{
	
		if (g_time == 21474832147483007)
			g_time = 0;
		Sleep(20);
		g_time++;
		if (g_time % 2 == 0)
		{
			KeyState();
		}
		if (g_flag == false)
		{
			break;
		}
		if (g_pause != true)
		{
			if (g_time % g_speed  == 0)
			{
				Fall();
			}
		}
		


	}

	system("pause>0");
	return 0;
}
void SetCursorPosition(int row, int line) {
	COORD cd = { line,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
void meanHead(void) {
	SetCursorPosition(0, 0);
	printf("\t\t\t\t\t\t**************************\n");
	printf("\t\t\t\t\t\t*                        *\n");
	printf("\t\t\t\t\t\t*       俄罗斯方块       *\n");
	printf("\t\t\t\t\t\t*                        *\n");
	printf("\t\t\t\t\t\t**************************\n");
}
void Frame(void) {
	
	int i ;
	int j ;
	for (i = 0; i < HIGH; i++) 
	{
		SetCursorPosition(5 + i, 30);
		for (j = 0; j < WIDE; j++) 
		{
			if (i == 0 || i == HIGH - 1 || j == 0 || j == WIDE -1|| j == LEFT ||j==RIGHT)
			{
				g_background[i][j] = 1;
				printf("※");
			}
			else
			{
				g_background[i][j] = 0;
				printf("  ");
			}
				
				
		}
	}
}
void LeftInfo(void)
{
	int i = 5, j = 32;
	SetCursorPosition(i += 2, j);
	printf("    操作提示\n");
	SetCursorPosition(i += 2, j);
	printf(" ← → 左右移动\n");
	SetCursorPosition(i += 2, j);
	printf(" ↑   变形\n");
	SetCursorPosition(i += 2, j);
	printf(" ↓   加速\n");
	SetCursorPosition(i += 2, j);
	printf("Enter 启动/暂停\n");
	SetCursorPosition(i += 2, j);
	printf(" Tab  切换速度\n");
	SetCursorPosition(i += 1, j + 8);
	printf("%d", g_speed);
	SetCursorPosition(i += 2, j);
	printf(" Esc  退出游戏\n");
	SetCursorPosition(i += 2, j);
	printf(" F1   新游戏\n");
}
void RightInfo(void)
{
	int i = 5, j = 76;
	SetCursorPosition(i += 2, j);
	printf("   <score>\n");
	SetCursorPosition(i += 1, j);
	printf("      0\n");
	SetCursorPosition(i += 2, j);
	printf("   <next>\n");
}

/// 以  0到6表示方块的不同形状,0-3表示不同的旋转度数

void FirstSqure() {
	g_PayingSquare = rand()%7;
    g_NextSquare = rand()%7;
	InitSqure();
	DrewplayingSqure("□");
}
void  NextSqure()
{
	
	g_PayingSquare = g_NextSquare;
	g_NextSquare = rand() % 7;
	InitSqure();
	DrewplayingSqure("□");
}
void  InitSqure(void) {
	if (g_PayingSquare == 6  )
	{
		Square_row = 1;
		Square_line = LEFT + 7;
	}else
	{
	
		Square_row = 2;
		Square_line = LEFT + 6;
	}
	g_State = 0;
	
}
void DrewNextSqure(void) {
	int row = 12, line = 80;
	//清空
	SetCursorPosition(12, line-2);
	printf("          ");
	SetCursorPosition(13, line);
	printf("          ");

	switch (g_NextSquare)
	{
	case 0:
		SetCursorPosition(row, line);
		printf("■");
		SetCursorPosition(row+1, line);
		printf("■■■");
		break;
	case 1:
		SetCursorPosition(row, line+4);
		printf("■");
		SetCursorPosition(row + 1, line);
		printf("■■■");
		break;
	case 2:
		SetCursorPosition(row, line );
		printf("■■");
		SetCursorPosition(row + 1, line+2);
		printf("■■");
		break;
	case 3:
		SetCursorPosition(row, line+2);
		printf("■■");
		SetCursorPosition(row + 1, line );
		printf("■■");
		break;
	case 4:
		SetCursorPosition(row, line + 2);
		printf("■");
		SetCursorPosition(row + 1, line);
		printf("■■■");
		break;
	case 5:
		SetCursorPosition(row, line );
		printf("■■");
		SetCursorPosition(row + 1, line);
		printf("■■");
		break;
	case 6:
		SetCursorPosition(row, line-2);
		printf("■■■■");
		break;

	}
}
void DrewplayingSqure(char *sq) 
{
	int row = 5, line = 30;
	switch (g_PayingSquare)
	{
	case 0:
		switch (g_State) 
		{
		case 0:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
				printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
				printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
				printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line-1) * 2 + line);
				printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row -1 + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, Square_line * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line + 1) * 2 + line);
			printf(sq);

			break;
		case 3:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1+ row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + 1 + row, (Square_line-1) * 2 + line);
			printf(sq);
			break;
		}
		break;
	case 1:
		switch (g_State)
		{
		case 0:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line +1) * 2 + line);
			printf(sq);
			break;
		case 1:	
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
		    printf(sq);
			SetCursorPosition(Square_row+1 + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row +1 + row, (Square_line+1) * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line - 1) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition((Square_row-1) + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition((Square_row - 1) + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + 1+row, Square_line * 2 + line);
			printf(sq);
			break;
	    }
		
		break;
	case 2:
		switch (g_State)
		{

		case 0:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row -1+ row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line -1)* 2 + line);
			printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row +1 +row, Square_line * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line - 1) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, Square_line * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + 1 + row, Square_line * 2 + line);
			printf(sq);
			break;

		}
		break;
	case 3:
		switch (g_State)
		{
		case 0:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line+1) * 2 + line);
			printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line + 1) * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line + 1) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + 1 + row, (Square_line + 1) * 2 + line);
			printf(sq);
			break;
			
		}
		break;
	case 4:
		switch (g_State)
		{
		case 0:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line) * 2 + line);
			printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line) * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line +1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line ) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row-1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line) * 2 + line);
			printf(sq);
			break;

		}
		break;
	case 5:
		switch (g_State)
		{
		case 0:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row -1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line-1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line-1) * 2 + line);
			printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			break;

		}
		break;
	case 6:
		switch (g_State)
		{
		case 0:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line+1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 2) * 2 + line);
			printf(sq);
			break;
		case 1:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row+1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 2 + row, (Square_line) * 2 + line);
			printf(sq);
			break;
		case 2:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line + 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 1) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + row, (Square_line - 2) * 2 + line);
			printf(sq);
			break;
		case 3:
			SetCursorPosition(Square_row + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row + 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 1 + row, (Square_line) * 2 + line);
			printf(sq);
			SetCursorPosition(Square_row - 2 + row, (Square_line) * 2 + line);
			printf(sq);
			break;
		}
		break;
	
	}
}
void Fall(void) {
	if (false == CanMove(g_State, Square_row + 1, Square_line))
	{
		DrewplayingSqure("■");
		StopSqure();
		Score();
		DrewScore();
		Isdead();


		//下一个方块
		NextSqure();
		DrewNextSqure();
		
		return;
	}
	
	DrewplayingSqure("  ");
	Square_row++;
	DrewplayingSqure("□");
}
void Left_(void) {

	DrewplayingSqure("  ");
	Square_line--;
	DrewplayingSqure("□");
}
void Right_(void) {
	DrewplayingSqure("  ");
	Square_line++;
	DrewplayingSqure("□");
}
void Change(void){

	DrewplayingSqure("  ");
	g_State++;
	g_State %= 4;
	DrewplayingSqure("□");
}

void KeyState(void) {
	
	if (g_pause != true)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			if (true == CanMove(g_State, Square_row, Square_line - 1))
				Left_();
		}
		else
			if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
			{
				if (true == CanMove(g_State, Square_row, Square_line + 1))
					Right_();
			}
			else
				if (GetAsyncKeyState(VK_UP) & 0x0001)
				{
					if (CanChange((g_State + 1) % 4) == true)
						Change();
				}
				else
					if (GetAsyncKeyState(VK_DOWN) & 0x0001)
					{
						Fall();
					}

	}
	

	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		g_pause = !g_pause;
	}
	if (GetAsyncKeyState(VK_F1) & 0x0001)
	{
		Initgame();
	}
	if (GetAsyncKeyState(VK_TAB) & 0x0001)
	{
		g_speed -= 10;
		if (g_speed <= 5)
			g_speed = 40;
		SetCursorPosition(18, 40);
		printf("%4d", 100-g_speed);

	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		QuitGame();
	}
	
}
bool CanMove(char State,char row,char line) {

	switch (g_PayingSquare)
	{
	case 0:
		switch (State)////0
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line - 1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 1][line + 1] == 1)
				return false;
			if (g_background[row +1][line] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row + 1][line + 1] == 1)
				return false;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row +1][line - 1] == 1)//
				return false;
			break;

		}
		break;
	case 1:
		switch (State)////1
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line + 1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row + 1][line + 1] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row + 1][line - 1] == 1)
				return false;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 1][line - 1] == 1)
				return false;
			break;

		}
		break;
	case 2:
		switch (State)////2
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 1][line - 1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line + 1] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 1][line - 1] == 1)
				return false;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line + 1] == 1)
				return false;
			break;

		}
		break;
	case 3:
		switch (State)////3
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row-1] [line] == 1)
				return false;
			if (g_background[row - 1][line +1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row ][line + 1] == 1)
				return false;
			if (g_background[row + 1][line + 1] == 1)
				return false;
			if (g_background[row - 1][line ] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line -1] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 1][line+1] == 1)
				return false;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row + 1][line + 1] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			break;

		}

		break;
	case 4:
		switch (State)////4
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row - 1][line + 1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row ][line+1] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row + 1][line ] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)//
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row + 1][line ] == 1)
				return false;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			if (g_background[row - 1][line ] == 1)
				return false;
			if (g_background[row ][line -1] == 1)
				return false;
			break;

		}
		break;
	case 5:
		if (g_background[row][line] == 1)
			return false;
		if (g_background[row][line - 1] == 1)
			return false;
		if (g_background[row-1][line ] == 1)
			return false;
		if (g_background[row - 1][line -1] == 1)
			return false;
		break;
	case 6:
		switch (State)////6
		{
		case 0:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line - 2] == 1)
				return false;
			if (g_background[row ][line + 1] == 1)
				return false;
			break;
		case 1:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row-1][line ] == 1)
				return false;
			if (g_background[row-2][line] == 1)
				return false;
			if (g_background[row+1 ][line] == 1)
				return false;
			break;
		case 2:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row][line - 1] == 1)
				return false;
			if (g_background[row][line - 2] == 1)
				return false;
			if (g_background[row][line + 1] == 1)
				return false;
			break;
			break;
		case 3:
			if (g_background[row][line] == 1)
				return false;
			if (g_background[row - 1][line] == 1)
				return false;
			if (g_background[row - 2][line] == 1)
				return false;
			if (g_background[row + 1][line] == 1)
				return false;
			break;

		}
		break;

	}

	return true;
}

void  StopSqure() {

	switch (g_PayingSquare)
	{
	case 0:
		switch (g_State)////0
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
			g_background[Square_row][Square_line - 1] = 1;
			g_background[Square_row][Square_line + 1] = 1;
			g_background[Square_row - 1][Square_line - 1] = 1;	
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;

			g_background[Square_row - 1][Square_line] = 1;

			g_background[Square_row - 1][Square_line + 1] = 1;

			g_background[Square_row + 1][Square_line] = 1;

			break;
		case 2:
			g_background[Square_row][Square_line] = 1;

			g_background[Square_row][Square_line - 1] = 1;

			g_background[Square_row][Square_line + 1] = 1;

			g_background[Square_row + 1][Square_line + 1] = 1;
	
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;
	
			g_background[Square_row + 1][Square_line] = 1;

			g_background[Square_row - 1][Square_line] = 1;
	
			g_background[Square_row +1][Square_line - 1] = 1;

			break;

		}
		break;
	case 1:
		switch (g_State)////1
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row][Square_line - 1] = 1;
			
			g_background[Square_row][Square_line + 1] = 1;

			g_background[Square_row - 1][Square_line + 1] = 1;
		
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row - 1][Square_line] = 1;
		
			g_background[Square_row + 1][Square_line] = 1;
				
			g_background[Square_row + 1][Square_line + 1] = 1;
				
			break;
		case 2:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row][Square_line - 1] = 1;
				
			g_background[Square_row][Square_line + 1] = 1;
				
			g_background[Square_row + 1][Square_line - 1] = 1;
				
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row + 1][Square_line] = 1;
			
			g_background[Square_row - 1][Square_line] = 1;
				
			g_background[Square_row -1][Square_line - 1] = 1;//
			
			break;

		}
		break;
	case 2:
		switch (g_State)////2
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row][Square_line + 1] = 1;
		
			g_background[Square_row - 1][Square_line] = 1;
		
			g_background[Square_row - 1][Square_line - 1] = 1;
				
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row + 1][Square_line] = 1;

			g_background[Square_row][Square_line + 1] = 1;
			
			g_background[Square_row - 1][Square_line + 1] =1;
			
			break;
		case 2:
				g_background[Square_row][Square_line] = 1;
	
				g_background[Square_row][Square_line + 1] = 1;

				g_background[Square_row - 1][Square_line] = 1;
	
				g_background[Square_row - 1][Square_line - 1] = 1;
			
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row + 1][Square_line] = 1;
			
			g_background[Square_row][Square_line + 1] = 1;
				
			g_background[Square_row - 1][Square_line + 1] = 1;
			
			break;

		}
		break;
	case 3:
		switch (g_State)////3
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row][Square_line - 1] = 1;
			
			g_background[Square_row - 1][Square_line] = 1;
				
			g_background[Square_row - 1][Square_line + 1] = 1;
				
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;
		
			g_background[Square_row][Square_line + 1] = 1;
				
			g_background[Square_row + 1][Square_line + 1] = 1;
				
			g_background[Square_row - 1][Square_line] = 1;
				
			break;
		case 2:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row][Square_line - 1] = 1;
			
			g_background[Square_row - 1][Square_line] = 1;
				
			g_background[Square_row - 1][Square_line + 1] = 1;
				
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row][Square_line + 1] = 1;
			
			g_background[Square_row + 1][Square_line + 1] = 1;
				
			g_background[Square_row - 1][Square_line] = 1;
				
			break;

		}

		break;
	case 4:
		switch (g_State)////4
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
			
			g_background[Square_row][Square_line-1 ] = 1;
				
			g_background[Square_row][Square_line + 1] = 1;
			
			g_background[Square_row - 1][Square_line ] = 1;//
			
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;
		
			g_background[Square_row][Square_line + 1] = 1;
				
			g_background[Square_row-1][Square_line ] = 1;//
				
			g_background[Square_row + 1][Square_line] = 1;
				
			break;
		case 2:
			g_background[Square_row][Square_line] = 1;
		
			g_background[Square_row + 1][Square_line] = 1;
				
			g_background[Square_row ][Square_line-1] = 1;//
			
			g_background[Square_row][Square_line + 1] = 1;
				
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;

			g_background[Square_row + 1][Square_line] = 1;

			g_background[Square_row - 1][Square_line] = 1;
				
			g_background[Square_row][Square_line - 1] = 1;
			
			break;

		}
		break;
	case 5:
		g_background[Square_row][Square_line] = 1;
		
		g_background[Square_row][Square_line - 1] = 1;

		g_background[Square_row - 1][Square_line] = 1;
			
		g_background[Square_row - 1][Square_line - 1] = 1;
			
		break;
	case 6:
		switch (g_State)////6
		{
		case 0:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row][Square_line - 1] = 1;
				
			g_background[Square_row][Square_line - 2] = 1;
				
			g_background[Square_row][Square_line + 1] = 1;
				
			break;
		case 1:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row - 1][Square_line] = 1;
				
			g_background[Square_row - 2][Square_line] = 1;
			
			g_background[Square_row + 1][Square_line] = 1;
				
			break;
		case 2:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row][Square_line - 1] = 1;
				
			g_background[Square_row][Square_line - 2] = 1;
				
			g_background[Square_row][Square_line + 1] = 1;
				
		
			break;
		case 3:
			g_background[Square_row][Square_line] = 1;
				
			g_background[Square_row - 1][Square_line] = 1;
			
			g_background[Square_row - 2][Square_line] = 1;
			
			g_background[Square_row + 1][Square_line] = 1;
				
			break;

		}
		break;

	}

}
void Initgame(void) {
	srand((unsigned)time(NULL));

	meanHead();
	Frame();
	LeftInfo();
	RightInfo();
	FirstSqure();
	DrewNextSqure();
	g_pause = true;
}

bool CanChange(char state) {

	if (CanMove(state, Square_row, Square_line) == true)
	{
		return true;
	}
	if (CanMove(state, Square_row, Square_line - 1) == true)
	{
		DrewplayingSqure("  ");
		Square_line -= 1;
		return true;
	}
	if (CanMove(state, Square_row, Square_line + 1) == true)
	{
		DrewplayingSqure("  ");
		Square_line += 1;
		return true;
	}
	if (CanMove(state, Square_row+1, Square_line ) == true)
	{
		DrewplayingSqure("  ");
		Square_row += 1;
		return true;
	}
	if (6 == g_PayingSquare && (g_State == 1 || g_State == 3)) 
	{
		if (CanMove(state, Square_row, Square_line + 2) == true)
		{
			DrewplayingSqure("  ");
			Square_line += 2;
			return true;
		}
		if (CanMove(state, Square_row, Square_line - 2) == true)
		{
			DrewplayingSqure("  ");
			Square_line -= 2;
			return true;
		}
	}
	return false;

}

void GetRow(char* low, char* deeper) {


	switch (g_PayingSquare)
	{
	case 0:
	case 1:
	case 4:
		switch (g_State) 
		{
		case 0:
		case 2:
			*low = Square_row - 1;
			*deeper = Square_row ;
			break;
		case 1:
		case 3:
			*low = Square_row - 1;
			*deeper = Square_row+1;
			break;
		}
		break;
	case 2:
	case 3:
		switch (g_State)
		{
		case 0:
		case 2:
			*low = Square_row - 1;
			*deeper = Square_row;
			break;
		case 1:
		case 3:
			*low = Square_row - 1;
			*deeper = Square_row + 1;
			break;
		}

		break;
	case 5:
		*low = Square_row - 1;
		*deeper = Square_row;
		break;
	case 6:
		switch (g_State)
		{
		case 0:
		case 2:
			*low = Square_row ;
			*deeper = Square_row;
			break;
		case 1:
		case 3:
			*low = Square_row - 2;
			*deeper = Square_row + 1;
			break;
		}
		
		break;

	}

}

void Score() {

	char low,  deeper;

	GetRow(&low, &deeper);
	for (char i = low; i <= deeper; i++)
	{
		if (IsFull(i) == true)
		{
			ClearSquqre(i);
			Landing(i);
			g_score++;
		}
	}
}
bool IsFull(char row) {

	int count=0;
	for (char i = LEFT + 1; i < RIGHT; i++)
	{
		count=g_background[row][i] + count;
	}
	if (11 == count)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ClearSquqre(char row) {

	char r = 5, l = 30;
	for (char i = LEFT+1; i < RIGHT; i++)
	{
		if (g_background[row ][i] == 1)
		{
			g_background[row ][ i] = 0;
			SetCursorPosition(row + r, l + i*2);
			Sleep(50);
			printf("  ");
		}
	}

}

void Landing(char row) {

	char r = 5, l = 30;
	for (char i = row; i > 2; i--)
	{
		for (char j = LEFT+1; j < RIGHT; j++) 
		{
			if (g_background[i-1][j] == 1)
			{
				g_background[i][j] = g_background[i - 1][j];
				g_background[i-1][j] = 0;

				SetCursorPosition(i-1+ r, l + j * 2);
				printf("  ");
				SetCursorPosition(i+r,l+j*2);
				printf("■");

			}
		}
	}


}
void DrewScore() {
	int i = 8, j = 76;

	SetCursorPosition(i , j);
	printf("      %d\n",g_score*10);

}
bool Isdead() {

	if (Square_row == 1 || Square_row == 2)
	{
		g_pause = true;
		ClearRow(7);
		ClearRow(8);
		ClearRow(9);

		SetCursorPosition(12, 55);
		printf("game over");
		SetCursorPosition(13, 55);
		printf("Esc 退出游戏");
		SetCursorPosition(14, 55);
		printf("F1 重新开始游戏");
		return true;
	}
	else
		return false;
}

void ClearRow(char row) {

	char r = 5, l = 30;
	for (char i = LEFT + 1; i < RIGHT; i++)
	{
		if (g_background[row][i] == 1)
		{
			g_background[row][i] = 0;
			SetCursorPosition(row + r, l + i * 2);
			printf("  ");
		}
	}

}
void QuitGame(void) {

	g_flag = false;
	ClearRow(7);
	ClearRow(8);
	ClearRow(9);

	SetCursorPosition(12, 55);
	printf("游戏结束");
	SetCursorPosition(13, 55);
	printf("按任意建退出游戏");

}