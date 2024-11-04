#include "Zobrist.h"

std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

void Zobrist::add(int depth, int score, Flag flag) {//新增条目
	int pos = (int)(CurrentZVal & INDEX);
	HashItem* tempItem = &hashItems[pos];//找到哈希表的对应位置
	//如果已有深度,且深度更大,就不添加
	if (tempItem->flag != Flag_EMPTY && tempItem->depth > depth)return;
	tempItem->ZVal = CurrentZVal;
	tempItem->score = score;
	tempItem->flag = flag;
	tempItem->depth = depth;
}
int Zobrist::get(int depth, int alpha, int beta) {//获取条目
	int pos = (int)(CurrentZVal & INDEX);
	HashItem* tempItem = &hashItems[pos];
	//如果找到的哈希条目为空,表示不存在
	Flag res = tempItem->flag;
	if (res == Flag_EMPTY)return UNKNOWN_SCORE;
	if (tempItem->ZVal == CurrentZVal && tempItem->depth >= depth) {
		if (res == EXACT)return tempItem->score;
		if (res == ALPHA && tempItem->score <= alpha)return alpha;//没有达到最佳着法,那么我便返回一个最佳着法
		if (res == BETA && tempItem->score >= beta)return beta;//我高于了边界,那么我返回边界
	}
	return UNKNOWN_SCORE;
}
void Zobrist::init() {//初始化一些函数
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			for (int k = 0; k < BOARD_WIDTH; k++) {
				BoardZVal[i][j][k] = rng();
			}
		}
	}
	CurrentZVal = rng();
}