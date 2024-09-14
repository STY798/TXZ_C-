
#include<graphics.h>
#include<conio.h>

#define SCREEN_WIDTH 800   //����ͼƬ��С
#define SCREEN_HEIGHT 650

#define RATIO 50           //ͼƬ���ش�С

#define LINE 9             //��ͼ��С
#define COLUMN 12

#define START_X 100        //ͼƬ�ڱ����ϵ�ƫ��
#define START_Y 100

//�ȼ�����
#define KEY_UP    'w'
#define KEY_LEFT  'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN  's'
#define KEY_QUIT  'e'

#define NUM_LEVELS 3       //�ؿ���

#define isValid(pos) (pos.x >= 0 && pos.x < LINE && pos.y >= 0 && pos.y < COLUMN)

typedef enum _DIRECTION DIRECTION;
typedef struct _POS POS;
typedef enum _PROPS PROPS;

//����
enum _PROPS //ö��Ĭ�ϴ� 0 ��ʼ
{
	WALL,   //ǽ
	FLOOR,  //�ذ�
	BOX_DES,//����Ŀ�ĵ�
	MAN,    //С��
	BOX,    //����
	HIT,    //��������Ŀ��
	ALL     //���߱Ƹ� Ϊ�˺����ĸ��õĸĽ�
};

//��Ϸ����
enum _DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//С��λ��
struct _POS
{
	int x; //С�����ڵ���
	int y; //С�����ڵ���
};

//��ͼ
int map[NUM_LEVELS][LINE][COLUMN] = {
	{
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,2,4,1,1,1,1,1,0},
	{0,2,1,0,0,1,0,1,1,0,0,0},
	{0,4,1,0,0,1,1,1,1,0,0,0},
	{0,1,1,0,0,1,0,0,1,0,0,0},
	{0,0,1,1,1,3,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	},
	{
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	}
}; //ע���������

int current_level = 0; //���ڵĹؿ�

IMAGE images[ALL]; //ALL

POS man; //С��ʵʱ��λ��

//�ж���Ϸ�Ƿ�����������ͼ��û��һ������Ŀ�ĵؾͽ�����Ϸ
bool isGameOver()
{
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[current_level][i][j] == BOX_DES)
				return false;
		}
	}
	return true;
}

//��Ϸ�����ĳ���
void gameOverScene(IMAGE* bg)
{
	putimage(0, 0, bg);
	settextcolor(RED);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, "����");
	drawtext("��ϲ��ͨ�أ�\n ����������һ��", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //ˮƽ��ֱ������
}

/*
* �ı���Ϸ��ͼ��ͼ��һ���Ӧ���߲�������ʾ
* �����
* pos  -  ���ߵ���������
* props  -  ��������
* ����ֵ �� ��
*/
void changeMap(POS* pos, PROPS props) //ˢ��λ�ú�ͼƬ
{
	map[current_level][pos->x][pos->y] = props;
	putimage(START_X + pos->y * RATIO, START_Y + pos->x * RATIO, &images[props]);

}

/*
*ʵ����Ϸ4������
* ���룺
* direct һ��ǰ������
* �������
*/
void gameControl(DIRECTION direct)
{
	POS next_pos = man, next_next_pos = man;

	switch (direct)
	{
	case UP:
		next_pos.x = man.x - 1;
		next_next_pos.x = man.x - 2;
		break;
	case DOWN:
		next_pos.x = man.x + 1;
		next_next_pos.x = man.x + 2;
		break;
	case LEFT:
		next_pos.y = man.y - 1;
		next_next_pos.y = man.y - 2;
		break;
	case RIGHT:
		next_pos.y = man.y + 1;
		next_next_pos.y = man.y + 2;
		break;
	}

	//1.С��ǰ��һ������ǵذ壬��ǰ��һ������վ��next_pos��λ�ã���֮ǰ��λ�û�ԭ�ɵذ壩
	if (isValid(next_pos) && map[current_level][next_pos.x][next_pos.y] == FLOOR)
	{
		changeMap(&next_pos, MAN);
		changeMap(&man, FLOOR);
		man = next_pos;
	}

	//2.��ǰ��һ����������ӣ����ж�����ǰ���λ�ã���next_next_pos,������ǵذ����������Ŀ�ĵأ�����������ǰ��
	if (isValid(next_pos) && map[current_level][next_pos.x][next_pos.y] == BOX)
	{
		//2.1����ǰ�ǵذ�
		if (isValid(next_next_pos) && map[current_level][next_next_pos.x][next_next_pos.y] == FLOOR)
		{
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN);
			changeMap(&man, FLOOR);
			man = next_pos;
		}
		else if (isValid(next_next_pos) && map[current_level][next_next_pos.x][next_next_pos.y] == BOX_DES)
		{
			changeMap(&next_next_pos, HIT);
			changeMap(&next_pos, MAN);
			changeMap(&man, FLOOR);
			man = next_pos;
		}
	}
}

void loadMap(int current_level)    //����ÿ���ؿ���ͼ����
{
	for (int i = 0; i < LINE; i++) //��forѭ�����������ͼ������ж�
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[current_level][i][j] == MAN) //�����ͼ��ͬʱ���С�˵�λ��
			{
				man.x = i;
				man.y = j;
			}
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[current_level][i][j]]); //images[map[i][j]] ȡͼƬ
		}
	}
}

int main()
{
	IMAGE bg_img; //��ʾ����ͼƬ
	IMAGE bg1_img;//��Ϸ����ͼƬ

	//��̨
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);

	//���ñ���
	loadimage(&bg_img, "0.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true); // ����ͼƬ������true��������Ӧ
	putimage(0, 0, &bg_img); // ���ͼƬ����

	loadimage(&bg1_img, "7.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT, true);

	//ǽ:0, �ذ�:1, ����Ŀ�ĵ�:2, С��:3, ����:4, ��������Ŀ��6
	loadimage(&images[WALL], "1.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "0.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX_DES], "2.bmp", RATIO, RATIO, true);
	loadimage(&images[MAN], "3.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX], "4.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "6.bmp", RATIO, RATIO, true);

	for (int i = 0; i < LINE; i++) //��forѭ�����������ͼ������ж�
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[current_level][i][j] == MAN) //�����ͼ��ͬʱ���С�˵�λ��
			{
				man.x = i;
				man.y = j;
			}
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[current_level][i][j]]); //images[map[i][j]] ȡͼƬ
		}
	}

	//�ȼ����壺 �� :a  �£�s  �ϣ�w   �ң�d   �˳���q  
	bool quit = false;

	do
	{
		if (_kbhit()) //���� _kbhit() ���ܼ�����ֵ�� ��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0; include <conio.h>
		{
			char ch = _getch(); //����ָ�� ��������

			if (ch == KEY_UP)
			{
				gameControl(UP);
			}
			else if (ch == KEY_DOWN)
			{
				gameControl(DOWN);
			}
			else if (ch == KEY_LEFT)
			{
				gameControl(LEFT);
			}
			else if (ch == KEY_RIGHT)
			{
				gameControl(RIGHT);
			}
			else if (ch == KEY_QUIT)
			{
				quit = true;
			}

			if (isGameOver())
			{
				if (current_level < NUM_LEVELS - 1)
				{
					current_level++;
					loadMap(current_level); //���ڼ���ָ���ؿ��ĵ�ͼ��
				}
				else
					gameOverScene(&bg1_img);
			}
		}
		Sleep(100); //100ms = 0.1s, ִ��һ����Ϣ0.1s,��cpu��Ϣ.��Լ��Դ
	} while (quit == false);

	system("pause");

	return 0;
}