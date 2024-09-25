#pragma once
#include "root.h"


namespace ChessEngine {
	//���ڸ�����������ɫ,������1,������2,��λ����0
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