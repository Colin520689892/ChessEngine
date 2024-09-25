#pragma once

#include "ChessEngine.h"
#include "root.h"
#include <set>
#include <vector>

struct HistoryItem {
	std::set<Position>addedPositons;
	Position removedPosition;
};

class PossiblePositionManager {
public:
	void add(int board[15][15], const Position& p);
	void Rollback();
	const std::set<Position>& get();
	void clear();
	

private:
	std::set<Position> currentPPos;//currentPossiblePosition,表示当前可能落子的点
	std::vector<HistoryItem> history;
	//当前可能的位置是一个集合,如果插入的是currentPossible里没有的元素,那么会返回true

};
