#pragma once
#include "root.h"


namespace ChessEngine {
	//现在给定了三个角色,人类是1,电脑是2,空位置是0
	enum Role{ HUMAN = 1, COMPUTER = 2, Role_EMPTY = 0 };
	void beforeStart();//初始化
	int isSomeOneWin();//谁赢了
	std::string takeBack();//悔棋
	void print();
	void dian();//电脑先手的时候调用
	//std::string reset(int role);
	void setLevel(int level);//设置难度
	Position getLastPosition();
	std::string nextStep(int x, int y);
}