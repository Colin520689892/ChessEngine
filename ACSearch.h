#pragma once
#include "root.h"

constexpr int N = 50;//表示节点数
constexpr int ALPHABET = 3;//子节点数
constexpr char convert = '0';//转换成数字
class AhoCorasick {
private:
	int trie[N][ALPHABET];
	int fail[N];
	int tot = 1, Q = 0;
	std::vector<int> nodes, f;
public:
	AhoCorasick(int _Q);
	int add(std::string& s);
	void work();
	std::vector<int> search(std::string S);
};