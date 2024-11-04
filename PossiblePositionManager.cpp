
#include "PossiblePositionManager.h"
#include<cassert>
std::vector<std::pair<int, int>>dir = { {1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1} };
bool IsInBoard(int x, int y) {//�ж��Ƿ���������
	if (x >= 0 && x < 15 && y >= 0 && y < 15)return true;
	return false;
}


void PossiblePositionManager::add(char board[15][15], const Position& p) {
	//��������,������ĵ㴫������ط�
	std::set<Position>addedPostions;//���ܵ�λ����һ�鲻�ظ���λ��
	for (int i = 0; i < dir.size(); i++) {//��p����Χ8������Ϊ�յĵ���ΪpossiblePosition
		//���ڸõ���˵��������
		int px = p.x + dir[i].first;
		int py = p.y + dir[i].second;
		if (!IsInBoard(px,py))continue;//�������㲻��������,�ͼ���
		if (board[px][py] == ChessEngine::Role_EMPTY) {
			Position pos(px, py);
			std::pair<std::set<Position>::iterator, bool>insertResult = currentPPos.insert(pos);
			if (insertResult.second)addedPostions.insert(pos);//�����ǰ�������ӵĵ㲻����,�ͽ��������뵽add��
		}
	}
	//������㴫������ʱ��,˵��������Ѿ��߹���,Ҫɾ��
	HistoryItem hi;
	hi.addedPositons = addedPostions;//p����Χ�����е�
	if (currentPPos.find(p) != currentPPos.end()) {//�ӵ�ǰ�������ӵĵ����ҵ������,����ҵ���,��ʱp���Ѿ������ӵ�,��currentPPosɾ����
		//����Hi��removed����Ϊp
		currentPPos.erase(p);
		hi.removedPosition = p;
	}
	else {//������Ϊ-1
		hi.removedPosition.x = -1;
	}
	//���뵽��ʷ��¼����
	history.push_back(hi);
	
}

void PossiblePositionManager::Rollback() {
	if (currentPPos.empty())return;//
	HistoryItem hi = history[history.size() - 1];
	history.pop_back();//�ó����һ����ʷ��¼
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
//�������,�ҵ�8������Ŀ�λ��,���Բ��뵽possible����,�������ɹ���˵����λ����possible��
//Ȼ��ɾ��possible���еĵ�p,��Ϊ�õ���������,�Ѿ�����possible��,���Ѽ�¼push��history��
//ɾ������:ȡ��history���һ��,�������еĵ㲢�����Ǵ�possible��ɾ��,�ָ�remove��,
//��ȡ����,����possible��
//��ղ���,��յ�ǰ���е�possible��history
