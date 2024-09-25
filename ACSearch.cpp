#include"ACSearch.h"
AhoCorasick::AhoCorasick(int _Q) :Q{ _Q } {
	for (int i = 0; i < ALPHABET; i++) {
		trie[0][i] = 1;
	}
}
int AhoCorasick::add(std::string& s) {
	int p = 1;
	for (auto c : s) {
		int& q = trie[p][c - convert];
		if (q == 0)q = ++tot;
		p = q;
	}
	return p;
}
void AhoCorasick::work() {
	std::queue<int> q;
	q.push(1);

	while (!q.empty()) {
		int x = q.front();
		nodes.push_back(x);
		q.pop();
		//对于每个节点,访问它的子节点
		for (int i = 0; i < ALPHABET; i++) {
			if (trie[x][i] == 0) {//如果子节点不存在
				trie[x][i] = trie[fail[x]][i];//将它的值设为失配指针指向节点的子节点
				//理由是失配指针每次指向的都是最长后缀的位置,所以如果能匹配到下一个字符是正确的
			}
			else {//如果节点存在
				fail[trie[x][i]] = trie[fail[x]][i];//该子节点失配指针等于父节点的失配指针所指向的子节点
				q.push(trie[x][i]);//并将它放入队列
			}
		}
	}
}
std::vector<int> AhoCorasick::search(std::string S) {
	f.assign(tot + 1,0);
	int p = 1;
	for (auto c : S) {//遍历每一个字符
		p = trie[p][c - convert];//根节点下如果有这个字符
		f[p]++;//让f[p]++;
	}
	//从最后一个访问的节点开始回溯
	for (int i = nodes.size() - 1; i >= 0; i--) {
		int x = nodes[i];//拿到该节点的序号
		f[fail[x]] += f[x];//如果x所代表的字符串出现过,代表其最长后缀也出现过
		//而x的最长后缀所指向的末尾字符就是fail指针所指向的末尾字符
	}
	return f;
}