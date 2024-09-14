
#include<graphics.h>
#include<conio.h>

#define SCREEN_WIDTH 800   //背景图片大小
#define SCREEN_HEIGHT 650

#define RATIO 50           //图片像素大小

#define LINE 9             //地图大小
#define COLUMN 12

#define START_X 100        //图片在背景上的偏移
#define START_Y 100

//热键控制
#define KEY_UP    'w'
#define KEY_LEFT  'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN  's'
#define KEY_QUIT  'e'

#define NUM_LEVELS 3       //关卡数

#define isValid(pos) (pos.x >= 0 && pos.x < LINE && pos.y >= 0 && pos.y < COLUMN)

typedef enum _DIRECTION DIRECTION;
typedef struct _POS POS;
typedef enum _PROPS PROPS;

//道具
enum _PROPS //枚举默认从 0 开始
{
	WALL,   //墙
	FLOOR,  //地板
	BOX_DES,//箱子目的地
	MAN,    //小人
	BOX,    //箱子
	HIT,    //箱子命中目标
	ALL     //更高逼格， 为了后续的更好的改进
};

//游戏方向
enum _DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//小人位置
struct _POS
{
	int x; //小人所在的行
	int y; //小人所在的列
};

//地图
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
}; //注意结束符号

int current_level = 0; //所在的关卡

IMAGE images[ALL]; //ALL

POS man; //小人实时的位置

//判断游戏是否结束，如果地图上没有一个箱子目的地就结束游戏
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

//游戏结束的场景
void gameOverScene(IMAGE* bg)
{
	putimage(0, 0, bg);
	settextcolor(RED);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, "宋体");
	drawtext("恭喜你通关！\n 奖励背带裤一条", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //水平垂直都居中
}

/*
* 改变游戏地图视图中一格对应道具并重新显示
* 输出：
* pos  -  道具的行列坐标
* props  -  道具类型
* 返回值 ： 无
*/
void changeMap(POS* pos, PROPS props) //刷新位置和图片
{
	map[current_level][pos->x][pos->y] = props;
	putimage(START_X + pos->y * RATIO, START_Y + pos->x * RATIO, &images[props]);

}

/*
*实现游戏4个方向
* 输入：
* direct 一人前进方向
* 输出：无
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

	//1.小人前进一步如果是地板，人前进一步（人站到next_pos的位置，人之前的位置还原成地板）
	if (isValid(next_pos) && map[current_level][next_pos.x][next_pos.y] == FLOOR)
	{
		changeMap(&next_pos, MAN);
		changeMap(&man, FLOOR);
		man = next_pos;
	}

	//2.人前进一步如果是箱子，则判断箱子前面的位置，即next_next_pos,如果他是地板或者是箱子目的地，人推箱子往前走
	if (isValid(next_pos) && map[current_level][next_pos.x][next_pos.y] == BOX)
	{
		//2.1箱子前是地板
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

void loadMap(int current_level)    //用于每个关卡地图加载
{
	for (int i = 0; i < LINE; i++) //用for循环解决整个地图的输出判断
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[current_level][i][j] == MAN) //输出地图的同时标记小人的位置
			{
				man.x = i;
				man.y = j;
			}
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[current_level][i][j]]); //images[map[i][j]] 取图片
		}
	}
}

int main()
{
	IMAGE bg_img; //表示背景图片
	IMAGE bg1_img;//游戏结束图片

	//搭台
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);

	//设置背景
	loadimage(&bg_img, "0.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true); // 加载图片函数；true参数自适应
	putimage(0, 0, &bg_img); // 输出图片函数

	loadimage(&bg1_img, "7.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT, true);

	//墙:0, 地板:1, 箱子目的地:2, 小人:3, 箱子:4, 箱子命中目标6
	loadimage(&images[WALL], "1.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "0.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX_DES], "2.bmp", RATIO, RATIO, true);
	loadimage(&images[MAN], "3.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX], "4.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "6.bmp", RATIO, RATIO, true);

	for (int i = 0; i < LINE; i++) //用for循环解决整个地图的输出判断
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[current_level][i][j] == MAN) //输出地图的同时标记小人的位置
			{
				man.x = i;
				man.y = j;
			}
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[current_level][i][j]]); //images[map[i][j]] 取图片
		}
	}

	//热键定义： 左 :a  下：s  上：w   右：d   退出：q  
	bool quit = false;

	do
	{
		if (_kbhit()) //函数 _kbhit() 功能及返回值： 检查当前是否有键盘输入，若有则返回一个非0值，否则返回0; include <conio.h>
		{
			char ch = _getch(); //输入指令 上下左右

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
					loadMap(current_level); //用于加载指定关卡的地图。
				}
				else
					gameOverScene(&bg1_img);
			}
		}
		Sleep(100); //100ms = 0.1s, 执行一次休息0.1s,让cpu休息.节约资源
	} while (quit == false);

	system("pause");

	return 0;
}