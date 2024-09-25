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
	std::set<Position> currentPPos;//currentPossiblePosition,��ʾ��ǰ�������ӵĵ�
	std::vector<HistoryItem> history;
	//��ǰ���ܵ�λ����һ������,����������currentPossible��û�е�Ԫ��,��ô�᷵��true

};
