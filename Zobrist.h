#pragma once
#include "root.h"


using ll = long long;
//生成一个64位的随机数

//ALPHA的边界,BETA的边界,EXACT确切的值,EMPTY空条目
enum Flag { ALPHA = 0, BETA = 1, EXACT = 2, EMPTY = 3 };

struct HashItem {
	ll ZVal;//ZobristValue的值
	int depth;//递归深度
	int score;//局面评分
	Flag flag;//条目类型
};

class Zobrist {
public:
	//初始化棋盘,每一个位置有两个ZVal,分别代表玩家和电脑的ZVAl
	ll BoardZVal[2][BOARD_WIDTH][BOARD_WIDTH];
	ll CurrentZVal;
	HashItem hashItems[INDEX + 1];
	void add(int depth, int score, Flag flag);//新增条目
	int get(int depth, int alpha, int beta);//获取条目
	void init();//初始化一些函数
};