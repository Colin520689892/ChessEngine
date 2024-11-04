#pragma once
#include "root.h"

constexpr int N = 50;//��ʾ�ڵ���
constexpr int ALPHABET = 3;//�ӽڵ���
constexpr char convert = '0';//ת��������
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