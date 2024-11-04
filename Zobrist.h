#pragma once
#include "root.h"


using ll = long long;
//����һ��64λ�������

//ALPHA�ı߽�,BETA�ı߽�,EXACTȷ�е�ֵ,EMPTY����Ŀ
enum Flag { ALPHA = 0, BETA = 1, EXACT = 2, Flag_EMPTY = 3 };
//���һ���ڵ㱻�����BETA,��˵����������16
//���һ���ڵ㱻�����ALPHA,��˵���������16
//���һ���ڵ���EXACT,��˵������׼ȷֵ16


struct HashItem {
	ll ZVal;//ZobristValue��ֵ
	int depth;//�ݹ����
	int score;//��������
	Flag flag;//��Ŀ����
};

class Zobrist {
public:
	//��ʼ������,ÿһ��λ��������ZVal,�ֱ������Һ͵��Ե�ZVAl
	ll BoardZVal[2][BOARD_WIDTH][BOARD_WIDTH];
	ll CurrentZVal;
	HashItem hashItems[INDEX + 1];
	void add(int depth, int score, Flag flag);//������Ŀ
	int get(int depth, int alpha, int beta);//��ȡ��Ŀ
	void init();//��ʼ��һЩ����
};