#include "ChessEngine.h"
#include "ACSearch.h"
#include "Zobrist.h"

#include "PossiblePositionManager.h"

namespace ChessEngine {

	//����ac�Զ���
	int DEPTH = 7
		;//�ݹ����
	struct Pattern {
		std::string pattern;
		int score;
	};
	std::vector<Pattern>patterns = {
		{ "11111",  50000 },
		{ "011110", 4320 },
		{ "011100", 720 },
		{ "001110", 720 },
		{ "011010", 720 },
		{ "010110", 720 },
		{ "11110",  720 },
		{ "01111",  720 },
		{ "11011",  720 },
		{ "10111",  720 },
		{ "11101",  720 },
		{ "001100", 120 },
		{ "001010", 120 },
		{ "010100", 120 },
		{ "000100", 20 },
		{ "001000", 20 }
	};
	int Q = patterns.size();
	AhoCorasick ac(Q);
	std::vector<int>end(Q);
	//Zobrist����
	Zobrist z;
	//����λ�õĹ���
	PossiblePositionManager ppm;
	//����
	char board[BOARD_WIDTH][BOARD_WIDTH];
	int winner;
	std::stack<Position>moves;
	Position searchResult;
	int scores[2][72];//������ַ���(2����ɫ72��,��������Ʋ��)
	int allScore[2];//����������,����ɫ

	char which(char who, int role) {
		return (who == role ? '1' : (who == 0 ? '0' : '2'));
	}
	//����ǶԵ�p��Χ��10�����������
	int evaluatePoint(char board[BOARD_WIDTH][BOARD_WIDTH], Position p) {
		int result=0;
		std::string lines[4], lines1[4];//lines��Human,lines1��Computer
		//��Ϊ��ģʽ�������6���ַ�,����ɨ���p������5���㼴��
		//�ȼ���������Ѿ�����,����������������ǵ��Ի�������,����Ҫ��1
		for (int i = std::max(0, p.x - 5); i < std::min(BOARD_WIDTH, p.x + 6); i++) {
			i == p.x ? lines[0].push_back('1') : lines[0].push_back(which(board[i][p.y], HUMAN));
			i == p.x ? lines1[0].push_back('1') : lines1[0].push_back(which(board[i][p.y], COMPUTER));
		}
		for (int i = std::max(0, p.y - 5); i < std::min(BOARD_WIDTH, p.y + 6); i++) {
			i == p.y ? lines[1].push_back('1') : lines[1].push_back(which(board[p.x][i], HUMAN));
			i == p.y ? lines1[1].push_back('1') : lines1[1].push_back(which(board[p.x][i], COMPUTER));
		}
		for (int i = p.x - std::min(std::min(p.x, p.y), 5), j = p.y - std::min(std::min(p.x, p.y), 5); i < std::min(BOARD_WIDTH, p.x + 6) && j < std::min(BOARD_WIDTH, p.y + 6); i++,j++ ) {
			i == p.x ? lines[2].push_back('1') : lines[2].push_back(which(board[i][j], HUMAN));
			i == p.x ? lines1[2].push_back('1') : lines1[2].push_back(which(board[i][j], COMPUTER));
		}
		for (int i = p.x + std::min(std::min(p.y, BOARD_WIDTH - 1 - p.x), 5), j = p.y - std::min(std::min(p.y, BOARD_WIDTH - 1 - p.x), 5); i >= std::max(0, p.x - 5) && j < std::min(BOARD_WIDTH, p.y + 6); i--, j++) {
			i == p.x ? lines[3].push_back('1') : lines[3].push_back(which(board[i][j], HUMAN));
			i == p.x ? lines1[3].push_back('1') : lines1[3].push_back(which(board[i][j], COMPUTER));
		}
		ll re1 = 0, re2 = 0;
		for (int i = 0; i < 4; i++) {
			std::vector<int>f = ac.search(lines[i]);//����
			for (int j = 0; j < Q; j++)re1 += patterns[j].score * f[end[j]];
			f = ac.search(lines1[i]);//����,�������õķ���
			for (int j = 0; j < Q; j++)re2 += patterns[j].score * f[end[j]];
		}
		return re1 + re2;
	}
	void printBoard(char board[BOARD_WIDTH][BOARD_WIDTH]) {
		int i, j;
		for (i = 0; i < BOARD_WIDTH; i++) {
			for (j = 0; j < BOARD_WIDTH; j++) {
				std::cout << (int)board[i][j] << " ";
			}
			std::cout << "\n";
		}
	}
	int getscore(Role role) {
		return (role == HUMAN ? allScore[0] : allScore[1]);
	}
	//����Ƕ�ȫ������,updatescore���Ǹ���allscore��
	void updateScore(char board[BOARD_WIDTH][BOARD_WIDTH], Position p) {
		std::string lines[4],lines1[4];//lines��Human,lines1��Computer
		//��ô����������˵,����õ���'1',��ʾ�Լ�,������1,�����������ж�
		//���ڵ�����˵,����õ���1,��ʾ����,�������1,�ж����ǲ���0
		for (int i = 0; i < BOARD_WIDTH; i++) {
			lines[0].push_back(which(board[i][p.y],HUMAN));
			lines1[0].push_back(which(board[i][p.y],COMPUTER));
		}
		for (int i = 0; i < BOARD_WIDTH; i++) {
			lines[1].push_back(which(board[p.x][i],HUMAN));
			lines1[1].push_back(which(board[p.x][i],COMPUTER));
		}
		for (int i = p.x - std::min(p.x, p.y), j = p.y - std::min(p.x, p.y); i < BOARD_WIDTH && j < BOARD_WIDTH; i++, j++) {
			lines[2].push_back(which(board[i][j],HUMAN));
			lines1[2].push_back(which(board[i][j],COMPUTER));
		}
		for (int i = p.x + std::min(p.y, BOARD_WIDTH - 1 - p.x), j = p.y - std::min(p.y, BOARD_WIDTH - 1 - p.x); i >= 0 && j < BOARD_WIDTH; i--, j++) {
			lines[3].push_back(which(board[i][j],HUMAN));
			lines1[3].push_back(which(board[i][j],COMPUTER));
		}
		std::vector<int> lineScore(4), line1Score(4);
		for (int i = 0; i < 4; i++) {
			std::vector<int>f = ac.search(lines[i]);
			for (int j = 0; j < Q; j++)lineScore[i] += patterns[j].score * f[end[j]];
			f = ac.search(lines1[i]);
			for (int j = 0; j < Q; j++)line1Score[i] += patterns[j].score * f[end[j]];
		}
		int a = p.y;//��
		int b = BOARD_WIDTH + p.x;//��
		int c = 2 * BOARD_WIDTH + (p.y - p.x + 10);//��
		int d = 2 * BOARD_WIDTH + 21 + (p.x + p.y - 4);
		for (int i = 0; i < 2; i++) {//��ȥ��ǰ�ļ�¼
			allScore[i] -= scores[i][a];
			allScore[i] -= scores[i][b];
		}
		scores[0][a] = lineScore[0];
		scores[1][a] = line1Score[0];
		scores[0][b] = lineScore[1];
		scores[1][b] = line1Score[1];
		for (int i = 0; i < 2; i++) {
			allScore[i] += scores[i][a];
			allScore[i] += scores[i][b];
		}
		//�ж������Ƿ��ڽ�����
		if (p.y - p.x >= -10 && p.y - p.x <= 10) {
			for (int i = 0; i < 2; i++)allScore[i] -= scores[i][c];
			scores[0][c] = lineScore[2];
			scores[1][c] = line1Score[2];
			for (int i = 0; i < 2; i++)allScore[i] += scores[i][c];
		}
		if (p.x + p.y >= 4 && p.x + p.y <= 24) {
			for (int i = 0; i < 2; i++)allScore[i] -= scores[i][d];
			scores[0][d] = lineScore[3];
			scores[1][d] = line1Score[3];
			for (int i = 0; i < 2; i++)allScore[i] += scores[i][d];
		}
	}
	//������һ������,Ȼ���ǵݹ�����,
	int abSearch(char board[BOARD_WIDTH][BOARD_WIDTH], int depth,int alpha,int beta,Role currentSearchRole) {
		Flag flag = ALPHA;
		int score = z.get(depth, alpha, beta);
		if (score != UNKNOWN_SCORE && depth != DEPTH)return score;
		int score1 = getscore(currentSearchRole);//�õ���ǰ�ķ���,0�������,1�ǵ��Ե�
		int score2 = getscore(currentSearchRole == HUMAN ? COMPUTER : HUMAN);//������ô��,��ǰ����ķ������ǵ��Լ�ȥ����
		if (score1 >= 50000)return MAX_SCORE - 1000 - (DEPTH - depth);//�����ҵķ�����
		if (score2 >= 50000)return MIN_SCORE + 1000 + (DEPTH - depth);
		if (depth == 0) {//����ط��߼��ǶԵ�,����ķ������ɵ��Եķ�����ȥ��ҵķ���,������������һ��,��ô���Ǳ��������ȷ��ֵ
			z.add(depth, score1 - score2, EXACT);
			return score1 - score2;
		}
		int count = 0;
		std::set<Position>possiblePosition;
		const std::set<Position>&tmpPossiblePosition = ppm.get();
		std::set<Position>::iterator iter;//��set�Ǵ�С�ŵ����,����Ӧ���������ȥ����
		for (iter = tmpPossiblePosition.begin(); iter != tmpPossiblePosition.end(); iter++) {
			possiblePosition.insert(Position(iter->x, iter->y, evaluatePoint(board, *iter)));
		}
		while (!possiblePosition.empty()) {
			Position p = *possiblePosition.begin();
			possiblePosition.erase(possiblePosition.begin());
			board[p.x][p.y] = currentSearchRole;//1
			z.CurrentZVal ^= z.BoardZVal[currentSearchRole - 1][p.x][p.y];//2
			updateScore(board, p);//3
			p.score = 0;
			ppm.add(board, p);//4
			int val = -abSearch(board, depth - 1, -beta, -alpha, currentSearchRole == HUMAN ? COMPUTER : HUMAN);
			if (depth == DEPTH)
				std::cout << "score(" << p.x << "," << p.y << "):" << val << "\n";
			ppm.Rollback();//4
			board[p.x][p.y] = 0;//1
			z.CurrentZVal ^= z.BoardZVal[currentSearchRole - 1][p.x][p.y];//2
			updateScore(board, p);//3
			if (val >= beta) {//�������������ֵ��������ڸõ�,��˵��Ҫ��֦
				z.add(depth, beta, BETA);//���ʱ��Ҫ��֦��,�����˵��alpha�Ѿ�����beta��,��ô�����������16
				return beta;
			}
			if (val > alpha) {//�������������ֵ�������alphaֵ
				flag = EXACT;//˵�������ѵ�һ�����õ����,��û�г���beta�ķ�Χ
				alpha = val;
				if (depth == DEPTH)searchResult = p;
			}
			count++;
			if (count >= 9)break;
		}
		z.add(depth, alpha, flag);//�����е㶼������,ȴû���ҵ��������˵�������������16
		return alpha;
	}
	void print() {
		printBoard(board);
	}
	Position getGoodMove(char board[BOARD_WIDTH][BOARD_WIDTH]) {
		//һ��ʼ�ǵ���,��ôӦ�ð�������-���Դ�С��������,Ȼ�����µݹ��Ժ�,alpha=-beta.
		//ԭ��beta��������-���Ե����ֵ,����-beta���������Լ�ȥ�������Сֵ
		int score = abSearch(board, DEPTH, MIN_SCORE, MAX_SCORE, COMPUTER);
		if (score >= MAX_SCORE - 1000 - 1)winner = COMPUTER;
		else if (score <= MIN_SCORE + 1000 + 1)winner = HUMAN;
		return searchResult;
	}
	void init() {
		//��ģʽ�����ŵ�ac�Զ�����,end[i]��ʾÿ��ģʽ���Ľ����ڵ�λ��
		winner = -1;
		for (int i = 0; i < Q; i++) end[i]=ac.add(patterns[i].pattern);
		ac.work();//����failָ��
		z.init();//Ȼ���ʼ��Zobrist��ϣ��
		ppm.clear();
	}



	//һ���Ƕ���ӿڵ�ʵ��------------------------------------------------------------------------------------------------------
	//�ڿ�ʼ֮ǰ,һЩ��ʼ������
	void beforeStart() {
		memset(board, Role_EMPTY, BOARD_WIDTH * BOARD_WIDTH * sizeof(char));//��ʼ��borad
		memset(scores, 0, sizeof(scores));
		init();
	}
	int isSomeOneWin() {
		if (winner == HUMAN)return 0;
		if (winner == COMPUTER)return 1;
		return -1;
	}

	void dian() {
		moves.push(Position(7, 7));
		board[7][7] = COMPUTER;
		z.CurrentZVal ^= z.BoardZVal[COMPUTER - 1][7][7];
		ppm.add(board, Position(7, 7));
		updateScore(board, { 7,7 });
	}

	std::string takeBack() {//������ÿ���ַ���˳��ƴ�ճ��ַ���������
		if (moves.size() < 2) {//�������һ�����ǵ����ߵ�,����Ҫ��������������
			std::cout << "can not take back" << "\n";
			std::string resultStr;
			for (int i = 0; i < BOARD_WIDTH; i++) {
				for (int j = 0; j < BOARD_WIDTH; j++) {
					resultStr.push_back(board[i][j] + 48);
				}
			}
			return resultStr;
		}
		Position previousPosition = moves.top();//moves������ߵĲ���,
		moves.pop();
		z.CurrentZVal ^= z.BoardZVal[COMPUTER - 1][previousPosition.x][previousPosition.y];
		board[previousPosition.x][previousPosition.y] = Role_EMPTY;
		updateScore(board, previousPosition);
		ppm.Rollback();

		previousPosition = moves.top();
		moves.pop();
		z.CurrentZVal ^= z.BoardZVal[HUMAN - 1][previousPosition.x][previousPosition.y];
		board[previousPosition.x][previousPosition.y] = Role_EMPTY;
		updateScore(board, previousPosition);
		
		ppm.Rollback();
		std::string resultStr;
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				resultStr.push_back(board[i][j] + 48);
			}
		}
		winner = -1;
		return resultStr;
	}
	
	Position getLastPosition() {
		return searchResult;
	}
	void setLevel(int level) {
		DEPTH = level;
	}

	std::string nextStep(int x, int y) {
		moves.push(Position(x, y));
		board[x][y] = HUMAN;
		z.CurrentZVal ^= z.BoardZVal[HUMAN - 1][x][y];
		updateScore(board, Position(x, y));
		ppm.add(board, Position(x, y));
		Position result = getGoodMove(board);
		board[result.x][result.y] = COMPUTER;
		z.CurrentZVal ^= z.BoardZVal[COMPUTER - 1][result.x][result.y];
		updateScore(board, result);
		ppm.add(board, result);
		if (winner == -1)moves.push(Position(result.x, result.y));
		std::string resultStr;
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				resultStr.push_back(board[i][j] + 48);
			}
		}
		return resultStr;
	}
}