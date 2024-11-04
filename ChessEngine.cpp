#include "ChessEngine.h"
#include "ACSearch.h"
#include "Zobrist.h"

#include "PossiblePositionManager.h"

namespace ChessEngine {

	//用于ac自动机
	int DEPTH = 7
		;//递归深度
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
	//Zobrist缓存
	Zobrist z;
	//可能位置的管理
	PossiblePositionManager ppm;
	//棋盘
	char board[BOARD_WIDTH][BOARD_WIDTH];
	int winner;
	std::stack<Position>moves;
	Position searchResult;
	int scores[2][72];//保存棋局分数(2个角色72行,包括横竖撇捺)
	int allScore[2];//局面总评分,两角色

	char which(char who, int role) {
		return (who == role ? '1' : (who == 0 ? '0' : '2'));
	}
	//这个是对点p周围的10个点进行评分
	int evaluatePoint(char board[BOARD_WIDTH][BOARD_WIDTH], Position p) {
		int result=0;
		std::string lines[4], lines1[4];//lines是Human,lines1是Computer
		//因为它模式串最多有6个字符,所以扫描点p的左右5个点即可
		//先假设这个点已经下了,但是在这个点无论是电脑还是人类,它都要下1
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
			std::vector<int>f = ac.search(lines[i]);//人类
			for (int j = 0; j < Q; j++)re1 += patterns[j].score * f[end[j]];
			f = ac.search(lines1[i]);//电脑,电脑所拿的分数
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
	//这个是对全局评分,updatescore就是更新allscore的
	void updateScore(char board[BOARD_WIDTH][BOARD_WIDTH], Position p) {
		std::string lines[4],lines1[4];//lines是Human,lines1是Computer
		//那么对于人类来说,如果该点是'1',表示自己,所以是1,其它的另外判断
		//对于电脑来说,如果该点是1,表示对手,如果不是1,判断它是不是0
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
		int a = p.y;//竖
		int b = BOARD_WIDTH + p.x;//横
		int c = 2 * BOARD_WIDTH + (p.y - p.x + 10);//捺
		int d = 2 * BOARD_WIDTH + 21 + (p.x + p.y - 4);
		for (int i = 0; i < 2; i++) {//减去以前的记录
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
		//判断他们是否在界限内
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
	//首先是一个棋盘,然后是递归的深度,
	int abSearch(char board[BOARD_WIDTH][BOARD_WIDTH], int depth,int alpha,int beta,Role currentSearchRole) {
		Flag flag = ALPHA;
		int score = z.get(depth, alpha, beta);
		if (score != UNKNOWN_SCORE && depth != DEPTH)return score;
		int score1 = getscore(currentSearchRole);//拿到当前的分数,0是人类的,1是电脑的
		int score2 = getscore(currentSearchRole == HUMAN ? COMPUTER : HUMAN);//不管怎么样,当前局面的分数都是电脑减去人类
		if (score1 >= 50000)return MAX_SCORE - 1000 - (DEPTH - depth);//如果玩家的分数和
		if (score2 >= 50000)return MIN_SCORE + 1000 + (DEPTH - depth);
		if (depth == 0) {//这个地方逻辑是对的,局面的分数是由电脑的分数减去玩家的分数,如果它是最底下一层,那么它是被计算过的确切值
			z.add(depth, score1 - score2, EXACT);
			return score1 - score2;
		}
		int count = 0;
		std::set<Position>possiblePosition;
		const std::set<Position>&tmpPossiblePosition = ppm.get();
		std::set<Position>::iterator iter;//而set是从小排到大的,所以应该用人类减去电脑
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
			if (val >= beta) {//这个点计算出来的值是如果大于该点,那说明要剪枝
				z.add(depth, beta, BETA);//这个时候要剪枝了,对这个说明alpha已经大于beta了,那么这个点至少是16
				return beta;
			}
			if (val > alpha) {//这个点计算出来的值如果大于alpha值
				flag = EXACT;//说明它能搜到一个更好的情况,且没有超过beta的范围
				alpha = val;
				if (depth == DEPTH)searchResult = p;
			}
			count++;
			if (count >= 9)break;
		}
		z.add(depth, alpha, flag);//将所有点都搜完了,却没有找到更大的那说明这个点至多是16
		return alpha;
	}
	void print() {
		printBoard(board);
	}
	Position getGoodMove(char board[BOARD_WIDTH][BOARD_WIDTH]) {
		//一开始是电脑,那么应该按照人类-电脑从小到大排序,然后向下递归以后,alpha=-beta.
		//原本beta是求人类-电脑的最大值,现在-beta变成了求电脑减去人类的最小值
		int score = abSearch(board, DEPTH, MIN_SCORE, MAX_SCORE, COMPUTER);
		if (score >= MAX_SCORE - 1000 - 1)winner = COMPUTER;
		else if (score <= MIN_SCORE + 1000 + 1)winner = HUMAN;
		return searchResult;
	}
	void init() {
		//将模式串都放到ac自动机里,end[i]表示每个模式串的结束节点位置
		winner = -1;
		for (int i = 0; i < Q; i++) end[i]=ac.add(patterns[i].pattern);
		ac.work();//生成fail指针
		z.init();//然后初始化Zobrist哈希表
		ppm.clear();
	}



	//一下是对外接口的实现------------------------------------------------------------------------------------------------------
	//在开始之前,一些初始化工作
	void beforeStart() {
		memset(board, Role_EMPTY, BOARD_WIDTH * BOARD_WIDTH * sizeof(char));//初始化borad
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

	std::string takeBack() {//将棋盘每个字符按顺序拼凑成字符串并返回
		if (moves.size() < 2) {//由于最后一步棋是电脑走的,所以要悔两步棋才算悔棋
			std::cout << "can not take back" << "\n";
			std::string resultStr;
			for (int i = 0; i < BOARD_WIDTH; i++) {
				for (int j = 0; j < BOARD_WIDTH; j++) {
					resultStr.push_back(board[i][j] + 48);
				}
			}
			return resultStr;
		}
		Position previousPosition = moves.top();//moves存的是走的步数,
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