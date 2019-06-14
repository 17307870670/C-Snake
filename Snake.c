#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define PAUSE ' '
#define WIDTH 52
#define HEIGHT 27
#define NAME_SIZE 128
#define MAX_PLAYER 1024

// * ����ʵ��̰��������
typedef struct Snake
{
	int x;
	int y;
	struct Snake *next;
}Snake;
// * ��ʳ�������ͷ�������һ���ṹ�����棬ʳ��ķ����ߵ����ѶȾ���
typedef struct food
{
	int x;
	int y;
	int score;
}Food;

typedef struct playerInfo
{
	char name[NAME_SIZE];
	int high_score;
	int play_times;
}Player;



// ************************************************************************
// * 
// * ��Ϸ�е�ȫ�ֱ���
// *
// ************************************************************************
char cGameScene[HEIGHT][WIDTH];             // �����Ϸ������СΪ 50 x 25 
char cUserName[NAME_SIZE];
int iTotalScore;
int iLastScore;
int iTotalTimes = 0;

int iDifficulty = 1;
int iRefreshTimes[4] = { 50, 25, 5, 0 };
int iGameOver = 0;
int iFoodSpawned = 0;
char cControl;
Snake *head, *tail;
Food food;

// ************************************************************************
// * 
// * ��������
// *
// ************************************************************************
void initGame();             // ��ʼ����Ϸ��������ȫ�ֱ���
void refreshScreen();        // ÿһ֡����Ļˢ��
void gameLoop();             // ��Ϸ�߼���ѭ��

void enterUsername();                            // �ս�����Ϸʱ��ѡ�����½���һ��Ƕ�ȡ�浵
void startMenu();                                // ��ʼ��Ϸ���ѡ��˵�
void chooseDifficulty();                         // ѡ����Ϸ�Ѷ�
void displayScoreBoard();                        // ��ʾ���а�
void myInfo();                                   // ��ʾ�ҵ���Ϣ
void gameOver();                                 // ��Ϸ����ҳ��
void updateScoreBoard();                         // �������а�

void updateSnakeShape();
int randint(int min, int max);
int ifGameOver();
void spawnFood();

void gotoxy(int x, int y);

int main(void)
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	srand((unsigned int)time(0));
	enterUsername();
	while (1)
	{
		startMenu();
		
		system("cls");
		initGame();

		gameLoop();
		gameOver();
		system("cls");
	}
	return 0;
}

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼����豸���
	SetConsoleCursorPosition(hOut, pos);//���������ֱ���ָ���ĸ����壬����λ��
}

int randint(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void enterUsername()
{
	int iSelect;
	char cSearchName[NAME_SIZE], cName[NAME_SIZE];
	FILE *fp;
	int iScore;
	int iFound = 0;

	system("cls");
	printf("====================================================\n");
	printf("#                                                  #\n");
	printf("#                                                  #\n");
	printf("#                  1. ���������                   #\n");
	printf("#                  2. ��ȡ�浵                     #\n");
	printf("#                                                  #\n");
	printf("#                                                  #\n");
	printf("====================================================\n");
	printf("                  ѡ�");

	scanf("%d", &iSelect);
	getchar();
	fflush(stdin);
	if (iSelect == 1)
	{
		system("cls");
		printf("\t\t�����û�����");
		gets(cUserName);
	}
	else if (iSelect == 2)
	{
		system("cls");
		if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
		{
			printf("\t\t�浵�ļ���ȡʧ�ܣ�������Ϸ������.\n");
			printf("\t\t��������˳���Ϸ\n");
			char ch = _getch();
			exit(1);
		}
		
		printf("\t\t����浵����");
		gets(cSearchName);

		while (!feof(fp))
		{
			fscanf(fp, "%s%d%d\n", cName, &iScore, &iTotalTimes);
			if (strcmp(cSearchName, cName) == 0)
			{
				iFound = 1;
				strcpy(cUserName, cSearchName);
				iLastScore = iScore;
				printf("\t\t�ҵ���ң�%s, ��ǰ������%d, ��Ϸ�ܴ�����%d\n", cUserName, iLastScore, iTotalTimes);
				printf("\t\t�����������.\n");
				char ch = _getch();
				break;
			}
		}
		if (iFound == 0)
		{
			printf("\t\tû���ҵ��ô浵��Ϣ�������Ƿ���ȷ����浵��.\n");
			printf("                    ����������س�ʼ�˵�\n");
			char ch = _getch();
			enterUsername();
		}
		fclose(fp);
	}
	else
	{
		printf("                    ѡ�����\n");
		printf("                    ����������س�ʼ�˵�\n");
		char ch = _getch();
		enterUsername();
	}
}

void startMenu()
{
	int iSelect;

	system("cls");
	printf("====================================================\n");
	printf("#                                                  #\n");
	printf("#                  ̰���� Version Alpha            #\n");
	printf("#                                                  #\n");
	printf("#                1. ��ʼ��Ϸ                       #\n");
	printf("#                2. �鿴���а�                     #\n");
	printf("#                3. �鿴�ҵķ���                   #\n");
	printf("#                4. �л����                       #\n");
	printf("#                5. �˳���Ϸ                       #\n");
	printf("#                                                  #\n");
	printf("#                                                  #\n");
	printf("====================================================\n");

	printf("                  ѡ�");
	scanf("%d", &iSelect);

	system("cls");
	if (iSelect == 1)
	{
		chooseDifficulty();
		iTotalTimes += 1;
		printf("                  Good Luck ! ! !\n");
		Sleep(2000);
	}
	else if (iSelect == 2)
	{
		displayScoreBoard();
		printf("                    ����������ؿ�ʼ����\n");
		char ch = _getch();
		startMenu();
	}
	else if (iSelect == 3)
	{
		myInfo();
		printf("                    ����������ؿ�ʼ����\n");
		char ch = _getch();
		startMenu();
	}
	else if (iSelect == 5)
	{
		printf("\t\t���ڽ���ķ���д�����а�����ر���Ϸ\n");
		updateScoreBoard();
		Sleep(500);
		printf("                    ��������˳���Ϸ\n");
		char ch = _getch();
		exit(1);
	}
	else if (iSelect == 4)
	{
		updateScoreBoard();
		enterUsername();
		startMenu();
	}
	else
	{
		printf("                    ѡ�����\n");
		printf("                    ����������ؿ�ʼ����\n");
		fflush(stdin);
		char ch = _getch();
		startMenu();
	}
}

void chooseDifficulty()
{
	int iSelect;

	printf("           1. ��  2. ��ͨ  3. ����  4. ج��\n");
	printf("                  ѡ�");
	scanf("%d", &iSelect);

	if (iSelect <= 4 && iSelect >= 1)
	{
		iDifficulty = iSelect;
	}
	else
	{
		printf("             ����ȷ����ѡ��\n");
		printf("                    ���������ѡ\n");
		char ch = _getch();
		chooseDifficulty();
	}
}

void displayScoreBoard()
{
	FILE *fp;
	char cName[NAME_SIZE];
	int iScore;
	int iTimes;

	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf("\t\t�浵�ļ���ȡʧ�ܣ�������Ϸ������.\n");
		printf("\t\t��������˳���Ϸ\n");
		char ch = _getch();
		exit(1);
	}
	
	system("cls");
	printf("======================  ���а�  ====================\n");
	while (!feof(fp))
	{
		fscanf(fp, "%s%d%d\n", cName, &iScore, &iTimes);
		printf("               %s : %d\n", cName, iScore);
	}
	printf("====================================================\n");
	fclose(fp);
}

void updateScoreBoard()
{
	FILE *fp;
	Player playerList[MAX_PLAYER];
	int iPlayerCount = 0;
	int iFound = 0;
	int i, j;


	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf("\t\t�浵�ļ���ȡʧ�ܣ�������Ϸ������.\n");
		printf("\t\t��������˳���Ϸ\n");
		char ch = _getch();
		exit(1);
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s%d%d\n", playerList[iPlayerCount].name, &playerList[iPlayerCount].high_score, &playerList[iPlayerCount].play_times);
		if (strcmp(cUserName, playerList[iPlayerCount].name) == 0)
		{
			iFound = 1;
			if (iTotalScore > playerList[iPlayerCount].high_score)
			{
				playerList[iPlayerCount].high_score = iTotalScore;
			}
			playerList[iPlayerCount].play_times = iTotalTimes;
		}
		
		iPlayerCount++;
	}
	if (iFound == 0)
	{
		playerList[iPlayerCount].high_score = iTotalScore;
		strcpy(playerList[iPlayerCount].name, cUserName);
		playerList[iPlayerCount].play_times = iTotalTimes;
		iPlayerCount++;
	}

	for (i = 0; i < iPlayerCount; i++)
	{
		for (j = 0; j < iPlayerCount - i - 1; j++)
		{
			if (playerList[j].high_score < playerList[j + 1].high_score)
			{
				Player temp = playerList[j];
				playerList[j] = playerList[j + 1];
				playerList[j + 1] = temp;
			}
		}
	}
	i = 0;
	fseek(fp, 0, SEEK_SET);
	while (i < iPlayerCount)
	{
		fprintf(fp, "%s %d %d\n", playerList[i].name, playerList[i].high_score, playerList[i].play_times);
		i++;
	}
	fclose(fp);
}

void myInfo()
{
	int iScore;
	char cName[NAME_SIZE];	
	FILE *fp;
	int iFound = 0;

	if ((fp = fopen("scoreBoard.txt", "r+")) == NULL)
	{
		printf("\t\t�浵�ļ���ȡʧ�ܣ�������Ϸ������.\n");
		printf("\t\t��������˳���Ϸ\n");
		char ch = _getch();
		exit(1);
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s%d\n", cName, &iScore);
		if (strcmp(cName, cUserName) == 0)
		{
			iFound = 1;
			iLastScore = iScore;
			printf("\t\t��ң�%s,  ��ʷ��߷֣�%d,  ��Ϸ����: %d\n", cUserName, iLastScore, iTotalTimes);
			break;
		}
	}
	if (iFound == 0)
	{
		if (iFound == 0)
		{
			printf("\t\t��ң�%s,  ��ǰ�ܷ֣�%d,  ��Ϸ������%d\n", cUserName, iTotalScore, iTotalTimes);
			printf("\t\t�����Ϣ��ʱδд��浵�����Ժ�... ...\n");
			updateScoreBoard();
			Sleep(1000);
		}
	}
	fclose(fp);
}

void gameOver()
{
	system("cls");
	printf("====================================================\n\n\n\n");
	printf("                      GAME OVER                     \n\n");
	printf("                  �ܷ֣�%4d\n\n\n", iTotalScore);
	printf("====================================================\n");
	
	if (iTotalScore > iLastScore)
	{
		updateScoreBoard();
		iLastScore = iTotalScore;
	}
	printf("                    ����������ؿ�ʼ����\n");
	char ch = _getch();
	iFoodSpawned = 0;
	iTotalScore = 0;
}

void initGame()
{
	int i, j;

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
			{
				cGameScene[i][j] = '#';
			}
			else
			{
				cGameScene[i][j] = ' ';
			}
		}
	}
	head = (Snake*)malloc(sizeof(Snake));
	head->x = 24;
	head->y = 12;
	head->next = (Snake*)malloc(sizeof(Snake));
	head->next->x = 23;
	head->next->y = 12;
	head->next->next = NULL;
	tail = head->next;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			putchar(cGameScene[i][j]);
		}
		putchar('\n');
	}
}

void refreshScreen()
{
	int i, j;

	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			gotoxy(j, i);
			putchar(cGameScene[i][j]);
		}
	}
	gotoxy(WIDTH + 5, 12);
	printf("�÷֣�%d", iTotalScore);
}

void updateSnakeShape()
{
	Snake *node = head;
	int x, y, preX = head->x, preY = head->y;
	int i, j;

	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			if (cGameScene[i][j] != '*')
			{
				cGameScene[i][j] = ' ';
			}
		}
	}
	if (head->next != NULL && (cControl == UP || cControl == DOWN || cControl == LEFT || cControl == RIGHT))
	{
		while (node != NULL)
		{
			x = node->x;
			y = node->y;
			node->x = preX;
			node->y = preY;
			preX = x;
			preY = y;
			node = node->next;
		}
	}
	node = head->next;
	while (node != NULL)
	{
		cGameScene[node->y][node->x] = 'O';
		node = node->next;
	}
}

int ifGameOver()
{
	Snake *node = head->next;

	if (head->x == WIDTH - 1 || head->x == 0 || head->y == HEIGHT - 1 || head->y == 0)
	{
		return 1;
	}
	while (node != NULL)
	{
		if (head->x == node->x && head->y == node->y)
		{
			return 1;
		}
		node = node->next;
	}
	return 0;
}

void spawnFood()
{
	food.x = randint(1, WIDTH - 2);
	food.y = randint(1, HEIGHT - 2);
	food.score = iDifficulty;

	Snake *node = head;
	while (node != NULL)
	{
		if (node->x == food.x && node->y == food.y)
		{
			food.x = randint(1, WIDTH - 2);
			food.y = randint(1, HEIGHT - 2);
			node = head;
			continue;
		}
		node = node->next;
	}

	cGameScene[food.y][food.x] = '*';

	iFoodSpawned = 1;
}

void gameLoop()
{
	
	while (!ifGameOver())
	{
		Sleep(iRefreshTimes[iDifficulty - 1]);
		if (_kbhit())
		{
			cControl = _getch();
		}
		updateSnakeShape();

		switch (cControl)
		{
		case UP: head->y -= 1; break;
		case DOWN: head->y += 1; break;
		case LEFT: head->x -= 1; break;
		case RIGHT: head->x += 1; break;
		default: break;
		}
		cGameScene[head->y][head->x] = '@';

		if (iFoodSpawned == 0)
		{
			spawnFood();
		}

		if (head->x == food.x && head->y == food.y)
		{
			iTotalScore += 4;
			tail->next = (Snake*)malloc(sizeof(Snake));
			switch (cControl)
			{
			case UP: tail->next->y = tail->y + 1; tail->next->x = tail->x; break;
			case DOWN: tail->next->y = tail->y - 1; tail->next->x = tail->x; break;
			case LEFT: tail->next->y = tail->y; tail->next->x = tail->x + 1; break;
			case RIGHT: tail->next->y = tail->y; tail->next->x = tail->x - 1; break;
			}
			tail = tail->next;
			tail->next = NULL;
			iFoodSpawned = 0;
		}
		refreshScreen();
	}
}