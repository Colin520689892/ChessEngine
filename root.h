#pragma once

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define BOARD_WIDTH 15//���̴�С
#define INDEX 0xffff//hashitems�Ĵ�С65535
#define UNKNOWN_SCORE 10000001
#define MAX_SCORE (10000000)
#define MIN_SCORE (-10000000)
constexpr int DEPTH = 7;//�ݹ����
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
//�����8������
//����ǽڵ�ṹ��
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
