
#include "PossiblePositionManager.h"
#include<cassert>
std::vector<std::pair<int, int>>dir = { {1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1} };
bool IsInBoard(int x, int y) {//判断是否在棋盘中
	if (x >= 0 && x < 15 && y >= 0 && y < 15)return true;
	return false;
}


void PossiblePositionManager::add(char board[15][15], const Position& p) {
	//人类下棋,将人类的点传给这个地方
	std::set<Position>addedPostions;//可能的位置是一组不重复的位置
	for (int i = 0; i < dir.size(); i++) {//将p点周围8个方向为空的点设为possiblePosition
		//对于该点来说其它方向
		int px = p.x + dir[i].first;
		int py = p.y + dir[i].second;
		if (!IsInBoard(px,py))continue;//如果这个点不在棋盘中,就继续
		if (board[px][py] == ChessEngine::Role_EMPTY) {
			Position pos(px, py);
			std::pair<std::set<Position>::iterator, bool>insertResult = currentPPos.insert(pos);
			if (insertResult.second)addedPostions.insert(pos);//如果当前可能落子的点不存在,就将这个点插入到add里
		}
	}
	//当这个点传进来的时候,说明这个点已经走过了,要删除
	HistoryItem hi;
	hi.addedPositons = addedPostions;//p点周围的所有点
	if (currentPPos.find(p) != currentPPos.end()) {//从当前可能落子的点中找到这个点,如果找到了,此时p是已经被落子的,让currentPPos删除它
		//并将Hi的removed点设为p
		currentPPos.erase(p);
		hi.removedPosition = p;
	}
	else {//否则设为-1
		hi.removedPosition.x = -1;
	}
	//放入到历史记录当中
	history.push_back(hi);
	
}

void PossiblePositionManager::Rollback() {
	if (currentPPos.empty())return;//
	HistoryItem hi = history[history.size() - 1];
	history.pop_back();//拿出最后一个历史记录
	std::set<Position>::iterator iter;
	for (iter = hi.addedPositons.begin(); iter != hi.addedPositons.end(); iter++) {
		currentPPos.erase(*iter);
	}
	if (hi.removedPosition.x != -1)currentPPos.insert(hi.removedPosition);
}

const std::set<Position>& PossiblePositionManager::get() {
	return currentPPos;
}
void PossiblePositionManager::clear() {
	currentPPos.clear();
	history.clear();
}
//插入操作,找到8个方向的空位置,尝试插入到possible当中,如果插入成功就说明该位置是possible的
//然后删除possible当中的点p,因为该点有棋子了,已经不是possible了,最后把记录push到history中
//删除操作:取出history最后一个,遍历当中的点并将他们从possible中删除,恢复remove点,
//获取操作,返回possible点
//清空操作,清空当前所有的possible和history
