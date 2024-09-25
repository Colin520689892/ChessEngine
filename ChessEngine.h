#pragma once
#include "root.h"


namespace ChessEngine {
	//现在给定了三个角色,人类是1,电脑是2,空位置是0
	enum Role{ HUMAN = 1, COMPUTER = 2, EMPTY = 0 };
	void beforeStart();
	int isSomeOneWin();
	std::string takeBack();
	void print();
	//std::string reset(int role);
	//void setLevel(int level);
	Position getLastPosition();
	std::string nextStep(int x, int y);
}