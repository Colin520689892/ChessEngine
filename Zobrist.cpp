#include "Zobrist.h"

std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

void Zobrist::add(int depth, int score, Flag flag) {//������Ŀ
	int pos = (int)(CurrentZVal & INDEX);
	HashItem* tempItem = &hashItems[pos];//�ҵ���ϣ��Ķ�Ӧλ��
	//����������,����ȸ���,�Ͳ����
	if (tempItem->flag != Flag_EMPTY && tempItem->depth > depth)return;
	tempItem->ZVal = CurrentZVal;
	tempItem->score = score;
	tempItem->flag = flag;
	tempItem->depth = depth;
}
int Zobrist::get(int depth, int alpha, int beta) {//��ȡ��Ŀ
	int pos = (int)(CurrentZVal & INDEX);
	HashItem* tempItem = &hashItems[pos];
	//����ҵ��Ĺ�ϣ��ĿΪ��,��ʾ������
	Flag res = tempItem->flag;
	if (res == Flag_EMPTY)return UNKNOWN_SCORE;
	if (tempItem->ZVal == CurrentZVal && tempItem->depth >= depth) {
		if (res == EXACT)return tempItem->score;
		if (res == ALPHA && tempItem->score <= alpha)return alpha;//û�дﵽ����ŷ�,��ô�ұ㷵��һ������ŷ�
		if (res == BETA && tempItem->score >= beta)return beta;//�Ҹ����˱߽�,��ô�ҷ��ر߽�
	}
	return UNKNOWN_SCORE;
}
void Zobrist::init() {//��ʼ��һЩ����
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			for (int k = 0; k < BOARD_WIDTH; k++) {
				BoardZVal[i][j][k] = rng();
			}
		}
	}
	CurrentZVal = rng();
}