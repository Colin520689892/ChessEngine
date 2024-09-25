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
		//����ÿ���ڵ�,���������ӽڵ�
		for (int i = 0; i < ALPHABET; i++) {
			if (trie[x][i] == 0) {//����ӽڵ㲻����
				trie[x][i] = trie[fail[x]][i];//������ֵ��Ϊʧ��ָ��ָ��ڵ���ӽڵ�
				//������ʧ��ָ��ÿ��ָ��Ķ������׺��λ��,���������ƥ�䵽��һ���ַ�����ȷ��
			}
			else {//����ڵ����
				fail[trie[x][i]] = trie[fail[x]][i];//���ӽڵ�ʧ��ָ����ڸ��ڵ��ʧ��ָ����ָ����ӽڵ�
				q.push(trie[x][i]);//�������������
			}
		}
	}
}
std::vector<int> AhoCorasick::search(std::string S) {
	f.assign(tot + 1,0);
	int p = 1;
	for (auto c : S) {//����ÿһ���ַ�
		p = trie[p][c - convert];//���ڵ������������ַ�
		f[p]++;//��f[p]++;
	}
	//�����һ�����ʵĽڵ㿪ʼ����
	for (int i = nodes.size() - 1; i >= 0; i--) {
		int x = nodes[i];//�õ��ýڵ�����
		f[fail[x]] += f[x];//���x��������ַ������ֹ�,���������׺Ҳ���ֹ�
		//��x�����׺��ָ���ĩβ�ַ�����failָ����ָ���ĩβ�ַ�
	}
	return f;
}