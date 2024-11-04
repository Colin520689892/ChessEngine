#pragma once
#include "root.h"


namespace ChessEngine {
	//���ڸ�����������ɫ,������1,������2,��λ����0
	enum Role{ HUMAN = 1, COMPUTER = 2, Role_EMPTY = 0 };
	void beforeStart();//��ʼ��
	int isSomeOneWin();//˭Ӯ��
	std::string takeBack();//����
	void print();
	void dian();//�������ֵ�ʱ�����
	//std::string reset(int role);
	void setLevel(int level);//�����Ѷ�
	Position getLastPosition();
	std::string nextStep(int x, int y);
}