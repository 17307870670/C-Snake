#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

// * ============================
// * ���尴��
// * ============================
#define UP 'w'
//#define UP_2 38
#define DOWN 's'
//#define DOWN_2 40
#define LEFT 'a'
//#define LEFT_2 37
#define RIGHT 'd'
//#define RIGHT_2 40
#define CONFIG_ON 'x'
#define CONFIG_OFF 'c'
#define QUIT 'q'

// * ==========================
// * �������ܺ���
// * ==========================
int randint(int min, int max);
void HideCursor();
void gotoxy(int x, int y);
void gotoPrint(int x, int y, char *content);
void initGame();
void clearXY(int x, int y);
void clearConfigInfo();

// * ==========================
// * ��Ϸ�߼�����
// * ==========================
void getControl();
void gameLoop();
int ifGameEnd();
void createFood();
void snakeOverride();
void eatFood();
void changeBody();
void startMenu();
void gameOver();

void displayList();
void findMyInfo();

// * ==========================
// * ����ʵ��̰��������
// * ����������ڵ� �ṹ�壬 �������꣨x, y), ����һ�ڵ�ָ��
// * ==========================
struct Snakes
{
	int x;
	int y;
	char type;
	struct Snakes *next;
};
typedef struct Snakes snake;


// * ==========================
// * ʳ��ṹ�壬 �������� (x, y) �� ÿ��ʳ���������������ֵ��
// * ==========================
struct Food
{
	int x;
	int y;
	int score;
};

// * ��ͷ����β�ڵ�
snake *head, *tail;
// * ����ʳ��ṹ�����
struct Food food;

int score = 0;
int length = 1;
char cUserName[20];

// * ���水������
char control = 0;

// * ��Ϸ�Ѷ�
int difficulty = 200;
int easy = 200;
int medium = 90;
int hard = 50;
int nightmare = 30;

// * ���Լ���ر���
int paused = 0;
int endGame = 0;
int iQuit = 0;
unsigned int foodEaten = 0;
unsigned int configMode = 0;
unsigned int hitTimes = 0;

// * ==========================
// * main����
int main(void)
{
	char cKey;

	srand((int)time(NULL));

	while (1)
	{
		system("cls");
		printf("�����û�����");
		gets(cUserName);
		HideCursor();
		startMenu();
		initGame();
		gameLoop();
		fflush(stdin);
		
		printf("�� E �˳���Ϸ.");
		cKey = _getch();
		if (cKey == 'e' || cKey == 'E')
		{
			break;
		}
		// д���������
		
	}
	
	gotoxy(0, 26);
	return 0;
}
// * ==========================
int randint(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
// * ==========================
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
// * ==========================
void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void gotoPrint(int x, int y, char *content)
{
	gotoxy(x, y);
	puts(content);
}
void clearXY(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
}
// * ==========================
void initGame()
{
	int i;

	for (i = 0; i < 58; i++)
	{
		gotoPrint(i, 0, "#");
		gotoPrint(i, 25, "#");
	}
	for (i = 0; i < 26; i++)
	{
		gotoPrint(0, i, "#");
		gotoPrint(57, i, "#");
	}

	head = (snake*)malloc(sizeof(snake));
	head->x = 25;
	head->y = 10;
	head->next = NULL;
	tail = head;
	gotoPrint(head->x, head->y, "@");
	createFood();

	//gotoPrint(64, 8, "Welcome to play the Snake! ! !");
	//gotoPrint(64, 9, "---------Version: BETA--------");
	gotoPrint(64, 10, "���� WASD ������");
	/*if (paused)
		gotoPrint(64, 11, "Press w a s d to continue.");
	else
		gotoPrint(64, 11, "Press p to pause game.");*/
	gotoPrint(64, 11, "���� P ��ͣ��Ϸ");
	gotoPrint(64, 12, "====================================");
	gotoxy(64, 14);
	printf("��ǰ�÷�: %2d", score);
}

// * ==========================

void clearConfigInfo()
{
	int i, j;

	for (i = 20; i <= 26; i++)
	{
		for (j = 64; j <= 100; j++)
		{
			clearXY(j, i);
		}
	}
}

// * ==========================

void getControl()
{
	if (_kbhit())
	{
		hitTimes++;
		control = _getch();
	}
}

// * ==========================

int ifGameEnd()
{
	if (head->x == 57 || head->x == 0 || head->y == 25 || head->y == 0)
		return 1;

	snake *node = head->next;
	if (head != tail)
	{
		while (1)
		{
			if (node->x == head->x && node->y == head->y)
			{
				return 1;
				break;
			}
			if (node->next == NULL)
				break;
			node = node->next;
		}
	}
	return 0;
}

// * ==========================

void createFood()
{
	int x, y, score;

	score = randint(1, 3);

	snake *node = head;
	
LABLE:
	x = randint(1, 56);
	y = randint(1, 24);
	while (1)
	{
		if (node->x == x && node->y == y)
			goto LABLE;
		if (node->next == NULL)
			break;
		node = node->next;
	}

	food.x = x;
	food.y = y;
	food.score = score;
	gotoPrint(food.x, food.y, "*");
	foodEaten = 0;
}

// * ==========================

void snakeOverride()
{
	snake *node = head;

	while (1)
	{
		clearXY(node->x, node->y);
		if (node->next == NULL)
			break;
		node = node->next;
	}
	
	switch (control)
	{
	case UP:changeBody(); head->y--; paused = 0; break;
	case DOWN:changeBody(); head->y++; paused = 0; break;
	case LEFT:changeBody(); head->x--; paused = 0;  break;
	case RIGHT:changeBody(); head->x++; paused = 0; break;
	case QUIT:endGame = 1; break;
	default:paused = 1; break;
	}

	node = head;
	while (1)
	{
		gotoPrint(node->x, node->y, "O");
		if (node->next == NULL)
			break;
		node = node->next;
	}
	gotoPrint(head->x, head->y, "@");
}

// * ==========================

void eatFood()
{
	if (head->x == food.x && head->y == food.y)
	{
		foodEaten = 1;
		score += food.score;
		length += 1;
		tail->next = (snake*)malloc(sizeof(snake));
		tail->next->next = NULL;
		tail = tail->next;
	}
}

// * ==========================

void changeBody()
{
	snake *node = head->next;

	int x1 = head->x, y1 = head->y, x2, y2;

	if (tail != head)
	{
		while (1)
		{
			x2 = node->x;
			y2 = node->y;
			node->x = x1;
			node->y = y1;
			x1 = x2;
			y1 = y2;
			if (node->next == NULL)
				break;
			node = node->next;
		}
	}
}

// * ==========================

void displayList()
{
	FILE *fp;
	int iScore;
	char cUserName[20];
	char key;

	system("cls");
	gotoxy(0, 0);
	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf(" �޷����ļ���������Ϸ������...");
		exit(0);
	}

	printf("\t\t\t\t���а�\n\n");
	while (!feof(fp))
	{
		fscanf(fp, "%s%d\n", cUserName, &iScore);
		printf("\t\t%s:  %d\n\n", cUserName, iScore);
	}

	printf("\t\t\t\t�����������");
	key = _getch();
	fclose(fp);
}

void findMyInfo(char myName[20])
{
	FILE *fp;
	char cUserName[20];
	int iScore;
	int iFound = 0;
	char key;

	system("cls");
	gotoxy(0, 0);
	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf(" �޷����ļ���������Ϸ������...");
		exit(0);
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s%d\n", cUserName, &iScore);
		if (strcmp(cUserName, myName) == 0)
		{
			iFound = 1;
			printf("\t\t\t\t���������\n\n");
			printf("\t\t��ң�     %s\n\n", myName);
			printf("\t\t�ܷ֣�     %d\n", iScore);
			break;
		}
	}
	if (iFound == 0)
	{
		printf("\t\tδ�ҵ������Ϣ\n");
	}
	printf("\t\t\t\t�����������");
	key = _getch();
	fclose(fp);
}

void startMenu()
{
	int select;
	int i;

	system("cls");
	for (i = 0; i < 58; i++)
	{
		gotoPrint(i, 0, "#");
		gotoPrint(i, 25, "#");
	}
	for (i = 0; i < 26; i++)
	{
		gotoPrint(0, i, "#");
		gotoPrint(57, i, "#");
	}
	gotoPrint(1, 10, "=================Welcome to play the snake==============");
	gotoPrint(1, 12, "                                                        ");
	gotoPrint(1, 14, "                      ��ѡ���Ѷ�:                  ");
	gotoPrint(1, 16, "      1. ��; 2. ��ͨ; 3. ����; 4. ج��; 5. �˳���Ϸ");
	gotoPrint(1, 17, "      6. ��ʾ���а�; 7. �鿴�ҵķ���; ");
	gotoPrint(1, 19, "                    ѡ��:");
	gotoxy(28, 19);
	scanf("%d", &select);
	switch (select)
	{
	case 1:difficulty = easy; break;
	case 2:difficulty = medium; break;
	case 3:difficulty = hard; break;
	case 4:difficulty = nightmare; break;
	case 5:system("cls"); exit(1); break;
	case 6: displayList(); system("cls"); startMenu(); break;
	case 7: findMyInfo(cUserName); system("cls"); startMenu(); break;
	default:break;
	}
	gotoPrint(1, 20, "                     Good Luck ! ! !                    ");
	Sleep(2000);
	system("cls");
}

// * ==========================

void gameOver()
{
	system("cls");
	int i;

	for (i = 0; i < 58; i++)
	{
		gotoPrint(i, 0, "#");
		gotoPrint(i, 25, "#");
	}
	for (i = 0; i < 26; i++)
	{
		gotoPrint(0, i, "#");
		gotoPrint(57, i, "#");
	}

	gotoPrint(1, 15, "=======================GAME OVER========================");
	gotoxy(1, 17);
	printf("                       ���յ÷�: %d", score);
	
	Sleep(6000);
	gotoPrint(1, 19, "                    ��������ص����˵�");
	while (1)
	{
		if (_kbhit())
			break;
	}
}

// * ==========================

void gameLoop()
{
	FILE *fp;

	system("cls");
	gotoxy(0, 0);
	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf(" �޷����ļ���������Ϸ������...");
		exit(0);
	}
	while (!ifGameEnd() && !endGame)
	{
		if (foodEaten)
			createFood();
		getControl();
		eatFood();
		snakeOverride();

		gotoxy(64, 14);
		printf("��ǰ�÷�: %2d", score);
		gotoxy(64, 15);
		printf("������: %2d", length);
		
		Sleep(difficulty);
	}
	gameOver();
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n%s %d\n", cUserName, score);
	fputc(EOF, fp);
	score = 0;
	fclose(fp);
}