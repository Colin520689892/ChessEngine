#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#define BOARD_WIDTH 15//棋盘大小
#define INDEX 0xffff//hashitems的大小65535
#define UNKNOWN_SCORE 10000001
#define MAX_SCORE (10000000)
#define MIN_SCORE (-10000000)
constexpr int DEPTH = 7;//递归深度
#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<array>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<chrono>
#include<string>
#include<random>
#include <stack>
#include<queue>
using ll = long long;
//这个是8个方向
//这个是节点结构体
struct Position {
	int x;
	int y;
	int score;
	Position() {}
	Position(int x, int y, int score = 0) {
		this->x = x;
		this->y = y;
		this->score = score;
	}
	bool operator<(const Position& pos)const {
		if (score != pos.score)return score > pos.score;
		if (x != pos.x)return x < pos.x;
		else return y < pos.y;
	}
};
